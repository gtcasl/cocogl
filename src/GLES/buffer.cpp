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

CBuffer::CBuffer() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  m_dwHandle = 0;
  m_pBits = NULL;
  m_size = 0;
  m_usage = GL_STATIC_DRAW;
}

CBuffer::~CBuffer() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  __safeDeleteArray(m_pBits);
}

GLenum CBuffer::Create(CBuffer **ppBuffer) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  ASSERT(ppBuffer);

  // Create a new surface object
  CBuffer *pBuffer = new CBuffer();
  if (NULL == pBuffer) {
    __glLogError(_T("CBuffer allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  pBuffer->AddRef();
  *ppBuffer = pBuffer;

  return GL_NO_ERROR;
}

GLenum CBuffer::Initialize(unsigned size, GLenum usage, const GLvoid *pData) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  uint8_t *const pBits = new uint8_t[size];
  if (NULL == pBits) {
    __glLogError(_T("CBuffer storage allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  __safeDeleteArray(m_pBits);

  m_pBits = pBits;
  m_size = size;
  m_usage = usage;

  if (pData) {
    this->CopyData(0, size, pData);
  }

  return GL_NO_ERROR;
}

GLenum CBuffer::GetParameter(GLenum pname, GLint *pParams) {
  GLint params;

  switch (pname) {
  case GL_BUFFER_SIZE:
    params = m_size;
    break;

  case GL_BUFFER_USAGE:
    params = m_usage;
    break;

  default:
    __glLogError(
        _T("CBuffer::GetParameter() failed, invalid pname parameter: %d.\r\n"),
        pname);
    return GL_INVALID_ENUM;
  }

  if (pParams) {
    *pParams = params;
  }

  return GL_NO_ERROR;
}
