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
#include "display.hpp"
#include "config.hpp"

CDisplay::CDisplay(EGLNativeDisplayType hNative, CHandleTable *pHandles) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  ASSERT(pHandles);
  pHandles->AddRef();
  m_pHandles = pHandles;

  m_hNative = hNative;
  m_bInitialized = false;
}

CDisplay::~CDisplay() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  if (m_hNative) {
#if defined(_WIN32)
    ReleaseDC(m_hNative);
#elif defined(__linux__)
    XCloseDisplay(m_hNative);
#endif
  }

  CHandleTable::Enumerator enumerator = m_pHandles->GetEnumerator(this);
  while (!enumerator.IsEnd()) {
    reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
  }

  __safeRelease(m_pHandles);
}

EGLint CDisplay::Create(CDisplay **ppDisplay, EGLNativeDisplayType hDC,
                        CHandleTable *pHandles) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  ASSERT(pHandles && ppDisplay);

  // Create a new display object
  CDisplay *const pDisplay = new CDisplay(hDC, pHandles);
  if (NULL == pDisplay) {
    __eglLogError(_T("CDisplay allocation failed, out of memory"));
    return EGL_BAD_ALLOC;
  }

  pDisplay->AddRef();

  *ppDisplay = pDisplay;

  return EGL_SUCCESS;
}

EGLint CDisplay::Initialize(EGLint *pMajor, EGLint *pMinor) {
  std::lock_guard<std::mutex> lock(m_CS);

  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;

  if (!m_bInitialized) {
  #if defined(COCOGL_RASTER_R5G6B5)
    err = this->CreateConfig(5, 6, 5, 0, 0, 0);
    if (__eglFailed(err)) {
      __eglLogError(_T("TList::Add() failed, err = %d.\r\n"), err);
      return err;
    }

    err = this->CreateConfig(5, 6, 5, 0, 16, 0);
    if (__eglFailed(err)) {
      __eglLogError(_T("TList::Add() failed, err = %d.\r\n"), err);
      return err;
    }

    err = this->CreateConfig(5, 6, 5, 0, 16, 8);
    if (__eglFailed(err)) {
      __eglLogError(_T("TList::Add() failed, err = %d.\r\n"), err);
      return err;
    }
  #elif defined (COCOGL_RASTER_A8R8G8B8)
    err = this->CreateConfig(8, 8, 8, 8, 0, 0);
    if (__eglFailed(err)) {
      __eglLogError(_T("TList::Add() failed, err = %d.\r\n"), err);
      return err;
    }

    err = this->CreateConfig(8, 8, 8, 8, 16, 0);
    if (__eglFailed(err)) {
      __eglLogError(_T("TList::Add() failed, err = %d.\r\n"), err);
      return err;
    }

    err = this->CreateConfig(8, 8, 8, 8, 16, 8);
    if (__eglFailed(err)) {
      __eglLogError(_T("TList::Add() failed, err = %d.\r\n"), err);
      return err;
    }
  #endif

    m_bInitialized = true;
  }

  if (pMajor) {
    *pMajor = EGL_VERSION_MAJOR;
  }

  if (pMinor) {
    *pMinor = EGL_VERSION_MINOR;
  }

  return EGL_SUCCESS;
}

EGLint CDisplay::CreateConfig(EGLint red, EGLint green, EGLint blue,
                              EGLint alpha, EGLint depth, EGLint stencil) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  EGLint err;
  CConfig *pConfig;

  // Create a new config object
  err = CConfig::Create(&pConfig, red, green, blue, alpha, depth, stencil);
  if (__eglFailed(err)) {
    __eglLogError(_T("CConfig::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  // Find the insertion sort iterator
  ConfigList::Iter iter = m_configs.GetBegin();
  for (ConfigList::Iter iterEnd = m_configs.GetEnd(); iter != iterEnd; ++iter) {
    const int cmp = CConfig::Compare(*iter, pConfig);
    if (cmp >= 0) {
      break;
    }
  }

  // Insert the config object into sorted list
  err = EGLERROR_FROM_HRESULT(m_configs.Insert(iter, pConfig));
  if (__eglFailed(err)) {
    __safeRelease(pConfig);
    __eglLogError(_T("TList::InsertAfter() failed, err = %d.\r\n"), err);
    return err;
  }

  uint32_t dwHandle;

  // Add the config object into handle table
  err = EGLERROR_FROM_HRESULT(
      m_pHandles->Insert(&dwHandle, pConfig, HANDLE_CONFIG, this));
  if (__eglFailed(err)) {
    __safeRelease(pConfig);
    __eglLogError(_T("CHandleTable::Insert() failed, err = %d.\r\n"), err);
    return err;
  }

  // Update the config ID
  pConfig->SetAttribute(EGL_CONFIG_ID, dwHandle);

  return EGL_SUCCESS;
}

EGLint CDisplay::QueryString(LPCSTR *plpValue, EGLint name) {
  ASSERT(plpValue);

  // Verify that the display was initialized
  if (!m_bInitialized) {
    __eglLogError(_T("The display was not initialized.\r\n"));
    return EGL_NOT_INITIALIZED;
  }

  switch (name) {
  case EGL_VENDOR:
    *plpValue = SZ_EGL_VENDOR;
    break;

  case EGL_VERSION:
    *plpValue = SZ_EGL_VERSION;
    break;

  case EGL_EXTENSIONS:
    *plpValue = SZ_EGL_EXTENSIONS;
    break;

  default:
    __eglLogError(_T("Invalid querystring name: %d.\r\n"), name);
    return EGL_BAD_PARAMETER;
  }

  return EGL_SUCCESS;
}

EGLint CDisplay::ChooseConfig(const EGLint *pAttrib_list, EGLConfig *pConfigs,
                              EGLint config_size, EGLint *pNum_config) {
  EGLint err;

  // Verify that the display was initialized
  if (!m_bInitialized) {
    __eglLogError(_T("The display was not initialized.\r\n"));
    return EGL_NOT_INITIALIZED;
  }

  if (NULL == pConfigs) {
    // Allow all config objects to be evaluated
    config_size = m_configs.GetSize();
  }

  int num_config = 0;

  for (CDisplay::ConfigList::Iter iter = m_configs.GetBegin(),
                                  iterEnd = m_configs.GetEnd();
       (iter != iterEnd) && (num_config < config_size);) {
    CConfig *const pConfig = *iter++;

    bool bResult;
    err = pConfig->Matches(pAttrib_list, &bResult);
    if (__eglFailed(err)) {
      __eglLogError(_T("CConfig::Matches() failed, err = %d.\r\n"), err);
      return err;
    }

    if (bResult) {
      if (pConfigs) {
        auto dwHandle = m_pHandles->FindHandle(pConfig, this);
        ASSERT(dwHandle);
        pConfigs[num_config] = reinterpret_cast<EGLConfig>(dwHandle);
      }

      ++num_config;
    }
  }

  // Update configs size
  *pNum_config = num_config;

  return EGL_SUCCESS;
}
