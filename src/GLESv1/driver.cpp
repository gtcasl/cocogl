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
#include "driver.hpp"

thread_local CGLContext *tls_glctx = nullptr;

CGLDriver::CGLDriver() : m_pHandles(nullptr), m_pRasterCache(nullptr) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  // Create the handle table
  err = GLERROR_FROM_HRESULT(CHandleTable::Create(&m_pHandles));
  if (__glFailed(err)) {
    __glLogError(_T("CHandleTable::Create() failed, err = %x.\r\n"), err);
    return;
  }

  // Create the raster cache
  err = CRasterCache::Create(&m_pRasterCache);
  if (__glFailed(err)) {
    __glLogError(_T("CRasterCache::Create() failed, err = %x.\r\n"), err);
    return;
  }
}

CGLDriver::~CGLDriver() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  __safeRelease(tls_glctx);
  __safeRelease(m_pRasterCache);

  if (m_pHandles) {
    {
      // Release all generated driver handles
      auto enumerator = m_pHandles->GetEnumerator(this);
      while (!enumerator.IsEnd()) {
        reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
      }
    }

    {
      // Release all other generated handles
      auto enumerator = m_pHandles->GetEnumerator(nullptr);
      while (!enumerator.IsEnd()) {
        reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
      }
    }

    // The handle table should now be empty
    ASSERT(0 == m_pHandles->GetNumHandles());

    // Release the handle table
    m_pHandles->Release();
  }
}

CGLContext *CGLDriver::GetCurrentContext() const { return tls_glctx; }

void CGLDriver::MakeCurrent(CGLContext *pContext, CGLSurface *pSurfDraw,
                            CGLSurface *pSurfRead) {
  CGLContext *pCtxCurr = this->GetCurrentContext();
  if (pCtxCurr != pContext) {
    if (pContext) {
      pContext->AddRef();
    }

    if (pCtxCurr) {
      pCtxCurr->SetDrawSurface(nullptr);
      pCtxCurr->SetReadSurface(nullptr);
      pCtxCurr->Release();
    }

    tls_glctx = pContext;
  }

  if (pContext) {
    pContext->SetDrawSurface(pSurfDraw);
    pContext->SetReadSurface(pSurfRead);
  }
}
