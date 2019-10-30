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

  ASSERT(pDisplay);
  pDisplay->AddRef();
  m_pDisplay = pDisplay;

  ASSERT(pConfig);
  pConfig->AddRef();
  m_pConfig = pConfig;

  m_pSurfDraw = NULL;
  m_pSurfRead = NULL;

  m_glContext = NULL;
}

CEGLContext::~CEGLContext() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  if (m_glContext) {
    __glDestroyContext(m_glContext);
  }

  __safeRelease(m_pSurfDraw);
  __safeRelease(m_pSurfRead);
  __safeRelease(m_pConfig);
  __safeRelease(m_pDisplay);
}

EGLint CEGLContext::Create(CEGLContext **ppContext, CDisplay *pDisplay,
                           CConfig *pConfig, CEGLContext *pCtxShared) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(pDisplay && pConfig && ppContext);

  // Create a new context object
  CEGLContext *pContext = new CEGLContext(pDisplay, pConfig);
  if (NULL == pContext) {
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
    err = EGLERROR_FROM_GLERROR(
        __glCreateContext(pCtxShared->m_glContext, &m_glContext));
  } else {
    err = EGLERROR_FROM_GLERROR(__glCreateContext(NULL, &m_glContext));
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
    value = m_pConfig->GetAttribute(EGL_CONFIG_ID);
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
  m_dwThreadID = dwThreadID;

  if (pSurfDraw) {
    pSurfDraw->AddRef();
  }

  if (m_pSurfDraw) {
    m_pSurfDraw->Release();
  }

  m_pSurfDraw = pSurfDraw;

  if (pSurfRead) {
    pSurfRead->AddRef();
  }

  if (m_pSurfRead) {
    m_pSurfRead->Release();
  }

  m_pSurfRead = pSurfRead;
}
