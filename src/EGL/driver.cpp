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

thread_local _EGLContext *tls_eglctx = nullptr;
thread_local EGLint tls_eglerror = EGL_SUCCESS;

_EGLDriver::_EGLDriver() : handles_(nullptr) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  // Create the handle table
  handles_ = new HandleTable();
  handles_->addRef();
}

_EGLDriver::~_EGLDriver() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeRelease(tls_eglctx);

  if (handles_) {
    // Release all generated driver handles
    auto enumerator = handles_->getEnumerator(this);
    while (!enumerator.isEnd()) {
      reinterpret_cast<IObject *>(enumerator.removeNext())->release();
    }

    // The handle table should now be empty
    assert(0 == handles_->getNumHandles());

    // Release the handle table
    handles_->release();
  }
}

void _EGLDriver::makeCurrent(_EGLContext *pContext, std::thread::id dwThreadID,
                             _EGLSurface *pSurfDraw, _EGLSurface *pSurfRead) {

  auto pCtxCurr = tls_eglctx;
  if (pCtxCurr != pContext) {
    if (pContext) {
      pContext->addRef();
    }

    if (pCtxCurr) {
      pCtxCurr->setBindings(dwThreadID, nullptr, nullptr);
      pCtxCurr->release();
    }
    tls_eglctx = pContext;
  }

  if (pContext) {
    pContext->setBindings(dwThreadID, pSurfDraw, pSurfRead);
  }
}

_EGLContext *_EGLDriver::getCurrentContext() const { return tls_eglctx; }

void _EGLDriver::setError(EGLint error) { tls_eglerror = error; }

EGLint _EGLDriver::getError() const {
  auto error = tls_eglerror;
  tls_eglerror = EGL_SUCCESS;
  return error;
}

EGLint _EGLDriver::getDisplay(uint32_t *pdwHandle,
                              EGLNativeDisplayType display_id) {
  __profileAPI(" - %s()\n", __FUNCTION__);

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
  auto enumerator = handles_->getEnumerator(this);
  while (!enumerator.isEnd()) {
    if (HANDLE_DISPLAY == enumerator.getType()) {
      auto pDisplay = reinterpret_cast<_EGLDisplay *>(enumerator.getObject());
      if (display_id == pDisplay->getNativeHandle()) {
        *pdwHandle = enumerator.getHandle();
        return EGL_SUCCESS;
      }
    }

    enumerator.moveNext();
  }

  _EGLDisplay *pDisplay;

  // Create a new display object
  err = _EGLDisplay::Create(&pDisplay, display_id, handles_);
  if (nullptr == pDisplay) {
    __eglLogError("_EGLDisplay::Create() failed, err = %d.\r\n", err);
    return err;
  }

  // Add the display to the handle table
  err = EGLERROR_FROM_HRESULT(
      handles_->insert(pdwHandle, pDisplay, HANDLE_DISPLAY, this));
  if (__eglFailed(err)) {
    __safeRelease(pDisplay);
    __eglLogError("HandleTable::insert() failed, err = %x.\r\n",
                  err);
    return EGL_BAD_ALLOC;
  }

  return EGL_SUCCESS;
}
