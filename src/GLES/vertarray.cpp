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

uint32_t VertexArray::GetBufferHandle() const {
  return (pBuffer ? pBuffer->GetHandle() : static_cast<uint32_t>(HANDLE_NONE));
}

GLenum VertexArray::Prepare(VertexDecoder *pDecoder, int first,
                            unsigned count) {
  ASSERT(pDecoder);

  int offset = first * this->Stride;
  const uint8_t *const pBits = this->pBuffer->GetBits();
  if (pBits) {
    pDecoder->pBits = pBits;
    offset += reinterpret_cast<uintptr_t>(this->pPointer);
    const unsigned dataSize = VertexDataSize(this->Format);
    const int padding = this->Stride - offset - dataSize;
    const unsigned bufSize = count * this->Stride - padding;
    if (bufSize <= this->pBuffer->GetSize()) {
      pDecoder->pBits = pBits;
    } else {
      __glLogError(
          _T("VertexArray::Prepare() failed, index out of range.\r\n"));
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
