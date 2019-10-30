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

class CBuffer;
struct VertexDecoder;

struct VertexArray {
  eVertexFormat Format;
  unsigned Stride;
  const void *pPointer;
  CBuffer *pBuffer;

  VertexArray()
      : Format(VERTEX_UNKNOWN), Stride(0), pPointer(NULL), pBuffer(NULL) {}

  ~VertexArray() {}

  uint32_t GetBufferHandle() const;

  GLenum Prepare(VertexDecoder *pDecoder, int first, unsigned count);
};

struct VertexDecoder {
  const uint8_t *pBits;
  unsigned Stride;
};