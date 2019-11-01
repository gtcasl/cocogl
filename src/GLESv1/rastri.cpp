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

void CRasterizer::DrawTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  const uint16_t *const pwFlags =
      (const uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  const uint32_t flags0 = pwFlags[i0];
  const uint32_t flags1 = pwFlags[i1];
  const uint32_t flags2 = pwFlags[i2];

  // Check if we need to clip vertices
  const uint32_t clipUnion = (flags0 | flags1 | flags2) & CLIPPING_MASK;
  if (0 == clipUnion) {
    // Do back-face culling in screen space
    if (this->CullScreenSpaceTriangle(i0, i1, i2)) {
      // Raster the triangle
      this->RasterTriangle(i0, i1, i2);
    }
  } else {
    // Discard primitives falling outside of the same plane.
    const uint32_t clipIntersect = (flags0 & flags1 & flags2) & CLIPPING_MASK;
    if (0 == clipIntersect) {
      // Do Back-face culling in clip space
      if (this->CullClipSpaceTriangle(i0, i1, i2)) {
        // Clip and raster the triangle
        this->RasterClippedTriangle(i0, i1, i2, clipUnion);
      }
    }
  }
}

bool CRasterizer::CullScreenSpaceTriangle(uint32_t i0, uint32_t i1,
                                          uint32_t i2) {
  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const RDVECTOR &v0 = pvScreenPos[i0];
  const RDVECTOR &v1 = pvScreenPos[i1];
  const RDVECTOR &v2 = pvScreenPos[i2];

  const fixed4 fdx10 = v1.x - v0.x;
  const fixed4 fdy10 = v1.y - v0.y;
  const fixed4 fdx20 = v2.x - v0.x;
  const fixed4 fdy20 = v2.y - v0.y;

  uint32_t culled;

  const CullStates cullStates = m_cullStates;
  if (CULL_FRONT_AND_BACK != cullStates.CullFace) {
    const fixed8 fSign = Math::TFastMul<fixed8>(fdx10, fdy20) -
                         Math::TFastMul<fixed8>(fdx20, fdy10);

    if (CULL_BACK == cullStates.CullFace) {
      culled = static_cast<uint32_t>(fSign < TConst<fixed8>::Zero()) ^
               cullStates.FrontFace;
    } else {
      culled = static_cast<uint32_t>(fSign > TConst<fixed8>::Zero()) ^
               cullStates.FrontFace;
    }
  } else {
    culled = 1;
  }

  if (cullStates.bCullFaceEnabled) {
    if (culled) {
      return false;
    }
  }

  const uint32_t colorIndex = cullStates.bTwoSidedLighting && culled;
  m_pbVertexColor = m_pbVertexData[VERTEXDATA_COLOR0 + colorIndex];

  return true;
}

