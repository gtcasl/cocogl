// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#pragma once

class CEGLContext;
class CEGLSurface;

class CEGLDriver {
public:
  CEGLDriver();
  ~CEGLDriver();

  EGLint GetDisplay(uint32_t *pdwHandle, EGLNativeDisplayType display_id);

  void SetError(EGLint error);
  EGLint GetError() const;

  template <class T> inline T TGetObject(void *handle) const {
    return reinterpret_cast<T>(
        m_pHandles->GetObject(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> inline T UnregisterObject(void *handle) const {
    return reinterpret_cast<T>(
        m_pHandles->Delete(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> inline T TGetObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        m_pHandles->GetObject(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  template <class T> inline T UnregisterObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        m_pHandles->Delete(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  uint32_t GetHandle(const void *pObject) {
    return m_pHandles->FindHandle(pObject, this);
  }

  uint32_t GetHandle(const void *pObject, void *pOwner) {
    return m_pHandles->FindHandle(pObject, pOwner);
  }

  void MakeCurrent(CEGLContext *pContext, std::thread::id dwThreadID,
                   CEGLSurface *pSurfDraw, CEGLSurface *pSurfRead);

  CEGLContext *GetCurrentContext() const;

  EGLint RegisterObject(uint32_t *pdwHandle, void *pObject, uint8_t type,
                   void *pOwner) {
    return EGLERROR_FROM_HRESULT(
        m_pHandles->Insert(pdwHandle, pObject, type, pOwner));
  }

private:

  CHandleTable *m_pHandles;
};
