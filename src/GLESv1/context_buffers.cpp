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

void GLContext::genBuffers(GLsizei n, GLuint *phBuffers) {
  GLenum err;

  assert(phBuffers);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::genBuffers() failed, invalid n parameter: %d.\r\n", n);
    return;
  }

  for (GLuint *phBuf = phBuffers, *const phEnd = phBuf + n; phBuf != phEnd;
       ++phBuf) {
    GLBuffer *pBuffer;
    err = GLBuffer::Create(&pBuffer);
    if (__glFailed(err)) {
      __glError(err, "GLBuffer::Create() failed, err = %d.\r\n", err);
      return;
    }

    uint32_t dwHandle;
    err = GLERROR_FROM_HRESULT(
        handles_->insert(&dwHandle, pBuffer, HANDLE_BUFFER, this));
    if (__glFailed(err)) {
      __safeRelease(pBuffer);
      __glError(err, "HandleTable::insert() failed, err = %d.\r\n", err);
      return;
    }

    pBuffer->setHandle(dwHandle);

    *phBuf = dwHandle;
  }
}

void GLContext::bindBuffer(GLenum target, GLuint buffer) {
  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM,
              "GLContext::bindBuffer() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  GLBuffer *pBuffer = nullptr;
  if (buffer) {
    // First, lookup buffers from the current context
    pBuffer = reinterpret_cast<GLBuffer *>(handles_->getObject(buffer, this));
    if (nullptr == pBuffer) {
      if (pCtxShared_) {
        // Else, lookup buffers from the shared context
        pBuffer = reinterpret_cast<GLBuffer *>(
            handles_->getObject(buffer, pCtxShared_));
      }

      if (nullptr == pBuffer) {
        __glError(GL_INVALID_VALUE,
                  "GLContext::bindBuffer() failed, "
                  "Invalid buffer parameter: %d\r\n",
                  buffer);
        return;
      }
    }
  } else {
    pBuffer = pBufDefault_;
  }

  this->setBufferObject(target, pBuffer);
}

void GLContext::setBufferData(GLenum target, GLsizeiptr size, const GLvoid *pData,
                            GLenum usage) {
  GLenum err;

  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setBufferData() failed, invalid "
              "target parameter: %d.\r\n",
              target);
    return;
  }

  if ((usage != GL_STATIC_DRAW) && (usage != GL_DYNAMIC_DRAW)) {
    __glError(
        GL_INVALID_ENUM,
        "GLContext::setBufferData() failed, invalid usage parameter: %d.\r\n",
        usage);
    return;
  }

  if (size < 0) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::setBufferData() failed, invalid size parameter: %d.\r\n",
        size);
    return;
  }

  auto pBuffer = this->getBufferObject(target);
  assert(pBuffer);

  err = pBuffer->initialize(size, usage, pData);
  if (__glFailed(err)) {
    __glError(err, "GLBuffer::initialize() failed, err = %d.\r\n", err);
    return;
  }
}

void GLContext::setBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                               const GLvoid *pData) {
  assert(pData);

  if ((target != GL_ARRAY_BUFFER) && (target != GL_ELEMENT_ARRAY_BUFFER)) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setBufferSubData() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  if ((offset < 0) || (size < 0)) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setBufferSubData() failed, "
              "invalid size(%d) or offset(%d) "
              "parameters.\r\n",
              size, offset);
    return;
  }

  auto pBuffer = this->getBufferObject(target);
  assert(pBuffer);

  if (uint32_t(offset + size) > pBuffer->getSize()) {
    __glError(GL_INVALID_VALUE,
              "GLContext::setBufferSubData() failed, size "
              "+ offset parameters out of range: %d.\r\n",
              size + offset);
    return;
  }

  pBuffer->copyData(offset, size, pData);
}

void GLContext::getBufferParameter(GLenum target, GLenum pname,
                                    GLint *pParams) {
  GLenum err;

  assert(pParams);

  if (target != GL_ARRAY_BUFFER) {
    __glError(GL_INVALID_ENUM,
              "GLContext::getBufferParameter() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  auto pBuffer = this->getBufferObject(target);
  assert(pBuffer);

  err = pBuffer->getParameter(pname, pParams);
  if (__glFailed(err)) {
    __glError(err, "GLBuffer::getParameter() failed, err = %d.\r\n", err);
    return;
  }
}

void GLContext::deleteBuffers(GLsizei n, const GLuint *phBuffers) {
  assert(phBuffers);

  if (n < 0) {
    __glError(
        GL_INVALID_VALUE,
        "GLContext::deleteBuffers() failed, invalid n parameter: %d.\r\n",
        n);
    return;
  }

  for (const GLuint *phBuf = phBuffers, *const phEnd = phBuf + n;
       phBuf != phEnd; ++phBuf) {
    GLuint handle = *phBuf;
    if (handle) {
      auto pBuffer =
          reinterpret_cast<GLBuffer *>(handles_->deleteHandle(handle, this));
      if (pBuffer) {
        // Unbind the buffer if bound.
        if (pBuffer == this->getBufferObject(GL_ARRAY_BUFFER)) {
          this->setBufferObject(GL_ARRAY_BUFFER, pBufDefault_);
        }
        if (pBuffer == this->getBufferObject(GL_ELEMENT_ARRAY_BUFFER)) {
          this->setBufferObject(GL_ELEMENT_ARRAY_BUFFER, pBufDefault_);
        }
        pBuffer->release();
      }
    }
  }
}