void CRasterizer::RasterTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  // Setup the triangle attributes
  if (!this->SetupTriangleAttributes(i0, i1, i2)) {
    return;
  }

  const PFN_Scanline pfnScanline = m_rasterData.pRasterOp->GetScanline();

  // Lookup screenspace positions
  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const RDVECTOR &v0 = pvScreenPos[i0];
  const RDVECTOR &v1 = pvScreenPos[i1];
  const RDVECTOR &v2 = pvScreenPos[i2];

  // Sort vertices by y-coordinate
  const RDVECTOR *vertices[3] = {&v0, &v1, &v2};
  if (v1.y < vertices[0]->y) {
    vertices[1] = vertices[0];
    vertices[0] = &v1;
  }

  if (v2.y < vertices[0]->y) {
    vertices[2] = vertices[1];
    vertices[1] = vertices[0];
    vertices[0] = &v2;
  } else if (v2.y < vertices[1]->y) {
    vertices[2] = vertices[1];
    vertices[1] = &v2;
  }

  // Setup triangle coordinates
  const fixed4 i4x0 = vertices[0]->x;
  const fixed4 i4x1 = vertices[1]->x;
  const fixed4 i4x2 = vertices[2]->x;

  const fixed4 i4y0 = vertices[0]->y;
  const fixed4 i4y1 = vertices[1]->y;
  const fixed4 i4y2 = vertices[2]->y;

  const fixed4 i4dx12 = (i4x1 - i4x0);
  const fixed4 i4dy12 = (i4y1 - i4y0);
  const fixed4 i4dx13 = (i4x2 - i4x0);

  const fixed4 i4dy13 = (i4y2 - i4y0);
  const fixed4 i4dx23 = (i4x2 - i4x1);
  const fixed4 i4dy23 = (i4y2 - i4y1);

  // Calculate the edge direction
  const fixed8 i8Area = Math::TFastMul<fixed8>(i4dx12, i4dy13) -
                        Math::TFastMul<fixed8>(i4dx13, i4dy12);
  const bool bMiddleIsRight = (i8Area >= TConst<fixed8>::Zero());
  const fixedDDA fRndCeil =
      fixedDDA::Make(fixedDDA::MASK) - TConst<fixedDDA>::Half();

  int y = Math::TMax<int>(Math::TCeili<int>(i4y0 - TConst<fixed4>::Half()),
                          m_scissorRect.top);

  const fixed4 i4Y0Diff = fixed4(y) - (i4y0 - TConst<fixed4>::Half());

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  m_rasterData.pRasterOp->StartProfile(Math::TCast<int>(Math::TAbs(i8Area)));
#endif

  fixedDDA fx0, fx1;
  fixedDDA fdx0, fdx1;

  if (i4dy12.GetRaw()) {
    const int y1 = Math::TMin<int>(
        Math::TCeili<int>(i4y1 - TConst<fixed4>::Half()), m_scissorRect.bottom);

    fdx0 = fixedDDA(i4dx12) / i4dy12;
    fdx1 = fixedDDA(i4dx13) / i4dy13;

    if (bMiddleIsRight) {
      __swap(fdx0, fdx1);
    }

    fx0 = fixedDDA(i4x0) + fdx0 * i4Y0Diff + fRndCeil;
    fx1 = fixedDDA(i4x0) + fdx1 * i4Y0Diff + fRndCeil;

    for (; y < y1; ++y, fx0 += fdx0, fx1 += fdx1) {
      const int x0 = Math::TMax<int>(Math::TCast<int>(fx0), m_scissorRect.left);
      const int x1 =
          Math::TMin<int>(Math::TCast<int>(fx1), m_scissorRect.right);

      if (x0 < x1) {
        (pfnScanline)(m_rasterData, y, x0, x1);
      }
    }
  } else {

    const fixedDDA fdx2 = fixedDDA(i4dx13) / i4dy13;
    const fixedDDA fx2 = fixedDDA(i4x0) + fdx2 * i4Y0Diff + fRndCeil;

    if (bMiddleIsRight) {
      fdx0 = fdx2;
      fx0 = fx2;
    } else {
      fdx1 = fdx2;
      fx1 = fx2;
    }
  }

  if (i4dy23.GetRaw()) {
    const int y2 = Math::TMin<int>(
        Math::TCeili<int>(i4y2 - TConst<fixed4>::Half()), m_scissorRect.bottom);

    const fixed4 i4Y1Diff = fixed4(y) - (i4y1 - TConst<fixed4>::Half());

    const fixedDDA fdx2 = fixedDDA(i4dx23) / i4dy23;
    const fixedDDA fx2 = fixedDDA(i4x1) + fdx2 * i4Y1Diff + fRndCeil;

    if (bMiddleIsRight) {
      fdx1 = fdx2;
      fx1 = fx2;
    } else {
      fdx0 = fdx2;
      fx0 = fx2;
    }

    for (; y < y2; ++y, fx0 += fdx0, fx1 += fdx1) {
      const int x0 = Math::TMax<int>(Math::TCast<int>(fx0), m_scissorRect.left);
      const int x1 =
          Math::TMin<int>(Math::TCast<int>(fx1), m_scissorRect.right);

      if (x0 < x1) {
        (pfnScanline)(m_rasterData, y, x0, x1);
      }
    }
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(
      end_time - start_time);
  m_rasterData.pRasterOp->EndProfile(elapsed_time.count());
#endif
}

