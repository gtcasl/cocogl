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

CEGLContext::CEGLContext(CDisplay *pDisplay, CConfig *pConfig) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  assert(pDisplay);
  pDisplay->AddRef();
  pDisplay_ = pDisplay;

  assert(pConfig);
  pConfig->AddRef();
  pConfig_ = pConfig;

  pSurfDraw_ = nullptr;
  pSurfRead_ = nullptr;

  glContext_ = nullptr;
}

CEGLContext::~CEGLContext() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  if (glContext_) {
    __glDestroyContext(glContext_);
  }

  __safeRelease(pSurfDraw_);
  __safeRelease(pSurfRead_);
  __safeRelease(pConfig_);
  __safeRelease(pDisplay_);
}

EGLint CEGLContext::Create(CEGLContext **ppContext, CDisplay *pDisplay,
                           CConfig *pConfig, CEGLContext *pCtxShared) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  assert(pDisplay && pConfig && ppContext);

  // Create a new context object
  auto pContext = new CEGLContext(pDisplay, pConfig);
  if (nullptr == pContext) {
    __eglLogError(_T("CEGLContext allocation failed, out of memory.\r\n"));
    return EGL_BAD_ALLOC;
  }

  pContext->AddRef();

  // Initialize the context
  err = pContext->Initialize(pCtxShared);
  if (__eglFailed(err)) {
    __safeRelease(pContext);
    __eglLogError(_T("CEGLContext::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  *ppContext = pContext;

  return EGL_SUCCESS;
}

EGLint CEGLContext::Initialize(CEGLContext *pCtxShared) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  // Create the GL context
  if (pCtxShared) {
    err = EGLERROR_FROM_HRESULT(
        __glCreateContext(pCtxShared->glContext_, &glContext_));
  } else {
    err = EGLERROR_FROM_HRESULT(__glCreateContext(nullptr, &glContext_));
  }

  if (__eglFailed(err)) {
    __eglLogError(_T("__glCreateContext() failed, err = %d.\r\n"), err);
    return err;
  }

  return EGL_SUCCESS;
}

EGLint CEGLContext::GetAttribute(EGLint name, EGLint *pValue) {
  EGLint value;

  switch (name) {
  case EGL_CONFIG_ID:
    value = pConfig_->GetAttribute(EGL_CONFIG_ID);
    break;

  default:
    __eglLogError(_T("Invalid attribute name : %d.\r\n"), name);
    return EGL_BAD_ATTRIBUTE;
  }

  if (pValue) {
    *pValue = value;
  }

  return EGL_SUCCESS;
}

void CEGLContext::SetBindings(std::thread::id dwThreadID,
                              CEGLSurface *pSurfDraw, CEGLSurface *pSurfRead) {
  dwThreadID_ = dwThreadID;

  if (pSurfDraw) {
    pSurfDraw->AddRef();
  }

  if (pSurfDraw_) {
    pSurfDraw_->Release();
  }

  pSurfDraw_ = pSurfDraw;

  if (pSurfRead) {
    pSurfRead->AddRef();
  }

  if (pSurfRead_) {
    pSurfRead_->Release();
  }

  pSurfRead_ = pSurfRead;
}
