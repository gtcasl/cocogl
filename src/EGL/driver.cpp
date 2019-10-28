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

thread_local CEGLContext* tls_ctx = nullptr;
thread_local EGLint tls_error = EGL_SUCCESS;

CEGLDriver::CEGLDriver() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
  m_pHandles = NULL;
}

CEGLDriver::~CEGLDriver() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));  
  if (m_pHandles) {
    // Release all generated driver handles
    CHandleTable::Enumerator enumerator = m_pHandles->GetEnumerator(this);
    while (!enumerator.IsEnd()) {
      reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
    }

    // The handle table should now be empty
    ASSERT(0 == m_pHandles->GetNumHandles());

    // Release the handle table
    m_pHandles->Release();
  }
}

EGLint CEGLDriver::Create(CEGLDriver **ppDriver) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(ppDriver);

  CEGLDriver *pDriver = new CEGLDriver();
  if (NULL == pDriver) {
    __eglLogError(_T("CEGLDriver allocation failed, out of memory.\r\n"));
    return EGL_BAD_ALLOC;
  }

  pDriver->AddRef();

  err = pDriver->Initialize();
  if (__eglFailed(err)) {
    __safeRelease(pDriver);
    __eglLogError(
        _T("CEGLDriver::Initialize() failed, out of memory, err = %x.\r\n"),
        err);
    return err;
  }

  *ppDriver = pDriver;

  return EGL_SUCCESS;
}


EGLint CEGLDriver::Initialize() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  // Create the handle table
  err = EGLERROR_FROM_HRESULT(CHandleTable::Create(&m_pHandles));
  if (__eglFailed(err)) {
    __eglLogError(_T("CHandleTable::Create() failed, err = %x.\r\n"), err);
    return err;
  }

  return EGL_SUCCESS;
}

void CEGLDriver::SetCurrentContext(CEGLContext *pContext) {
  tls_ctx = pContext;
}

CEGLContext* CEGLDriver::GetCurrentContext() const {
  return tls_ctx;
}


void CEGLDriver::SetError(EGLint error) {
  tls_error = error;
}


EGLint CEGLDriver::GetError() const {
  const EGLint error = tls_error;
  tls_error = EGL_SUCCESS;
  return error;
}


EGLint CEGLDriver::GetDisplay(uint32_t *pdwHandle,
                              EGLNativeDisplayType display_id) {
  std::lock_guard<std::mutex> lock(m_CS);

  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  ASSERT(pdwHandle);

  if (EGL_DEFAULT_DISPLAY == display_id) {
    // Set the default display ID
    display_id = ::GetDefaultDisplay();
  }

  // Look up for an existing display
  CHandleTable::Enumerator enumerator = m_pHandles->GetEnumerator(this);
  while (!enumerator.IsEnd()) {
    if (HANDLE_DISPLAY == enumerator.GetType()) {
      CDisplay *const pDisplay =
          reinterpret_cast<CDisplay *>(enumerator.GetObject());
      if (display_id == pDisplay->GetDC()) {
        *pdwHandle = enumerator.GetHandle();
        return EGL_SUCCESS;
      }
    }

    enumerator.MoveNext();
  }

  CDisplay *pDisplay;

  // Create a new display object
  err = CDisplay::Create(&pDisplay, display_id, m_pHandles);
  if (NULL == pDisplay) {
    __eglLogError(_T("CDisplay::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  // Add the display to the handle table
  err = EGLERROR_FROM_HRESULT(
      m_pHandles->Insert(pdwHandle, pDisplay, HANDLE_DISPLAY, this));
  if (__eglFailed(err)) {
    __safeRelease(pDisplay);
    __eglLogError(_T("CHandleTable::InsertObject() failed, err = %x.\r\n"),
                  err);
    return EGL_BAD_ALLOC;
  }

  return EGL_SUCCESS;
}