bool CRasterizer::SetupTriangleAttributes(uint32_t i0, uint32_t i1,
                                          uint32_t i2) {
  // Lookup vertex screen positions
  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const RDVECTOR &v0 = pvScreenPos[i0];
  const RDVECTOR &v1 = pvScreenPos[i1];
  const RDVECTOR &v2 = pvScreenPos[i2];

  const fixed4 i4dx12 = v1.x - v0.x;
  const fixed4 i4dy12 = v1.y - v0.y;
  const fixed4 i4dx13 = v2.x - v0.x;
  const fixed4 i4dy13 = v2.y - v0.y;

  const fixed8 i8Area = Math::TFastMul<fixed8>(i4dx12, i4dy13) -
                        Math::TFastMul<fixed8>(i4dx13, i4dy12);

  // Reject small areas (1/4 x 1/4 = 1/16)
  const fixed8 u8Area = Math::TAbs(i8Area);
  if (u8Area.GetRaw() < 16) {
    return false;
  }

  const floatQ fRatio = Math::TInv<floatQ>(i8Area);
  TriangleGradient gradient(i4dx12, i4dy12, i4dx13, i4dy13, fRatio);

  // Backup the rasterID
  const RASTERID rasterID = m_rasterID;

  const RASTERFLAGS rasterFlags = rasterID.Flags;
  Register *pRegister = m_rasterData.Registers;

  if (rasterFlags.DepthTest) {
    pRegister = this->DepthGradient(pRegister, gradient, v0, v1, v2);
  }

  if (rasterFlags.Color) {
    pRegister = this->ColorGradient(pRegister, gradient, i0, i1, i2);
  }

  if (rasterFlags.NumTextures) {
    if ((u8Area > TConst<fixed8>::Half()) &&
        ((fixedRX(Math::TAbs(v1.rhw - v0.rhw)) > TConst<fixedRX>::Epsilon()) ||
         (fixedRX(Math::TAbs(v2.rhw - v0.rhw)) > TConst<fixedRX>::Epsilon()))) {
      pRegister =
          this->PerspectiveTextureGradient(pRegister, gradient, i0, i1, i2);
    } else {
      pRegister = this->AffineTextureGradient(pRegister, gradient, i0, i1, i2);
    }
  }

  if (rasterFlags.Fog) {
    pRegister = this->FogGradient(pRegister, gradient, i0, i1, i2);
  }

  // Set the reference offset
  m_rasterData.fRefX = v0.x - TConst<fixed4>::Half();
  m_rasterData.fRefY = v0.y - TConst<fixed4>::Half();

  // Generate the rasterization routine
  if (!this->GenerateRasterOp()) {
    m_error = GL_INVALID_OPERATION;
    __glLogError(_T("CRasterizer::GenerateRasterOp() failed.\r\n"));
    return false;
  }

  // Restore the rasterID
  m_rasterID = rasterID;

  return true;
}

Register *CRasterizer::DepthGradient(Register *pRegister,
                                     const TriangleGradient &gradient,
                                     const RDVECTOR &v0, const RDVECTOR &v1,
                                     const RDVECTOR &v2) {
  ASSERT(pRegister);

  const float20 delta[2] = {v1.z - v0.z, v2.z - v0.z};

  if ((fixed16(Math::TAbs(delta[0])) > TConst<fixed16>::Epsilon()) ||
      (fixed16(Math::TAbs(delta[1])) > TConst<fixed16>::Epsilon())) {
    pRegister->m[0] = gradient.TCalcDeltaX<fixedRX>(delta[0], delta[1]);
    pRegister->m[1] = gradient.TCalcDeltaY<fixedRX>(delta[0], delta[1]);
    m_rasterID.Flags.InterpolateDepth = 1;
  } else {
    pRegister->m[0] = TConst<fixedRX>::Zero();
    pRegister->m[1] = TConst<fixedRX>::Zero();
  }

  pRegister->m[2] = static_cast<fixedRX>(v0.z);

  if (m_caps.PolygonOffsetFill) {
    this->ApplyPolygonOffset(pRegister);
  }

  return pRegister + 1;
}

void CRasterizer::ApplyPolygonOffset(Register *pRegister) {
  ASSERT(pRegister);

  const float20 fSlope = Math::TCast<float20>(
      Math::TMax(Math::TAbs(pRegister->m[0]), Math::TAbs(pRegister->m[1])));

  const floatf fOffset = static_cast<floatf>(
      (m_polygonOffset.fFactor * fSlope) + m_polygonOffset.fUnits * fEPS);

  pRegister->m[2] = static_cast<fixedRX>(
      Math::TSat(Math::TCast<floatf>(pRegister->m[2]) + fOffset));
}

