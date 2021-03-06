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

class GLBuffer : public Object {
public:
  static GLenum Create(GLBuffer **ppBuffer);

  GLenum initialize(uint32_t size, GLenum usage, const GLvoid *pData);

  void copyData(uint32_t offset, uint32_t size, const GLvoid *pData) {
    ::memcpy(pBits_ + offset, pData, size);
  }

  GLenum getParameter(GLenum pname, GLint *pParams);

  const uint8_t *getBits() const {
    return pBits_;
  }

  uint32_t getSize() const {
    return size_;
  }

  GLenum getUsage() const {
    return usage_;
  }

  uint32_t getHandle() const {
    return handle_;
  }

  void setHandle(uint32_t handle) {
    handle_ = handle;
  }

private:
  GLBuffer();
  ~GLBuffer();

  uint32_t handle_;
  uint8_t *pBits_;
  uint32_t size_;
  GLenum usage_;
};
