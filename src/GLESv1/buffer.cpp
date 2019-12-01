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
#include "buffer.hpp"

GLBuffer::GLBuffer() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  handle_ = 0;
  pBits_ = nullptr;
  size_ = 0;
  usage_ = GL_STATIC_DRAW;
}

GLBuffer::~GLBuffer() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeDeleteArray(pBits_);
}

GLenum GLBuffer::Create(GLBuffer **ppBuffer) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(ppBuffer);

  // Create a new surface object
  auto pBuffer = new GLBuffer();
  if (nullptr == pBuffer) {
    __glLogError("GLBuffer allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  pBuffer->addRef();
  *ppBuffer = pBuffer;

  return GL_NO_ERROR;
}

GLenum GLBuffer::initialize(uint32_t size, GLenum usage, const GLvoid *pData) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pBits = new uint8_t[size];
  if (nullptr == pBits) {
    __glLogError("GLBuffer storage allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  __safeDeleteArray(pBits_);

  pBits_ = pBits;
  size_ = size;
  usage_ = usage;

  if (pData) {
    this->copyData(0, size, pData);
  }

  return GL_NO_ERROR;
}

GLenum GLBuffer::getParameter(GLenum pname, GLint *pParams) {
  GLint params;

  switch (pname) {
  case GL_BUFFER_SIZE:
    params = size_;
    break;

  case GL_BUFFER_USAGE:
    params = usage_;
    break;

  default:
    __glLogError(
        "GLBuffer::getParameter() failed, invalid pname parameter: %d.\r\n",
        pname);
    return GL_INVALID_ENUM;
  }

  if (pParams) {
    *pParams = params;
  }

  return GL_NO_ERROR;
}
