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

class GLContext;
class GLSurface;
class ThreadPool;
class RasterCache;

class GLDriver {
public:
  GLDriver();
  ~GLDriver();

  void makeCurrent(GLContext *pContext, GLSurface *pSurfDraw,
                   GLSurface *pSurfRead);

  GLContext *getCurrentContext() const;

  HandleTable *getHandles() const { return handles_; }

  RasterCache *getRasterCache() const { return pRasterCache_; }

  template <typename T> inline T getObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->getObject(reinterpret_cast<intptr_t>(handle), this));
  }

  template <typename T> inline T getObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->getObject(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  GLenum registerObject(uint32_t *phandle, void *pObject, uint8_t type) {
    return GLERROR_FROM_HRESULT(handles_->insert(phandle, pObject, type, this));
  }

  GLenum registerObject(uint32_t *phandle, void *pObject, uint8_t type,
                        void *pOwner) {
    return GLERROR_FROM_HRESULT(
        handles_->insert(phandle, pObject, type, pOwner));
  }

  template <typename T> inline T unregisterObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->deleteHandle(reinterpret_cast<intptr_t>(handle), this));
  }

  template <typename T>
  inline T unregisterObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->deleteHandle(reinterpret_cast<intptr_t>(handle), pOwner));
  }

private:
  HandleTable *handles_;
  ThreadPool* threadpool_;
  RasterCache *pRasterCache_;
};
