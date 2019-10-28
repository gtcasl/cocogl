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
#include "inputasm.hpp"
#include "buffer.hpp"
#include "vertarray.hpp"


GLenum CInputAssembler::PrepareIndices(GLenum type, const GLvoid **ppvIndices,
                                       unsigned count, unsigned *pMin,
                                       unsigned *pMax) {
  ASSERT(ppvIndices);

  const GLvoid *pvIndices = *ppvIndices;
  CBuffer *const pBufElements = this->GetBufferObject(GL_ELEMENT_ARRAY_BUFFER);
  ASSERT(pBufElements);

  const uint8_t *const pBits = pBufElements->GetBits();
  if (pBits) {
    const unsigned stride = GLSizeOf(type);
    const unsigned size = pBufElements->GetSize();
    const unsigned offset =
        static_cast<const uint8_t *>(pvIndices) - static_cast<const uint8_t *>(NULL);
    if ((offset + stride * count) <= size) {
      pvIndices = pBits + offset;
    } else {
      __glLogError(_T("The element array buffer is out of range.\r\n"));
      return GL_INVALID_OPERATION;
    }
  } else {
    if (NULL == pvIndices) {
      __glLogError(_T("The indices buffer is NULL.\r\n"));
      return GL_INVALID_OPERATION;
    }
  }

  unsigned min = UINT_MAX;
  unsigned max = 0;

  switch (type) {
  case GL_UNSIGNED_SHORT: {
    const uint16_t *const pwIndices = reinterpret_cast<const uint16_t *>(pvIndices);
    for (unsigned i = 0; i < count; ++i) {
      const unsigned value = pwIndices[i];
      if (value < min) {
        min = value;
      }

      if (value > max) {
        max = value;
      }
    }
  }

  break;

  case GL_UNSIGNED_BYTE: {
    const uint8_t *const pbIndices = reinterpret_cast<const uint8_t *>(pvIndices);
    for (unsigned i = 0; i < count; ++i) {
      const unsigned value = pbIndices[i];
      if (value < min) {
        min = value;
      }

      if (value > max) {
        max = value;
      }
    }
  }

  break;

  default:
    __glLogError(_T("CGLContext::PrepareIndices() failed, invalid type ")
                 _T("parameter: %d.\r\n"),
                 type);
    return GL_INVALID_ENUM;
  }

  *ppvIndices = pvIndices;
  *pMin = min;
  *pMax = max;

  return GL_NO_ERROR;
}