Register *CRasterizer::ColorGradient(Register *pRegister,
                                     const TriangleGradient &gradient,
                                     uint32_t i0, uint32_t i1, uint32_t i2) {
  ASSERT(pRegister);

  const RASTERFLAGS rasterFlags = m_rasterID.Flags;

  // Lookup vertex colors
  const ColorARGB *const pcColors = (const ColorARGB *)(m_pbVertexColor);
  const ColorARGB c0 = pcColors[i0];
  const ColorARGB c1 = pcColors[i1];
  const ColorARGB c2 = pcColors[i2];

  uint32_t interpolateMask = (rasterFlags.InterpolateColor ? 0x7 : 0) |
                             (rasterFlags.InterpolateAlpha << 3);
  if (interpolateMask) {
    for (uint32_t i = 0; i < 4; ++i) {
      const int delta[2] = {c1.m[i] - c0.m[i], c2.m[i] - c0.m[i]};
      pRegister[i].m[0] = gradient.TCalcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = gradient.TCalcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(c0.m[i]) >> fixed8::FRAC;
    }
    m_rasterID.Flags.InterpolateColor = (interpolateMask & 0x7) ? 1 : 0;
    m_rasterID.Flags.InterpolateAlpha = interpolateMask >> 3;
  } else {
    for (uint32_t i = 0; i < 4; ++i) {
      pRegister[i].m[0] = TConst<fixedRX>::Zero();
      pRegister[i].m[1] = TConst<fixedRX>::Zero();
      pRegister[i].m[2] = static_cast<fixedRX>(c2.m[i]) >> fixed8::FRAC;
    }
  }

  return pRegister + 4;
}

Register *CRasterizer::AffineTextureGradient(Register *pRegister,
                                             const TriangleGradient &gradient,
                                             uint32_t i0, uint32_t i1,
                                             uint32_t i2) {
  ASSERT(pRegister);

  const RASTERFLAGS rasterFlags = m_rasterID.Flags;
  const uint32_t numTextures = rasterFlags.NumTextures;

  for (uint32_t j = 0; j < numTextures; ++j) {
    ASSERT(j < MAX_TEXTURES);

    const TEXCOORD2 *const vTexCoords =
        (const TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + j];
    const TEXCOORD2 &uv0 = vTexCoords[i0];
    const TEXCOORD2 &uv1 = vTexCoords[i1];
    const TEXCOORD2 &uv2 = vTexCoords[i2];

    for (uint32_t i = 0; i < 2; ++i) {
      const float20 delta[2] = {uv1.m[i] - uv0.m[i], uv2.m[i] - uv0.m[i]};

      pRegister[i].m[0] = gradient.TCalcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = gradient.TCalcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(uv0.m[i]);
    }

    pRegister += 2;
  }

  if (rasterFlags.TextureMips) {
    pRegister = this->AffineTextureMipmapGradient(pRegister);
  }

  m_rasterID.Flags.Perspective = 0;

  return pRegister;
}

