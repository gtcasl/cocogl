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
#include "context.hpp"

class GLDriver {
public:
  GLDriver();
  ~GLDriver();

  void makeCurrent(GLContext *pContext, GLSurface *pSurfDraw,
                   GLSurface *pSurfRead);

  GLContext *getCurrentContext() const;

  HandleTable *getHandles() const { return handles_; }

  CRasterCache *getRasterCache() const { return pRasterCache_; }

  template <class T> 
  inline T getObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->getObject(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> 
  inline T unregisterObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->deleteHandle(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> 
  inline T getObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->getObject(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  template <class T>
  inline T unregisterObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->deleteHandle(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  GLenum registerObject(void *pObject, uint8_t type, void *pOwner,
                        uint32_t *pdwHandle) {
    return GLERROR_FROM_HRESULT(
        handles_->insert(pdwHandle, pObject, type, pOwner));
  }

private:
  HandleTable *handles_;
  CRasterCache *pRasterCache_;
};
