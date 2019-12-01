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

class _EGLContext;
class _EGLSurface;

class _EGLDriver {
public:
  _EGLDriver();
  ~_EGLDriver();

  EGLint getDisplay(uint32_t *phandle, EGLNativeDisplayType display_id);

  void setError(EGLint error);
  EGLint getError() const;

  template <class T> 
  inline T getObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->getObject(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> 
  inline T getObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->getObject(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  EGLint registerObject(uint32_t *phandle, void *pObject, uint8_t type) {
    return EGLERROR_FROM_HRESULT(
        handles_->insert(phandle, pObject, type, this));
  }  

  EGLint registerObject(uint32_t *phandle, void *pObject, uint8_t type, void *pOwner) {
    return EGLERROR_FROM_HRESULT(
        handles_->insert(phandle, pObject, type, pOwner));
  }

  template <class T> 
  inline T unregisterObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->deleteHandle(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T>
  inline T unregisterObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->deleteHandle(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  uint32_t getHandle(const void *pObject) {
    return handles_->findHandle(pObject, this);
  }

  uint32_t getHandle(const void *pObject, void *pOwner) {
    return handles_->findHandle(pObject, pOwner);
  }

  void makeCurrent(_EGLContext *pContext, 
                   std::thread::id threadID,
                   _EGLSurface *pSurfDraw, 
                   _EGLSurface *pSurfRead);

  _EGLContext *getCurrentContext() const;

private:
  HandleTable *handles_;
};