Register *
CRasterizer::PerspectiveTextureGradient(Register *pRegister,
                                        const TriangleGradient &gradient,
                                        uint32_t i0, uint32_t i1, uint32_t i2) {
  ASSERT(pRegister);

  const RASTERFLAGS rasterFlags = m_rasterID.Flags;
  const uint32_t numTextures = rasterFlags.NumTextures;

  // Lookup vertex screen positions
  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const RDVECTOR &v0 = pvScreenPos[i0];
  const RDVECTOR &v1 = pvScreenPos[i1];
  const RDVECTOR &v2 = pvScreenPos[i2];

  floatRW rhws[3] = {v0.rhw, v1.rhw, v2.rhw};

  // Scale up rhw to avoid fixed point overflow
  const floatRW fSatW = TConst<floatRW>::One() / 16;
  while ((rhws[0] < fSatW) && (rhws[1] < fSatW) && (rhws[2] < fSatW)) {
    rhws[0] *= 16;
    rhws[1] *= 16;
    rhws[2] *= 16;
  }

  const floatRW delta[2] = {rhws[1] - rhws[0], rhws[2] - rhws[0]};

  pRegister->m[0] = gradient.TCalcDeltaX<fixedRX>(delta[0], delta[1]);
  pRegister->m[1] = gradient.TCalcDeltaY<fixedRX>(delta[0], delta[1]);
  pRegister->m[2] = static_cast<fixedRX>(rhws[0]);

  ++pRegister;

  for (uint32_t j = 0; j < numTextures; ++j) {
    const TEXCOORD2 *const vTexCoords =
        (const TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + j];
    const TEXCOORD2 &uv0 = vTexCoords[i0];
    const TEXCOORD2 &uv1 = vTexCoords[i1];
    const TEXCOORD2 &uv2 = vTexCoords[i2];

    for (uint32_t i = 0; i < 2; ++i) {
      const float20 uvw0 = uv0.m[i] * rhws[0];
      const float20 uvw1 = uv1.m[i] * rhws[1];
      const float20 uvw2 = uv2.m[i] * rhws[2];

      const float20 delta[2] = {uvw1 - uvw0, uvw2 - uvw0};

      pRegister[i].m[0] = gradient.TCalcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = gradient.TCalcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(uvw0);
    }

    pRegister += 2;
  }

  if (rasterFlags.TextureMips) {
    pRegister = this->PerspectiveTextureMipmapGradient(pRegister, gradient, v0,
                                                       v1, v2, rhws);
  }

  m_rasterID.Flags.Perspective = 1;

  return pRegister;
}

Register *CRasterizer::AffineTextureMipmapGradient(Register *pRegister) {
  const RASTERFLAGS rasterFlags = m_rasterID.Flags;
  const uint32_t numTextures = rasterFlags.NumTextures;
  const uint32_t textureMips = rasterFlags.TextureMips;
  const Register *pCur = pRegister - (2 * numTextures);

  for (uint32_t j = 0; j < numTextures; ++j, pCur += 2) {
    ASSERT(j < MAX_TEXTURES);

    if ((textureMips >> j) & 0x1) {
      float20 fMaxDm[2];

      for (uint32_t i = 0; i < 2; ++i) {
        fMaxDm[i] = Math::TCast<float20>(
            Math::TMax(Math::TAbs(pCur[i].m[0]), Math::TAbs(pCur[i].m[1])));
      }

      Sampler &sampler = m_rasterData.Samplers[j];

      const uint32_t logWidth = sampler.pMipLevels[0].GetLogWidth();
      const uint32_t logHeight = sampler.pMipLevels[0].GetLogHeight();
      const floatf fMU = Math::TMul<floatf>(fMaxDm[0], 1 << logWidth);
      const floatf fMV = Math::TMul<floatf>(fMaxDm[1], 1 << logHeight);
      floatf fM = Math::TMax(fMU, fMV);

      if (fM > fONE) {
        if (FILTER_NONE == m_rasterID.Textures[j].MipFilter) {
          // Turn off trilinear filtering
          m_rasterID.Textures[j].MipFilter = FILTER_NONE;
          m_rasterID.Textures[j].MagFilter = m_rasterID.Textures[j].MinFilter;
          m_rasterID.Flags.TextureMips &= ~(1 << j);
        } else {
          const fixed16 fJ = static_cast<fixed16>(fM);
          const uint32_t mipLevel = Math::iLog2(fJ.GetRaw()) - fixed16::FRAC;
          if (mipLevel >= sampler.MaxMipLevel) {
            // Use nearest mipmap filtering
            m_rasterID.Textures[j].MipFilter = FILTER_NEAREST;
            m_rasterID.Textures[j].MagFilter = m_rasterID.Textures[j].MinFilter;
            fM = static_cast<floatf>(1 << sampler.MaxMipLevel);
          }

          pRegister->m[2] = fixedRX::Make(static_cast<fixed16>(fM).GetRaw());
          pRegister->m[0] = TConst<fixedRX>::Zero();
          pRegister->m[1] = TConst<fixedRX>::Zero();
          ++pRegister;
        }
      } else {
        // Turn off trilinear filtering
        m_rasterID.Textures[j].MipFilter = FILTER_NONE;
        m_rasterID.Textures[j].MinFilter = m_rasterID.Textures[j].MagFilter;
        m_rasterID.Flags.TextureMips &= ~(1 << j);
      }
    }
  }

  return pRegister;
}

