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

thread_local CGLContext* tls_glctx = nullptr;

CGLDriver::CGLDriver() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
  m_pHandles = NULL;
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
      auto enumerator = m_pHandles->GetEnumerator(NULL);
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


GLenum CGLDriver::Create(CGLDriver **ppDriver) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  ASSERT(ppDriver);

  CGLDriver *pDriver = new CGLDriver();
  if (NULL == pDriver) {
    __glLogError(_T("CGLDriver allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  pDriver->AddRef();

  err = pDriver->Initialize();
  if (__glFailed(err)) {
    __safeRelease(pDriver);
    __glLogError(
        _T("CGLDriver::Initialize() failed, out of memory, err = %x.\r\n"),
        err);
    return err;
  }

  *ppDriver = pDriver;

  return GL_NO_ERROR;
}


GLenum CGLDriver::Initialize() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  // Create the handle table
  err = GLERROR_FROM_HRESULT(CHandleTable::Create(&m_pHandles));
  if (__glFailed(err)) {
    __glLogError(_T("CHandleTable::Create() failed, err = %x.\r\n"), err);
    return err;
  }

  // Create the raster cache
  err = CRasterCache::Create(&m_pRasterCache);
  if (__glFailed(err)) {
    __glLogError(_T("CRasterCache::Create() failed, err = %x.\r\n"), err);
    return err;
  }

  return GL_NO_ERROR;
}

CGLContext *CGLDriver::GetCurrentContext() const {
    return tls_glctx;
  }

void CGLDriver::MakeCurrent(CGLContext *pContext, CGLSurface *pSurfDraw,
                            CGLSurface *pSurfRead) {
  CGLContext *pCtxCurr = this->GetCurrentContext();
  if (pCtxCurr != pContext) {
    if (pContext) {
      pContext->AddRef();
    }

    if (pCtxCurr) {
      pCtxCurr->SetDrawSurface(NULL);
      pCtxCurr->SetReadSurface(NULL);
      pCtxCurr->Release();
    }

    tls_glctx = pContext;
  }

  if (pContext) {
    pContext->SetDrawSurface(pSurfDraw);
    pContext->SetReadSurface(pSurfRead);
  }
}
