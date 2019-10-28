//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools
// installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR
// INDEMNITIES.
//
#include "stdafx.h"
#include "raster.hpp"
#include "raster.inl"

template <class R> inline float TScalar(float lhs, float rhs) {
  ASSERT(lhs != rhs);
  return static_cast<R>(lhs / (lhs - rhs));
}


template <class R, unsigned int T>
inline R TScalar(TFixed<T> lhs, TFixed<T> rhs) {
  ASSERT(lhs.GetRaw() != rhs.GetRaw());
  const int diff = lhs.GetRaw() - rhs.GetRaw();
  return R::Make((static_cast<int64_t>(lhs.GetRaw()) << R::FRAC) / diff);
}


template <unsigned int T>
inline int CullSign(TFixed<T> x0, TFixed<T> y0, TFixed<T> w0, TFixed<T> x1,
                    TFixed<T> y1, TFixed<T> w1, TFixed<T> x2, TFixed<T> y2,
                    TFixed<T> w2) {
  const int64_t _x0 = static_cast<int64_t>(x0.GetRaw());
  const int64_t _x1 = static_cast<int64_t>(x1.GetRaw());
  const int64_t _x2 = static_cast<int64_t>(x2.GetRaw());

  const int SHIFT = 2 * TFixed<T>::FRAC - 12;

  const int64_t sign12 =
      (w0.GetRaw() * ((_x1 * y2.GetRaw() - _x2 * y1.GetRaw()) >> SHIFT) -
       w1.GetRaw() * ((_x0 * y2.GetRaw() - _x2 * y0.GetRaw()) >> SHIFT) +
       w2.GetRaw() * ((_x0 * y1.GetRaw() - _x1 * y0.GetRaw()) >> SHIFT)) >>
      TFixed<T>::FRAC;

  if (sign12 < 0) {
    return -1;
  } else if (sign12 > 0) {
    return 1;
  }

  return 0;
}


inline int CullSign(float x0, float y0, float w0, float x1, float y1, float w1,
                    float x2, float y2, float w2) {
  const float sign = (w0 * (x1 * y2 - x2 * y1) - w1 * (x0 * y2 - x2 * y0) +
                      w2 * (x0 * y1 - x1 * y0));

  if (sign < 0) {
    return -1;
  } else if (sign > 0) {
    return 1;
  }

  return 0;
}


bool CRasterizer::CullClipSpaceTriangle(unsigned i0, unsigned i1, unsigned i2) {
  bool bIsCulled;

  const CullStates cullStates = m_cullStates;

  if (CULL_FRONT_AND_BACK != cullStates.CullFace) {
    const VECTOR4 *const pvClipPos =
        (const VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];
    const VECTOR4 &v0 = pvClipPos[i0];
    const VECTOR4 &v1 = pvClipPos[i1];
    const VECTOR4 &v2 = pvClipPos[i2];

    const floatf x0 = v0.x;
    const floatf y0 = v0.y;
    const floatf w0 = v0.w;

    const floatf x1 = v1.x;
    const floatf y1 = v1.y;
    const floatf w1 = v1.w;

    const floatf x2 = v2.x;
    const floatf y2 = v2.y;
    const floatf w2 = v2.w;

    const int sign = CullSign(x0, y0, w0, x1, y1, w1, x2, y2, w2);

    if (CULL_BACK == cullStates.CullFace) {
      bIsCulled = (sign < 0) ^ cullStates.FrontFace;
    } else {
      bIsCulled = (sign > 0) ^ cullStates.FrontFace;
    }
  } else {
    bIsCulled = true;
  }

  if (cullStates.bCullFaceEnabled) {
    if (bIsCulled) {
      return false;
    }
  }

  const unsigned colorIndex = cullStates.bTwoSidedLighting && bIsCulled;
  m_pbVertexColor = m_pbVertexData[VERTEXDATA_COLOR0 + colorIndex];

  return true;
}


