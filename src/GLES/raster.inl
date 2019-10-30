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
#pragma once

template <eClipFlags ClipPlane>
inline unsigned CRasterizer::TClipTriangle(unsigned nNumVertices,
                                           unsigned *pSrc, unsigned *pDst,
                                           unsigned *pTmp) {
  const VECTOR4 *const pvClipPos =
      (const VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];

  const int coord = ClipPlane >> 1;
  static constexpr bool Signed = (ClipPlane & 1) != 0;

  unsigned iTmp = *pTmp;
  unsigned iVB = pSrc[nNumVertices - 1];
  unsigned iVA;

  unsigned nClipVertices = 0;

  if
    constexpr(Signed) {
      for (floatf fDistA, fDistB = pvClipPos[iVB].w - pvClipPos[iVB].m[coord];
           nNumVertices--; iVB = iVA, fDistB = fDistA) {
        iVA = *pSrc++;
        fDistA = pvClipPos[iVA].w - pvClipPos[iVA].m[coord];

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
    }
  else {
    for (floatf fDistA, fDistB = pvClipPos[iVB].w + pvClipPos[iVB].m[coord];
         nNumVertices--; iVB = iVA, fDistB = fDistA) {
      iVA = *pSrc++;
      fDistA = pvClipPos[iVA].w + pvClipPos[iVA].m[coord];

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
  }

  *pTmp = iTmp;

  return nClipVertices;
}

template <class T>
inline GLenum
CRasterizer::TRenderIndexedPrimitive(GLenum mode, const T *pIndices,
                                     unsigned count, unsigned startVertex) {
  ASSERT(pIndices);

  switch (mode) {
  case GL_TRIANGLES:
    for (const T *const pEnd = pIndices + count - 2; pIndices < pEnd;) {
      const unsigned i0 = *pIndices++ - startVertex;
      const unsigned i1 = *pIndices++ - startVertex;
      const unsigned i2 = *pIndices++ - startVertex;
      this->DrawTriangle(i0, i1, i2);
    }
    break;

  case GL_TRIANGLE_STRIP:
    if (count > 2) {
      const T *const pEnd = pIndices + count;
      unsigned i0 = *pIndices++ - startVertex;
      unsigned i1 = *pIndices++ - startVertex;

      for (const T *const pEnd2 = pEnd - 1; pIndices < pEnd2;) {
        const unsigned i2 = *pIndices++ - startVertex;
        this->DrawTriangle(i0, i1, i2);

        const unsigned i3 = *pIndices++ - startVertex;
        this->DrawTriangle(i2, i1, i3);

        i0 = i2;
        i1 = i3;
      }

      if (pIndices < pEnd) {
        const unsigned i2 = *pIndices++ - startVertex;
        this->DrawTriangle(i0, i1, i2);
      }
    }
    break;

  case GL_TRIANGLE_FAN:
    if (count > 2) {
      const T *const pEnd = pIndices + count;
      const unsigned is = *pIndices++ - startVertex;
      unsigned i0 = *pIndices++;

      for (; pIndices < pEnd;) {
        const unsigned i1 = *pIndices++ - startVertex;
        this->DrawTriangle(is, i0, i1);
        i0 = i1;
      }
    }
    break;

  case GL_LINES:
    for (const T *const pEnd = pIndices + count - 1; pIndices < pEnd;) {
      const unsigned i0 = *pIndices++ - startVertex;
      const unsigned i1 = *pIndices++ - startVertex;
      this->DrawLine(i0, i1);
    }
    break;

  case GL_LINE_STRIP:
    if (count > 1) {
      const T *const pEnd = pIndices + count;
      unsigned i0 = *pIndices++ - startVertex;

      for (; pIndices < pEnd;) {
        const unsigned i1 = *pIndices++ - startVertex;
        this->DrawLine(i0, i1);
        i0 = i1;
      }
    }
    break;

  case GL_LINE_LOOP:
    if (count > 1) {
      const T *const pEnd = pIndices + count;
      const unsigned is = *pIndices++ - startVertex;
      unsigned i0 = is;

      for (; pIndices < pEnd;) {
        const unsigned i1 = *pIndices++ - startVertex;
        this->DrawLine(i0, i1);
        i0 = i1;
      }

      this->DrawLine(i0, is);
    }
    break;

  case GL_POINTS:
    for (const T *const pEnd = pIndices + count; pIndices < pEnd;) {
      const unsigned i0 = *pIndices++ - startVertex;
      this->DrawPoint(i0);
    }
    break;

  default:
    __glLogError(_T("CGLContext::RenderPrimitive() failed, invalid mode ")
                 _T("parameter: %d.\r\n"),
                 mode);
    return GL_INVALID_ENUM;
  }

  return GL_NO_ERROR;
}