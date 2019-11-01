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
                                       uint32_t count, uint32_t *pMin,
                                       uint32_t *pMax) {
  ASSERT(ppvIndices);

  const GLvoid *pvIndices = *ppvIndices;
  CBuffer *const pBufElements = this->GetBufferObject(GL_ELEMENT_ARRAY_BUFFER);
  ASSERT(pBufElements);

  const uint8_t *const pBits = pBufElements->GetBits();
  if (pBits) {
    const uint32_t stride = GLSizeOf(type);
    const uint32_t size = pBufElements->GetSize();
    const uint32_t offset = static_cast<const uint8_t *>(pvIndices) -
                            static_cast<const uint8_t *>(nullptr);
    if ((offset + stride * count) <= size) {
      pvIndices = pBits + offset;
    } else {
      __glLogError(_T("The element array buffer is out of range.\r\n"));
      return GL_INVALID_OPERATION;
    }
  } else {
    if (nullptr == pvIndices) {
      __glLogError(_T("The indices buffer is nullptr.\r\n"));
      return GL_INVALID_OPERATION;
    }
  }

  uint32_t min = UINT_MAX;
  uint32_t max = 0;

  switch (type) {
  case GL_UNSIGNED_SHORT: {
    const uint16_t *const pwIndices =
        reinterpret_cast<const uint16_t *>(pvIndices);
    for (uint32_t i = 0; i < count; ++i) {
      const uint32_t value = pwIndices[i];
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
    const uint8_t *const pbIndices =
        reinterpret_cast<const uint8_t *>(pvIndices);
    for (uint32_t i = 0; i < count; ++i) {
      const uint32_t value = pbIndices[i];
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