void CRasterizer::RasterClippedLine(unsigned i0, unsigned i1,
                                    unsigned clipUnion) {
  const VECTOR4 *const pvClipPos =
      (const VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];
  const uint16_t *const pwFlags = (const uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  RDVECTOR *const pvScreenPos =
      (RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];

  unsigned iNext = m_clipVerticesBaseIndex;
  unsigned iFrom = i0;
  unsigned iTo = i1;

  floatf fDistA, fDistB;

  for (unsigned plane = 0; clipUnion; clipUnion >>= 1, ++plane) {
    if (clipUnion & 0x1) {
      const VECTOR4 &vFrom = pvClipPos[iFrom];
      const VECTOR4 &vTo = pvClipPos[iTo];

      switch (plane) {
      default:
        __no_default;

      case CLIP_LEFT:
        fDistA = vFrom.x + vFrom.w;
        fDistB = vTo.x + vTo.w;
        break;

      case CLIP_RIGHT:
        fDistA = vFrom.w - vFrom.x;
        fDistB = vTo.w - vTo.x;
        break;

      case CLIP_TOP:
        fDistA = vFrom.w - vFrom.y;
        fDistB = vTo.w - vTo.y;
        break;

      case CLIP_BOTTOM:
        fDistA = vFrom.y + vFrom.w;
        fDistB = vTo.y + vTo.w;
        break;

      case CLIP_FRONT:
        fDistA = vFrom.z + vFrom.w;
        fDistB = vTo.z + vTo.w;
        break;

      case CLIP_BACK:
        fDistA = vFrom.w - vFrom.z;
        fDistB = vTo.w - vTo.z;
        break;

      case CLIP_PLANE0:
      case CLIP_PLANE1:
      case CLIP_PLANE2:
      case CLIP_PLANE3:
      case CLIP_PLANE4:
      case CLIP_PLANE5:
        ASSERT(plane < __countof(m_vClipPlanesCS));
        fDistA = Math::TDot<floatf>(vFrom, m_vClipPlanesCS[plane]);
        fDistB = Math::TDot<floatf>(vTo, m_vClipPlanesCS[plane]);
        break;
      }

      if (fDistA >= fZERO) {
        if (fDistB >= fZERO) {
          continue;
        }

        this->InterpolateVertex(iTo, iFrom, fDistA, fDistB, iNext);
        iTo = iNext++;
      } else if (fDistB < fZERO) {
        continue;
      }

      this->InterpolateVertex(iFrom, iTo, fDistB, fDistA, iNext);
      iFrom = iNext++;
    }
  }

  // Transform clipped vertices back to screen space
  if (pwFlags[iFrom] & CLIPPING_MASK) {
    this->XformScreenSpace(&pvScreenPos[iFrom], &pvClipPos[iFrom], 1);
  }

  if (pwFlags[iTo] & CLIPPING_MASK) {
    this->XformScreenSpace(&pvScreenPos[iTo], &pvClipPos[iTo], 1);
  }

  // Raster the resulting line
  this->RasterLine(iFrom, iTo);
}


void CRasterizer::RasterClippedTriangle(unsigned i0, unsigned i1, unsigned i2,
                                        unsigned clipUnion) {
  const VECTOR4 *const pvClipPos =
      (const VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];
  const uint16_t *const pwFlags = (const uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  RDVECTOR *const pvScreenPos =
      (RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];

  unsigned clipVertices[2][CLIP_BUFFER_SIZE];
  unsigned iTmpVertices = m_clipVerticesBaseIndex;
  unsigned *pSrc = clipVertices[0];
  unsigned *pDst = clipVertices[1];

  pSrc[0] = i0;
  pSrc[1] = i1;
  pSrc[2] = i2;

  unsigned nNumVertices = 3;
  const unsigned clz = Clz(clipUnion);
  clipUnion <<= clz;

  for (unsigned plane = 31 - clz; clipUnion; clipUnion <<= 1, --plane) {
    if (clipUnion & 0x80000000) {
      switch (plane) {
      default:
        __no_default;

      case CLIP_LEFT:
        nNumVertices = this->TClipTriangle<CLIP_LEFT>(nNumVertices, pSrc, pDst,
                                                      &iTmpVertices);
        break;

      case CLIP_RIGHT:
        nNumVertices = this->TClipTriangle<CLIP_RIGHT>(nNumVertices, pSrc, pDst,
                                                       &iTmpVertices);
        break;

      case CLIP_TOP:
        nNumVertices = this->TClipTriangle<CLIP_TOP>(nNumVertices, pSrc, pDst,
                                                     &iTmpVertices);
        break;

      case CLIP_BOTTOM:
        nNumVertices = this->TClipTriangle<CLIP_BOTTOM>(nNumVertices, pSrc,
                                                        pDst, &iTmpVertices);
        break;

      case CLIP_FRONT:
        nNumVertices = this->TClipTriangle<CLIP_FRONT>(nNumVertices, pSrc, pDst,
                                                       &iTmpVertices);
        break;

      case CLIP_BACK:
        nNumVertices = this->TClipTriangle<CLIP_BACK>(nNumVertices, pSrc, pDst,
                                                      &iTmpVertices);
        break;

      case CLIP_PLANE0:
      case CLIP_PLANE1:
      case CLIP_PLANE2:
      case CLIP_PLANE3:
      case CLIP_PLANE4:
      case CLIP_PLANE5:
        nNumVertices = this->UserClipTriangle(plane, nNumVertices, pSrc, pDst,
                                              &iTmpVertices);
        break;
      }

      if (nNumVertices < 3) {
        return;
      }

      __swap(pSrc, pDst);
    }
  }

  // Transform clipped vertices back to screen space
  for (unsigned i = 0; i < nNumVertices; ++i) {
    const unsigned index = pSrc[i];
    if (pwFlags[index] & CLIPPING_MASK) {
      this->XformScreenSpace(&pvScreenPos[index], &pvClipPos[index], 1);
    }
  }

  // Raster each triangle
  {
    const unsigned v0 = *pSrc;
    for (unsigned *pV = pSrc + 1, *const pEnd = pSrc + nNumVertices - 1;
         pV != pEnd; ++pV) {
      this->RasterTriangle(v0, pV[0], pV[1]);
    }
  }
}


unsigned CRasterizer::UserClipTriangle(unsigned plane, unsigned nNumVertices,
                                       unsigned *pSrc, unsigned *pDst,
                                       unsigned *pTmp) {
  const VECTOR4 *const pvClipPos =
      (const VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];

  floatf fDistA, fDistB;

  unsigned iTmp = *pTmp;
  unsigned iVB = pSrc[nNumVertices - 1];
  unsigned iVA;

  unsigned nClipVertices = 0;

  plane -= CLIP_PLANE0;
  ASSERT(plane < m_vClipPlanesCS.GetSize());

  for (fDistB = Math::TDot<floatf>(pvClipPos[iVB], m_vClipPlanesCS[plane]);
       nNumVertices--; iVB = iVA, fDistB = fDistA) {
    iVA = *pSrc++;
    fDistA = Math::TDot<floatf>(pvClipPos[iVA], m_vClipPlanesCS[plane]);

    if (fDistB >= fZERO) {
      // Add vertex to the current list
      ASSERT(nClipVertices < CLIP_BUFFER_SIZE);
      pDst[nClipVertices++] = iVB;
      if (fDistA >= fZERO) {
        continue;
      }
    } else if (fDistA < fZERO) {
      continue;
    }

    // Compute the intersecting vertex
    this->InterpolateVertex(iVA, iVB, fDistA, fDistB, iTmp);

    // Add the new vertex to the current list
    ASSERT(nClipVertices < CLIP_BUFFER_SIZE);
    pDst[nClipVertices++] = iTmp++;
  }

  *pTmp = iTmp;

  return nClipVertices;
}


void CRasterizer::InterpolateVertex(unsigned i0, unsigned i1, floatf fDistA,
                                    floatf fDistB, unsigned i2) {
  uint16_t *const pwFlags = (uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  VECTOR4 *const pvClipPos = (VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];

  const float30 fScalar = TScalar<float30>(fDistA, fDistB);
  ASSERT((fScalar >= TConst<float30>::Zero()) &&
         (fScalar <= TConst<float30>::One()));

  pvClipPos[i2].x = Math::Lerpf(pvClipPos[i0].x, pvClipPos[i1].x, fScalar);
  pvClipPos[i2].y = Math::Lerpf(pvClipPos[i0].y, pvClipPos[i1].y, fScalar);
  pvClipPos[i2].z = Math::Lerpf(pvClipPos[i0].z, pvClipPos[i1].z, fScalar);
  pvClipPos[i2].w = Math::Lerpf(pvClipPos[i0].w, pvClipPos[i1].w, fScalar);

  const RASTERFLAGS rasterFlags = m_rasterID.Flags;

  if (rasterFlags.Color) {
    ColorARGB *const pcColors = (ColorARGB *)m_pbVertexColor;

    pcColors[i2] =
        ColorARGB(Math::Lerp(pcColors[i0].a, pcColors[i1].a, fScalar),
                  Math::Lerp(pcColors[i0].r, pcColors[i1].r, fScalar),
                  Math::Lerp(pcColors[i0].g, pcColors[i1].g, fScalar),
                  Math::Lerp(pcColors[i0].b, pcColors[i1].b, fScalar));
  }

  if (rasterFlags.NumTextures) {
    for (unsigned i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      TEXCOORD2 *const pvTexCoords =
          (TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + i];
      pvTexCoords[i2].m[0] =
          Math::Lerpf(pvTexCoords[i0].m[0], pvTexCoords[i1].m[0], fScalar);
      pvTexCoords[i2].m[1] =
          Math::Lerpf(pvTexCoords[i0].m[1], pvTexCoords[i1].m[1], fScalar);
    }
  }

  if (rasterFlags.Fog) {
    float20 *const pfFogs = (float20 *)m_pbVertexData[VERTEXDATA_FOG];
    pfFogs[i2] = Math::Lerpf(pfFogs[i0], pfFogs[i1], fScalar);
  }

  pwFlags[i2] = 1;
}
