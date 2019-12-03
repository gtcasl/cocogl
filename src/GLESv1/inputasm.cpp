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

GLenum InputAssembler::prepareIndices(GLenum type, const GLvoid **ppvIndices,
                                      uint32_t count, uint32_t *pMin,
                                      uint32_t *pMax) {
  assert(ppvIndices);

  const GLvoid *pvIndices = *ppvIndices;
  auto pBufElements = this->getBufferObject(GL_ELEMENT_ARRAY_BUFFER);
  assert(pBufElements);

  auto pBits = pBufElements->getBits();
  if (pBits) {
    uint32_t stride = GLSizeOf(type);
    uint32_t size = pBufElements->getSize();
    auto offset = static_cast<const uint8_t *>(pvIndices) -
                  static_cast<const uint8_t *>(nullptr);
    if ((offset + stride * count) <= size) {
      pvIndices = pBits + offset;
    } else {
      __glLogError("The element array buffer is out of range.\r\n");
      return GL_INVALID_OPERATION;
    }
  } else {
    if (nullptr == pvIndices) {
      __glLogError("The indices buffer is nullptr.\r\n");
      return GL_INVALID_OPERATION;
    }
  }

  uint32_t min = UINT_MAX;
  uint32_t max = 0;

  switch (type) {
  case GL_UNSIGNED_SHORT: {
    auto pwIndices = reinterpret_cast<const uint16_t *>(pvIndices);
    for (uint32_t i = 0; i < count; ++i) {
      uint32_t value = pwIndices[i];
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
    auto pbIndices = reinterpret_cast<const uint8_t *>(pvIndices);
    for (uint32_t i = 0; i < count; ++i) {
      uint32_t value = pbIndices[i];
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
    __glLogError("GLContext::prepareIndices() failed, invalid type "
                 "parameter: %d.\r\n",
                 type);
    return GL_INVALID_ENUM;
  }

  *ppvIndices = pvIndices;
  *pMin = min;
  *pMax = max;

  return GL_NO_ERROR;
}