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
#include "context.hpp"

void CGLContext::GenBuffers(GLsizei n, GLuint *pBuffers) {
  GLenum err;

  ASSERT(pBuffers);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::GenBuffers() failed, invalid n parameter: %d.\r\n"), n);
    return;
  }

  for (GLuint *const pEnd = pBuffers + n; pBuffers != pEnd; ++pBuffers) {
    CBuffer *pBuffer;
    err = CBuffer::Create(&pBuffer);
    if (__glFailed(err)) {
      __glError(err, _T("CBuffer::Create() failed, err = %d.\r\n"), err);
      return;
    }

    uint32_t dwHandle;
    err = GLERROR_FROM_HRESULT(
        m_pHandles->Insert(&dwHandle, pBuffer, HANDLE_BUFFER, this));
    if (__glFailed(err)) {
      __safeRelease(pBuffer);
      __glError(err, _T("CHandleTable::Insert() failed, err = %d.\r\n"), err);
      return;
    }

    pBuffer->SetHandle(dwHandle);

    *pBuffers = dwHandle;
  }
}


void CGLContext::BindBuffer(GLenum target, GLuint buffer) {
  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::BindBuffer() failed, invalid ")
                               _T("target parameter: %d.\r\n"),
              target);
    return;
  }

  CBuffer *pBuffer = NULL;
  if (buffer) {
    // First, lookup buffers from the current context
    pBuffer = reinterpret_cast<CBuffer *>(m_pHandles->GetObject(buffer, this));
    if (NULL == pBuffer) {
      if (m_pCtxShared) {
        // Else, lookup buffers from the shared context
        pBuffer = reinterpret_cast<CBuffer *>(
            m_pHandles->GetObject(buffer, m_pCtxShared));
      }

      if (NULL == pBuffer) {
        __glError(GL_INVALID_VALUE, _T("CGLContext::BindBuffer() failed, ")
                                    _T("Invalid buffer parameter: %d\r\n"),
                  buffer);
        return;
      }
    }
  } else {
    pBuffer = m_pBufDefault;
  }

  this->SetBufferObject(target, pBuffer);
}


void CGLContext::BufferData(GLenum target, GLsizeiptr size, const GLvoid *pData,
                            GLenum usage) {
  GLenum err;

  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::BufferData() failed, invalid ")
                               _T("target parameter: %d.\r\n"),
              target);
    return;
  }

  if ((usage != GL_STATIC_DRAW) && (usage != GL_DYNAMIC_DRAW)) {
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::BufferData() failed, invalid usage parameter: %d.\r\n"),
        usage);
    return;
  }

  if (size < 0) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::BufferData() failed, invalid size parameter: %d.\r\n"),
        size);
    return;
  }

  CBuffer *const pBuffer = this->GetBufferObject(target);
  ASSERT(pBuffer);

  err = pBuffer->Initialize(size, usage, pData);
  if (__glFailed(err)) {
    __glError(err, _T("CBuffer::SetData() failed, err = %d.\r\n"), err);
    return;
  }
}


void CGLContext::BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                               const GLvoid *pData) {
  ASSERT(pData);

  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::BufferSubData() failed, ")
                               _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  if ((offset < 0) || (size < 0)) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::BufferSubData() failed, ")
                                _T("invalid size(%d) or offset(%d) ")
                                _T("parameters.\r\n"),
              size, offset);
    return;
  }

  CBuffer *const pBuffer = this->GetBufferObject(target);
  ASSERT(pBuffer);

  if (unsigned(offset + size) > pBuffer->GetSize()) {
    __glError(GL_INVALID_VALUE, _T("CGLContext::BufferSubData() failed, size ")
                                _T("+ offset parameters out of range: %d.\r\n"),
              size + offset);
    return;
  }

  pBuffer->CopyData(offset, size, pData);
}


void CGLContext::GetBufferParameter(GLenum target, GLenum pname,
                                    GLint *pParams) {
  GLenum err;

  ASSERT(pParams);

  if (target != GL_ARRAY_BUFFER) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::GetBufferParameter() failed, ")
                               _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  CBuffer *const pBuffer = this->GetBufferObject(target);
  ASSERT(pBuffer);

  err = pBuffer->GetParameter(pname, pParams);
  if (__glFailed(err)) {
    __glError(err, _T("CBuffer::GetParameter() failed, err = %d.\r\n"), err);
    return;
  }
}


void CGLContext::DeleteBuffers(GLsizei n, const GLuint *pBuffers) {
  ASSERT(pBuffers);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        _T("CGLContext::DeleteBuffers() failed, invalid n parameter: %d.\r\n"),
        n);
    return;
  }

  for (const GLuint *const pEnd = pBuffers + n; pBuffers != pEnd; ++pBuffers) {
    const GLuint hBuffer = *pBuffers;
    if (hBuffer) {
      CBuffer *const pBuffer =
          reinterpret_cast<CBuffer *>(m_pHandles->Delete(hBuffer, this));
      if (pBuffer) {
        //
        // Unbind the buffer if bound.
        //
        if (pBuffer == this->GetBufferObject(GL_ARRAY_BUFFER)) {
          this->SetBufferObject(GL_ARRAY_BUFFER, m_pBufDefault);
        }

        if (pBuffer == this->GetBufferObject(GL_ELEMENT_ARRAY_BUFFER)) {
          this->SetBufferObject(GL_ELEMENT_ARRAY_BUFFER, m_pBufDefault);
        }

        pBuffer->Release();
      }
    }
  }
}
