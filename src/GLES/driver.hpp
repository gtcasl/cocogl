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


class CGLDriver : public CObject {
public:
  static GLenum Create(CGLDriver **ppDriver);

  void MakeCurrent(CGLContext *pContext, CGLSurface *pSurfDraw,
                   CGLSurface *pSurfRead);

  CGLContext *GetCurrentContext() const;

  CHandleTable *GetHandles() const { return m_pHandles; }

  CRasterCache *GetRasterCache() const { return m_pRasterCache; }

  template <class T> inline T TGetObject(void *object) const {
    return reinterpret_cast<T>(
        m_pHandles->GetObject(reinterpret_cast<intptr_t>(object), this));
  }

  template <class T> inline T TDeleteObject(void *object) const {
    return reinterpret_cast<T>(
        m_pHandles->Delete(reinterpret_cast<intptr_t>(object), this));
  }

  template <class T> inline T TGetObject(void *object, void *pOwner) const {
    return reinterpret_cast<T>(
        m_pHandles->GetObject(reinterpret_cast<intptr_t>(object), pOwner));
  }

  template <class T> inline T TDeleteObject(void *object, void *pOwner) const {
    return reinterpret_cast<T>(
        m_pHandles->Delete(reinterpret_cast<intptr_t>(object), pOwner));
  }

  GLenum AddObject(void *pObject, uint8_t type, void *pOwner, uint32_t *pdwHandle) {
    return GLERROR_FROM_HRESULT(
        m_pHandles->Insert(pdwHandle, pObject, type, pOwner));
  }

private:
  CGLDriver();
  ~CGLDriver();

  GLenum Initialize();

  CHandleTable *m_pHandles;
  CRasterCache *m_pRasterCache;
};