Register *CRasterizer::PerspectiveTextureMipmapGradient(
    Register *pRegister, const TriangleGradient &gradient, const RDVECTOR &v0,
    const RDVECTOR &v1, const RDVECTOR &v2, floatRW rhws[3]) {
  ASSERT(pRegister);

  const RASTERFLAGS rasterFlags = m_rasterID.Flags;
  const uint32_t textureMips = rasterFlags.TextureMips;
  const uint32_t numTextures = rasterFlags.NumTextures;

  floatf fRhw2s[3];

  for (uint32_t k = 0; k < 3; ++k) {
    fRhw2s[k] = Math::TMul<floatf>(rhws[k], rhws[k]);
  }

  const Register *pCur = pRegister - 2 * numTextures - 1;

  const float20 fRhwdA = Math::TCast<float20>(pCur->m[0]);
  const float20 fRhwdB = Math::TCast<float20>(pCur->m[1]);
  const float20 fRhwdC = Math::TCast<float20>(pCur->m[2]);

  ++pCur;

  const RDVECTOR *vScreenPos[3] = {&v0, &v1, &v2};

  for (uint32_t j = 0; j < numTextures; ++j, pCur += 2) {
    if ((textureMips >> j) & 0x1) {
      float20 fMaxDms[3][2];

      for (uint32_t i = 0; i < 2; ++i) {
        const float20 fA = Math::TCast<float20>(pCur[i].m[0]);
        const float20 fB = Math::TCast<float20>(pCur[i].m[1]);
        const float20 fC = Math::TCast<float20>(pCur[i].m[2]);

        const float24 fK1 = Math::TMulSub<float24>(fA, fRhwdB, fB, fRhwdA);
        const float20 fK2 = Math::TMulSub<float20>(fA, fRhwdC, fRhwdA, fC);
        const float20 fK3 = Math::TMulSub<float20>(fB, fRhwdC, fRhwdB, fC);

        for (uint32_t k = 0; k < 3; ++k) {
          const float20 fK1x = Math::TMul<float20>(fK1, vScreenPos[k]->x);
          const float20 fK1y = Math::TMul<float20>(fK1, vScreenPos[k]->y);
          fMaxDms[k][i] =
              Math::TMax(Math::TAbs(fK2 + fK1y), Math::TAbs(fK3 - fK1x));
        }
      }

      const Sampler &sampler = m_rasterData.Samplers[j];
      const uint32_t logWidth = sampler.pMipLevels[0].GetLogWidth();
      const uint32_t logHeight = sampler.pMipLevels[0].GetLogHeight();

      floatf fMs[3];

      for (uint32_t k = 0; k < 3; ++k) {
        const floatf fMU = Math::TShiftLeft<floatf>(fMaxDms[k][0], logWidth);
        const floatf fMV = Math::TShiftLeft<floatf>(fMaxDms[k][1], logHeight);
        fMs[k] = Math::TMax(fMU, fMV);
      }

      if ((fMs[0] > fRhw2s[0]) || (fMs[1] > fRhw2s[1]) ||
          (fMs[2] > fRhw2s[2])) {
        if ((Math::TShiftRight<floatf>(fMs[0], sampler.MaxMipLevel) <
             fRhw2s[0]) ||
            (Math::TShiftRight<floatf>(fMs[1], sampler.MaxMipLevel) <
             fRhw2s[1]) ||
            (Math::TShiftRight<floatf>(fMs[2], sampler.MaxMipLevel) <
             fRhw2s[2])) {
          const floatf delta[2] = {fMs[1] - fMs[0], fMs[2] - fMs[0]};

          if ((fixed16(Math::TAbs(delta[0])) > TConst<fixed16>::Epsilon()) ||
              (fixed16(Math::TAbs(delta[1])) > TConst<fixed16>::Epsilon())) {
            m_rasterID.Flags.InterpolateMips |= (1 << j);
            pRegister->m[2] =
                fixedRX::Make(static_cast<fixed16>(fMs[0]).GetRaw());
            pRegister->m[0] = fixedRX::Make(
                gradient.TCalcDeltaX<fixed16>(delta[0], delta[1]).GetRaw());
            pRegister->m[1] = fixedRX::Make(
                gradient.TCalcDeltaY<fixed16>(delta[0], delta[1]).GetRaw());
            ++pRegister;
          } else {
            if (fMs[0] > fRhw2s[0]) {
              if (FILTER_NONE == m_rasterID.Textures[j].MipFilter) {
                // Turn off trilinear filtering
                m_rasterID.Textures[j].MipFilter = FILTER_NONE;
                m_rasterID.Textures[j].MagFilter =
                    m_rasterID.Textures[j].MinFilter;
                m_rasterID.Flags.TextureMips &= ~(1 << j);
              } else {
                floatf fM = fMs[0] / fRhw2s[0];
                const fixed16 fJ = static_cast<fixed16>(fM);
                const uint32_t mipLevel =
                    Math::iLog2(fJ.GetRaw()) - fixed16::FRAC;

                if (mipLevel >= sampler.MaxMipLevel) {
                  // Use nearest mipmap filtering
                  m_rasterID.Textures[j].MipFilter = FILTER_NEAREST;
                  m_rasterID.Textures[j].MagFilter =
                      m_rasterID.Textures[j].MinFilter;
                  fM = static_cast<floatf>(1 << sampler.MaxMipLevel);
                }

                pRegister->m[2] =
                    fixedRX::Make(static_cast<fixed16>(fM).GetRaw());
                pRegister->m[0] = TConst<fixedRX>::Zero();
                pRegister->m[1] = TConst<fixedRX>::Zero();
                ++pRegister;
              }
            } else {
              // Turn off trilinear filtering
              m_rasterID.Textures[j].MipFilter = FILTER_NONE;
              m_rasterID.Textures[j].MinFilter =
                  m_rasterID.Textures[j].MagFilter;
              m_rasterID.Flags.TextureMips &= ~(1 << j);
            }
          }
        } else {
          if (FILTER_NONE == m_rasterID.Textures[j].MipFilter) {
            // Turn off trilinear filtering
            m_rasterID.Textures[j].MagFilter = m_rasterID.Textures[j].MinFilter;
            m_rasterID.Flags.TextureMips &= ~(1 << j);
          } else {
            // Use nearest mipmap filtering
            m_rasterID.Textures[j].MipFilter = FILTER_NEAREST;
            m_rasterID.Textures[j].MagFilter = m_rasterID.Textures[j].MinFilter;
            pRegister->m[2] =
                fixedRX::Make(fixed16(1 << sampler.MaxMipLevel).GetRaw());
            pRegister->m[0] = TConst<fixedRX>::Zero();
            pRegister->m[1] = TConst<fixedRX>::Zero();
            ++pRegister;
          }
        }
      } else {
        // Turn off trilinear filtering
        m_rasterID.Textures[j].MipFilter = FILTER_NONE;
        m_rasterID.Textures[j].MinFilter = m_rasterID.Textures[j].MagFilter;
        m_rasterID.Flags.TextureMips &= ~(1 << j);
      }
    }
  }

  return pRegister;
}

