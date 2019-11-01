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

class CBuffer : public CObject {
public:
  static GLenum Create(CBuffer **ppBuffer);

  GLenum Initialize(uint32_t size, GLenum usage, const GLvoid *pData);

  void CopyData(uint32_t offset, uint32_t size, const GLvoid *pData) {
    ::memcpy(m_pBits + offset, pData, size);
  }

  GLenum GetParameter(GLenum pname, GLint *pParams);

  const uint8_t *GetBits() const { return m_pBits; }

  uint32_t GetSize() const { return m_size; }

  GLenum GetUsage() const { return m_usage; }

  uint32_t GetHandle() const { return m_dwHandle; }

  void SetHandle(uint32_t dwHandle) { m_dwHandle = dwHandle; }

private:
  CBuffer();
  ~CBuffer();

  uint32_t m_dwHandle;
  uint8_t *m_pBits;
  uint32_t m_size;
  GLenum m_usage;
};
