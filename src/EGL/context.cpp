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
#include "context.hpp"
#include "config.hpp"
#include "display.hpp"
#include "surface.hpp"

_EGLContext::_EGLContext(_EGLDisplay *pDisplay, _EGLConfig *pConfig) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(pDisplay);
  pDisplay->addRef();
  pDisplay_ = pDisplay;

  assert(pConfig);
  pConfig->addRef();
  pConfig_ = pConfig;

  pSurfDraw_ = nullptr;
  pSurfRead_ = nullptr;

  glContext_ = nullptr;
}

_EGLContext::~_EGLContext() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (glContext_) {
    __glDestroyContext(glContext_);
  }

  __safeRelease(pSurfDraw_);
  __safeRelease(pSurfRead_);
  __safeRelease(pConfig_);
  __safeRelease(pDisplay_);
}

EGLint _EGLContext::Create(_EGLContext **ppGLContext, 
                           _EGLDisplay *pDisplay,
                           _EGLConfig *pConfig, 
                           _EGLContext *pCtxShared) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  assert(pDisplay && pConfig && ppGLContext);

  // Create a new context object
  auto pGLContext = new _EGLContext(pDisplay, pConfig);
  if (nullptr == pGLContext) {
    __eglLogError("EGLGLContext allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pGLContext->addRef();

  // Initialize the context
  err = pGLContext->initialize(pCtxShared);
  if (__eglFailed(err)) {
    __safeRelease(pGLContext);
    __eglLogError("GLContext::initialize() failed, err = %d.\r\n", err);
    return err;
  }

  *ppGLContext = pGLContext;

  return EGL_SUCCESS;
}

EGLint _EGLContext::initialize(_EGLContext *pCtxShared) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  // Create the GL context
  if (pCtxShared) {
    err = EGLERROR_FROM_HRESULT(
        __glCreateContext(pCtxShared->glContext_, &glContext_));
  } else {
    err = EGLERROR_FROM_HRESULT(__glCreateContext(nullptr, &glContext_));
  }

  if (__eglFailed(err)) {
    __eglLogError("__glCreateGLContext() failed, err = %d.\r\n", err);
    return err;
  }

  return EGL_SUCCESS;
}

EGLint _EGLContext::getAttribute(EGLint name, EGLint *pValue) {
  EGLint value;

  switch (name) {
  case EGL_CONFIG_ID:
    value = pConfig_->getAttribute(EGL_CONFIG_ID);
    break;

  default:
    __eglLogError("Invalid attribute name : %d.\r\n", name);
    return EGL_BAD_ATTRIBUTE;
  }

  if (pValue) {
    *pValue = value;
  }

  return EGL_SUCCESS;
}

void _EGLContext::setBindings(std::thread::id dwThreadID,
                              _EGLSurface *pSurfDraw, 
                              _EGLSurface *pSurfRead) {
  dwThreadID_ = dwThreadID;

  if (pSurfDraw) {
    pSurfDraw->addRef();
  }

  if (pSurfDraw_) {
    pSurfDraw_->release();
  }

  pSurfDraw_ = pSurfDraw;

  if (pSurfRead) {
    pSurfRead->addRef();
  }

  if (pSurfRead_) {
    pSurfRead_->release();
  }

  pSurfRead_ = pSurfRead;
}
