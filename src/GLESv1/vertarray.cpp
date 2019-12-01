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
#include "vertarray.hpp"
#include "buffer.hpp"

uint32_t VertexArray::getBufferHandle() const {
  return (pBuffer ? pBuffer->getHandle() : static_cast<uint32_t>(HANDLE_NONE));
}

GLenum VertexArray::prepare(VertexDecoder *pDecoder, int first,
                            uint32_t count) {
  assert(pDecoder);

  int offset = first * this->Stride;
  auto pBits = this->pBuffer->getBits();
  if (pBits) {
    pDecoder->pBits = pBits;
    offset += reinterpret_cast<uintptr_t>(this->pPointer);
    uint32_t dataSize = VertexDataSize(this->Format);
    int padding = this->Stride - offset - dataSize;
    uint32_t bufSize = count * this->Stride - padding;
    if (bufSize <= this->pBuffer->getSize()) {
      pDecoder->pBits = pBits;
    } else {
      __glLogError(
          "VertexArray::prepare() failed, index out of range.\r\n");
      return GL_INVALID_OPERATION;
    }
  } else {
    pDecoder->pBits = reinterpret_cast<const uint8_t *>(this->pPointer);
  }

  if (pDecoder->pBits) {
    pDecoder->pBits += offset;
  }

  pDecoder->Stride = this->Stride;

  return GL_NO_ERROR;
}
