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
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#include "stdafx.h"
#include "driver.hpp"
#include "context.hpp"
#include "display.hpp"
#include "surface.hpp"

thread_local CEGLContext *tls_eglctx = nullptr;
thread_local EGLint tls_eglerror = EGL_SUCCESS;

CEGLDriver::CEGLDriver() : handles_(nullptr) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  // Create the handle table
  auto err = EGLERROR_FROM_HRESULT(CHandleTable::Create(&handles_));
  if (__eglFailed(err)) {
    __eglLogError(_T("CHandleTable::Create() failed, err = %x.\r\n"), err);
  }
}

CEGLDriver::~CEGLDriver() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  __safeRelease(tls_eglctx);

  if (handles_) {
    // Release all generated driver handles
    auto enumerator = handles_->GetEnumerator(this);
    while (!enumerator.IsEnd()) {
      reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
    }

    // The handle table should now be empty
    assert(0 == handles_->GetNumHandles());

    // Release the handle table
    handles_->Release();
  }
}

void CEGLDriver::MakeCurrent(CEGLContext *pContext, std::thread::id dwThreadID,
                             CEGLSurface *pSurfDraw, CEGLSurface *pSurfRead) {

  auto pCtxCurr = tls_eglctx;
  if (pCtxCurr != pContext) {
    if (pContext) {
      pContext->AddRef();
    }

    if (pCtxCurr) {
      pCtxCurr->SetBindings(dwThreadID, nullptr, nullptr);
      pCtxCurr->Release();
    }
    tls_eglctx = pContext;
  }

  if (pContext) {
    pContext->SetBindings(dwThreadID, pSurfDraw, pSurfRead);
  }
}

CEGLContext *CEGLDriver::GetCurrentContext() const { return tls_eglctx; }

void CEGLDriver::SetError(EGLint error) { tls_eglerror = error; }

EGLint CEGLDriver::GetError() const {
  auto error = tls_eglerror;
  tls_eglerror = EGL_SUCCESS;
  return error;
}

EGLint CEGLDriver::GetDisplay(uint32_t *pdwHandle,
                              EGLNativeDisplayType display_id) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  assert(pdwHandle);

  if (EGL_DEFAULT_DISPLAY == display_id) {
// Set the default display ID
#if defined(_WIN32)
    display_id = GetDC(nullptr);
#elif defined(__linux__)
    display_id = XOpenDisplay(nullptr);
#endif
  }

  // Look up for an existing display
  auto enumerator = handles_->GetEnumerator(this);
  while (!enumerator.IsEnd()) {
    if (HANDLE_DISPLAY == enumerator.GetType()) {
      auto pDisplay = reinterpret_cast<CDisplay *>(enumerator.GetObject());
      if (display_id == pDisplay->GetNativeHandle()) {
        *pdwHandle = enumerator.GetHandle();
        return EGL_SUCCESS;
      }
    }

    enumerator.MoveNext();
  }

  CDisplay *pDisplay;

  // Create a new display object
  err = CDisplay::Create(&pDisplay, display_id, handles_);
  if (nullptr == pDisplay) {
    __eglLogError(_T("CDisplay::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  // Add the display to the handle table
  err = EGLERROR_FROM_HRESULT(
      handles_->Insert(pdwHandle, pDisplay, HANDLE_DISPLAY, this));
  if (__eglFailed(err)) {
    __safeRelease(pDisplay);
    __eglLogError(_T("CHandleTable::InsertObject() failed, err = %x.\r\n"),
                  err);
    return EGL_BAD_ALLOC;
  }

  return EGL_SUCCESS;
}
