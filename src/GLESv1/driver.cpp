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

CGLDriver::CGLDriver() : handles_(nullptr), pRasterCache_(nullptr) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  // Create the handle table
  err = GLERROR_FROM_HRESULT(CHandleTable::Create(&handles_));
  if (__glFailed(err)) {
    __glLogError("CHandleTable::Create() failed, err = %x.\r\n", err);
    return;
  }

  // Create the raster cache
  err = CRasterCache::Create(&pRasterCache_);
  if (__glFailed(err)) {
    __glLogError("CRasterCache::Create() failed, err = %x.\r\n", err);
    return;
  }
}

CGLDriver::~CGLDriver() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeRelease(tls_glctx);
  __safeRelease(pRasterCache_);

  if (handles_) {
    {
      // Release all generated driver handles
      auto enumerator = handles_->GetEnumerator(this);
      while (!enumerator.IsEnd()) {
        reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
      }
    }

    {
      // Release all other generated handles
      auto enumerator = handles_->GetEnumerator(nullptr);
      while (!enumerator.IsEnd()) {
        reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
      }
    }

    // The handle table should now be empty
    assert(0 == handles_->GetNumHandles());

    // Release the handle table
    handles_->Release();
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
