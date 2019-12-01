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

class CGLDriver {
public:
  CGLDriver();
  ~CGLDriver();

  void MakeCurrent(CGLContext *pContext, CGLSurface *pSurfDraw,
                   CGLSurface *pSurfRead);

  CGLContext *GetCurrentContext() const;

  CHandleTable *GetHandles() const { return handles_; }

  CRasterCache *GetRasterCache() const { return pRasterCache_; }

  template <class T> 
  inline T TGetObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->GetObject(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> 
  inline T UnregisterObject(void *handle) const {
    return reinterpret_cast<T>(
        handles_->Delete(reinterpret_cast<intptr_t>(handle), this));
  }

  template <class T> 
  inline T TGetObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->GetObject(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  template <class T>
  inline T UnregisterObject(void *handle, void *pOwner) const {
    return reinterpret_cast<T>(
        handles_->Delete(reinterpret_cast<intptr_t>(handle), pOwner));
  }

  GLenum RegisterObject(void *pObject, uint8_t type, void *pOwner,
                        uint32_t *pdwHandle) {
    return GLERROR_FROM_HRESULT(
        handles_->Insert(pdwHandle, pObject, type, pOwner));
  }

private:
  CHandleTable *handles_;
  CRasterCache *pRasterCache_;
};
