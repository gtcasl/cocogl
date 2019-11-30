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
inline uint32_t CRasterizer::TClipTriangle(uint32_t nNumVertices,
                                           uint32_t *pSrc, uint32_t *pDst,
                                           uint32_t *pTmp) {
  auto pvClipPos =
      reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEXDATA_CLIPPOS]);

  int coord = ClipPlane >> 1;
  static constexpr bool Signed = (ClipPlane & 1) != 0;

  uint32_t iTmp = *pTmp;
  uint32_t iVB = pSrc[nNumVertices - 1];
  uint32_t iVA;

  uint32_t nClipVertices = 0;

  if constexpr (Signed) {
    for (floatf fDistA, fDistB = pvClipPos[iVB].w - pvClipPos[iVB].m[coord];
         nNumVertices--; iVB = iVA, fDistB = fDistA) {
      iVA = *pSrc++;
      fDistA = pvClipPos[iVA].w - pvClipPos[iVA].m[coord];

      if (fDistB >= fZERO) {
        // Add vertex to the current list
        assert(nClipVertices < CLIP_BUFFER_SIZE);
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
      assert(nClipVertices < CLIP_BUFFER_SIZE);
      pDst[nClipVertices++] = iTmp++;
    }
  } else {
    for (floatf fDistA, fDistB = pvClipPos[iVB].w + pvClipPos[iVB].m[coord];
         nNumVertices--; iVB = iVA, fDistB = fDistA) {
      iVA = *pSrc++;
      fDistA = pvClipPos[iVA].w + pvClipPos[iVA].m[coord];

      if (fDistB >= fZERO) {
        // Add vertex to the current list
        assert(nClipVertices < CLIP_BUFFER_SIZE);
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
      assert(nClipVertices < CLIP_BUFFER_SIZE);
      pDst[nClipVertices++] = iTmp++;
    }
  }

  *pTmp = iTmp;

  return nClipVertices;
}

template <class T>
inline GLenum
CRasterizer::TRenderIndexedPrimitive(GLenum mode, const T *pIndices,
                                     uint32_t count, uint32_t startVertex) {
  assert(pIndices);

  switch (mode) {
  case GL_TRIANGLES:
    for (auto pEnd = pIndices + count - 2; pIndices < pEnd;) {
      uint32_t i0 = *pIndices++ - startVertex;
      uint32_t i1 = *pIndices++ - startVertex;
      uint32_t i2 = *pIndices++ - startVertex;
      this->DrawTriangle(i0, i1, i2);
    }
    break;

  case GL_TRIANGLE_STRIP:
    if (count > 2) {
      auto pEnd = pIndices + count;
      uint32_t i0 = *pIndices++ - startVertex;
      uint32_t i1 = *pIndices++ - startVertex;

      for (auto pEnd2 = pEnd - 1; pIndices < pEnd2;) {
        uint32_t i2 = *pIndices++ - startVertex;
        this->DrawTriangle(i0, i1, i2);

        uint32_t i3 = *pIndices++ - startVertex;
        this->DrawTriangle(i2, i1, i3);

        i0 = i2;
        i1 = i3;
      }

      if (pIndices < pEnd) {
        uint32_t i2 = *pIndices++ - startVertex;
        this->DrawTriangle(i0, i1, i2);
      }
    }
    break;

  case GL_TRIANGLE_FAN:
    if (count > 2) {
      auto pEnd = pIndices + count;
      uint32_t is = *pIndices++ - startVertex;
      uint32_t i0 = *pIndices++;

      for (; pIndices < pEnd;) {
        uint32_t i1 = *pIndices++ - startVertex;
        this->DrawTriangle(is, i0, i1);
        i0 = i1;
      }
    }
    break;

  case GL_LINES:
    for (auto pEnd = pIndices + count - 1; pIndices < pEnd;) {
      uint32_t i0 = *pIndices++ - startVertex;
      uint32_t i1 = *pIndices++ - startVertex;
      this->DrawLine(i0, i1);
    }
    break;

  case GL_LINE_STRIP:
    if (count > 1) {
      auto pEnd = pIndices + count;
      uint32_t i0 = *pIndices++ - startVertex;

      for (; pIndices < pEnd;) {
        uint32_t i1 = *pIndices++ - startVertex;
        this->DrawLine(i0, i1);
        i0 = i1;
      }
    }
    break;

  case GL_LINE_LOOP:
    if (count > 1) {
      auto pEnd = pIndices + count;
      uint32_t is = *pIndices++ - startVertex;
      uint32_t i0 = is;

      for (; pIndices < pEnd;) {
        uint32_t i1 = *pIndices++ - startVertex;
        this->DrawLine(i0, i1);
        i0 = i1;
      }

      this->DrawLine(i0, is);
    }
    break;

  case GL_POINTS:
    for (auto pEnd = pIndices + count; pIndices < pEnd;) {
      uint32_t i0 = *pIndices++ - startVertex;
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