Register *CRasterizer::FogGradient(Register *pRegister,
                                   const TriangleGradient &gradient,
                                   uint32_t i0, uint32_t i1, uint32_t i2) {
  ASSERT(pRegister);

  const float20 *const pfFogs = (const float20 *)m_pbVertexData[VERTEXDATA_FOG];
  const float20 fFog0 = pfFogs[i0];
  const float20 fFog1 = pfFogs[i1];
  const float20 fFog2 = pfFogs[i2];

  const float20 delta[2] = {fFog1 - fFog0, fFog2 - fFog0};

  if ((fixed8(Math::TAbs(delta[0])) > TConst<fixed8>::Epsilon()) ||
      (fixed8(Math::TAbs(delta[1])) > TConst<fixed8>::Epsilon())) {
    pRegister->m[0] = gradient.TCalcDeltaX<fixedRX>(delta[0], delta[1]);
    pRegister->m[1] = gradient.TCalcDeltaY<fixedRX>(delta[0], delta[1]);
    m_rasterID.Flags.InterpolateFog = 1;
  } else {
    pRegister->m[0] = TConst<fixedRX>::Zero();
    pRegister->m[1] = TConst<fixedRX>::Zero();
  }

  pRegister->m[2] = static_cast<fixedRX>(fFog0);

  return pRegister + 1;
}