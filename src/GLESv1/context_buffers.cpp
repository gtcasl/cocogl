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

void CGLContext::GenBuffers(GLsizei n, GLuint *phBuffers) {
  GLenum err;

  assert(phBuffers);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        "CGLContext::GenBuffers() failed, invalid n parameter: %d.\r\n", n);
    return;
  }

  for (GLuint *phBuf = phBuffers, *const phEnd = phBuf + n; phBuf != phEnd;
       ++phBuf) {
    CBuffer *pBuffer;
    err = CBuffer::Create(&pBuffer);
    if (__glFailed(err)) {
      __glError(err, "CBuffer::Create() failed, err = %d.\r\n", err);
      return;
    }

    uint32_t dwHandle;
    err = GLERROR_FROM_HRESULT(
        handles_->Insert(&dwHandle, pBuffer, HANDLE_BUFFER, this));
    if (__glFailed(err)) {
      __safeRelease(pBuffer);
      __glError(err, "CHandleTable::Insert() failed, err = %d.\r\n", err);
      return;
    }

    pBuffer->SetHandle(dwHandle);

    *phBuf = dwHandle;
  }
}

void CGLContext::BindBuffer(GLenum target, GLuint buffer) {
  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM,
              "CGLContext::BindBuffer() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  CBuffer *pBuffer = nullptr;
  if (buffer) {
    // First, lookup buffers from the current context
    pBuffer = reinterpret_cast<CBuffer *>(handles_->GetObject(buffer, this));
    if (nullptr == pBuffer) {
      if (pCtxShared_) {
        // Else, lookup buffers from the shared context
        pBuffer = reinterpret_cast<CBuffer *>(
            handles_->GetObject(buffer, pCtxShared_));
      }

      if (nullptr == pBuffer) {
        __glError(GL_INVALID_VALUE,
                  "CGLContext::BindBuffer() failed, "
                  "Invalid buffer parameter: %d\r\n",
                  buffer);
        return;
      }
    }
  } else {
    pBuffer = pBufDefault_;
  }

  this->SetBufferObject(target, pBuffer);
}

void CGLContext::BufferData(GLenum target, GLsizeiptr size, const GLvoid *pData,
                            GLenum usage) {
  GLenum err;

  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM,
              "CGLContext::BufferData() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  if ((usage != GL_STATIC_DRAW) && (usage != GL_DYNAMIC_DRAW)) {
    __glError(
        GL_INVALID_ENUM,
        "CGLContext::BufferData() failed, invalid usage parameter: %d.\r\n",
        usage);
    return;
  }

  if (size < 0) {
    __glError(
        GL_INVALID_VALUE,
        "CGLContext::BufferData() failed, invalid size parameter: %d.\r\n",
        size);
    return;
  }

  auto pBuffer = this->GetBufferObject(target);
  assert(pBuffer);

  err = pBuffer->Initialize(size, usage, pData);
  if (__glFailed(err)) {
    __glError(err, "CBuffer::SetData() failed, err = %d.\r\n", err);
    return;
  }
}

void CGLContext::BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                               const GLvoid *pData) {
  assert(pData);

  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM,
              "CGLContext::BufferSubData() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  if ((offset < 0) || (size < 0)) {
    __glError(GL_INVALID_VALUE,
              "CGLContext::BufferSubData() failed, "
              "invalid size(%d) or offset(%d) "
              "parameters.\r\n",
              size, offset);
    return;
  }

  auto pBuffer = this->GetBufferObject(target);
  assert(pBuffer);

  if (uint32_t(offset + size) > pBuffer->GetSize()) {
    __glError(GL_INVALID_VALUE,
              "CGLContext::BufferSubData() failed, size "
              "+ offset parameters out of range: %d.\r\n",
              size + offset);
    return;
  }

  pBuffer->CopyData(offset, size, pData);
}

void CGLContext::GetBufferParameter(GLenum target, GLenum pname,
                                    GLint *pParams) {
  GLenum err;

  assert(pParams);

  if (target != GL_ARRAY_BUFFER) {
    __glError(GL_INVALID_ENUM,
              "CGLContext::GetBufferParameter() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  auto pBuffer = this->GetBufferObject(target);
  assert(pBuffer);

  err = pBuffer->GetParameter(pname, pParams);
  if (__glFailed(err)) {
    __glError(err, "CBuffer::GetParameter() failed, err = %d.\r\n", err);
    return;
  }
}

void CGLContext::DeleteBuffers(GLsizei n, const GLuint *phBuffers) {
  assert(phBuffers);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        "CGLContext::DeleteBuffers() failed, invalid n parameter: %d.\r\n",
        n);
    return;
  }

  for (const GLuint *phBuf = phBuffers, *const phEnd = phBuf + n;
       phBuf != phEnd; ++phBuf) {
    GLuint handle = *phBuf;
    if (handle) {
      auto pBuffer =
          reinterpret_cast<CBuffer *>(handles_->Delete(handle, this));
      if (pBuffer) {
        // Unbind the buffer if bound.
        if (pBuffer == this->GetBufferObject(GL_ARRAY_BUFFER)) {
          this->SetBufferObject(GL_ARRAY_BUFFER, pBufDefault_);
        }
        if (pBuffer == this->GetBufferObject(GL_ELEMENT_ARRAY_BUFFER)) {
          this->SetBufferObject(GL_ELEMENT_ARRAY_BUFFER, pBufDefault_);
        }
        pBuffer->Release();
      }
    }
  }
}
