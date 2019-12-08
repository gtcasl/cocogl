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

_EGLDriver::_EGLDriver() {
  __profileAPI(" - %s()\n", __FUNCTION__);
}

_EGLDriver::~_EGLDriver() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeRelease(tls_eglctx);

  // Release all handles
  auto enumerator = handles_.getEnumerator();
  while (!enumerator.isEnd()) {
    reinterpret_cast<IObject *>(enumerator.getObject())->release();
    enumerator.moveNext();
  }
}

EGLint _EGLDriver::getDisplay(uint32_t *phandle,
                              EGLNativeDisplayType display_id) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(phandle);  

  // Look up existing displays
  auto enumerator = handles_.getEnumerator();
  while (!enumerator.isEnd()) {
    if (HANDLE_DISPLAY == enumerator.getType()) {
      auto pDisplay = reinterpret_cast<_EGLDisplay *>(enumerator.getObject());
      if (display_id == pDisplay->getNativeHandle()
       || (EGL_DEFAULT_DISPLAY == display_id && pDisplay->isDefault())) {
        *phandle = handles_.getHandle(enumerator);
        return EGL_SUCCESS;
      }
    }
    enumerator.moveNext();
  }

  _EGLDisplay *pDisplay;

  // Create a new display object
  err = _EGLDisplay::Create(&pDisplay, display_id);
  if (nullptr == pDisplay) {
    __eglLogError("_EGLDisplay::Create() failed, err = %d.\r\n", err);
    return err;
  }

  // Add the display to the handle table
  err = EGLERROR_FROM_HRESULT(
      handles_.insert(phandle, pDisplay, HANDLE_DISPLAY));
  if (__eglFailed(err)) {
    __eglLogError("HandleTable::insert() failed, err = %x.\r\n", err);
    __safeRelease(pDisplay);
    return EGL_BAD_ALLOC;
  }

  return EGL_SUCCESS;
}

EGLint  _EGLDriver::makeCurrent(_EGLContext *pContext, 
                                std::thread::id threadID,
                                _EGLSurface *pSurfDraw, 
                                _EGLSurface *pSurfRead) {

  // Set the current GL context first    
  if (pContext) {
    auto err = EGLERROR_FROM_HRESULT(__glMakeCurrent(pContext->getNativeData(),
                                                     pSurfDraw->getNativeData(),
                                                     pSurfRead->getNativeData()));
    if (__eglFailed(err)) {
      __eglLogError("__glMakeCurrent() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    auto err = EGLERROR_FROM_HRESULT(__glMakeCurrent(nullptr, nullptr, nullptr));
    if (__eglFailed(err)) {
      __eglLogError("__glMakeCurrent() failed, err = %d.\r\n", err);
      return err;
    }
  }
  
  auto pCtxCurr = tls_eglctx;
  if (pCtxCurr != pContext) {
    if (pContext) {
      pContext->addRef();
    }
    __safeRelease(pCtxCurr);
    tls_eglctx = pContext;
  }

  if (pContext) {
    pContext->setBindings(threadID, pSurfDraw, pSurfRead);
  }

  return EGL_SUCCESS;
}

_EGLContext *_EGLDriver::getCurrentContext() const {
  return tls_eglctx;
}

void _EGLDriver::setError(EGLint error) {
  tls_eglerror = error;
}

EGLint _EGLDriver::getError() const {
  auto error = tls_eglerror;
  tls_eglerror = EGL_SUCCESS;
  return error;
}