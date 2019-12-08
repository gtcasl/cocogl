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

_EGLDisplay::_EGLDisplay(EGLNativeDisplayType hNative, bool isDefault) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  hNative_ = hNative;
  isDefault = isDefault;
  isInitialized_ = false;  
}

_EGLDisplay::~_EGLDisplay() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  this->terminate();

  if (hNative_) {
#if defined(_WIN32)
    ReleaseDC(hNative_);
#elif defined(__linux__)
    XCloseDisplay(hNative_);
#endif
  }
}

EGLint _EGLDisplay::Create(_EGLDisplay **ppDisplay, EGLNativeDisplayType display_id) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(ppDisplay);

  bool isDefault = false;

  if (EGL_DEFAULT_DISPLAY == display_id) {
    // Set the default display ID
    isDefault = true;
#if defined(_WIN32)
    display_id = GetDC(nullptr);
#elif defined(__linux__)
    display_id = XOpenDisplay(nullptr);
#endif
  }

  // Create a new display object
  auto pDisplay = new _EGLDisplay(display_id, isDefault);
  if (nullptr == pDisplay) {
    __eglLogError("EGLDisplay allocation failed, out of memory");
    return EGL_BAD_ALLOC;
  }

  pDisplay->addRef();

  *ppDisplay = pDisplay;

  return EGL_SUCCESS;
}

EGLint _EGLDisplay::initialize(EGLint *pMajor, EGLint *pMinor) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;

  if (!isInitialized_) {
#if defined(COCOGL_RASTER_R5G6B5)
    err = this->createConfig(5, 6, 5, 0, 0, 0);
    if (__eglFailed(err)) {
      __eglLogError("createConfig() failed, err = %d.\r\n", err);
      return err;
    }

    err = this->createConfig(5, 6, 5, 0, 16, 0);
    if (__eglFailed(err)) {
      __eglLogError("createConfig() failed, err = %d.\r\n", err);
      return err;
    }

    err = this->createConfig(5, 6, 5, 0, 16, 8);
    if (__eglFailed(err)) {
      __eglLogError("createConfig() failed, err = %d.\r\n", err);
      return err;
    }
#elif defined(COCOGL_RASTER_A8R8G8B8)
    err = this->createConfig(8, 8, 8, 8, 0, 0);
    if (__eglFailed(err)) {
      __eglLogError("createConfig() failed, err = %d.\r\n", err);
      return err;
    }

    err = this->createConfig(8, 8, 8, 8, 16, 0);
    if (__eglFailed(err)) {
      __eglLogError("createConfig() failed, err = %d.\r\n", err);
      return err;
    }

    err = this->createConfig(8, 8, 8, 8, 16, 8);
    if (__eglFailed(err)) {
      __eglLogError("createConfig() failed, err = %d.\r\n", err);
      return err;
    }
#endif

    isInitialized_ = true;
  }

  if (pMajor) {
    *pMajor = EGL_VERSION_MAJOR;
  }

  if (pMinor) {
    *pMinor = EGL_VERSION_MINOR;
  }

  return EGL_SUCCESS;
}

EGLint _EGLDisplay::createConfig(EGLint red, EGLint green, EGLint blue,
                                 EGLint alpha, EGLint depth, EGLint stencil) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err;
  _EGLConfig *pConfig;

  // Create a new config object
  err = _EGLConfig::Create(&pConfig, red, green, blue, alpha, depth, stencil);
  if (__eglFailed(err)) {
    __eglLogError("_EGLConfig::Create() failed, err = %d.\r\n", err);
    return err;
  }

  // Find the insertion sort iterator
  auto iter = configs_.begin();
  for (auto iterEnd = configs_.end(); iter != iterEnd; ++iter) {
    int cmp = _EGLConfig::compare(*iter, pConfig);
    if (cmp >= 0) {
      break;
    }
  }

  // Insert the config object into sorted list
  configs_.insert(iter, pConfig);

  uint32_t handle;

  // Add the config object into handle table
  err = EGLERROR_FROM_HRESULT(
      handles_.insert(&handle, pConfig, HANDLE_CONFIG));
  if (__eglFailed(err)) {
    __eglLogError("HandleTable::insert() failed, err = %d.\r\n", err);
    __safeRelease(pConfig);
    return err;
  }

  // Update the config ID
  pConfig->setAttribute(EGL_CONFIG_ID, handle);

  return EGL_SUCCESS;
}

EGLint _EGLDisplay::queryString(const char **plpValue, EGLint name) {
  assert(plpValue);

  // Verify that the display was initialized
  if (!isInitialized_) {
    __eglLogError("The display was not initialized.\r\n");
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
    __eglLogError("Invalid querystring name: %d.\r\n", name);
    return EGL_BAD_PARAMETER;
  }

  return EGL_SUCCESS;
}

EGLint _EGLDisplay::chooseConfig(const EGLint *pAttrib_list,
                                 EGLConfig *pConfigs, 
                                 EGLint config_size,
                                 EGLint *pNum_config) {
  EGLint err;

  // Verify that the display was initialized
  if (!isInitialized_) {
    __eglLogError("The display was not initialized.\r\n");
    return EGL_NOT_INITIALIZED;
  }

  if (nullptr == pConfigs) {
    // Allow all config objects to be evaluated
    config_size = configs_.size();
  }

  int num_config = 0;
  for (auto pConfig : configs_) {
    if (num_config >= config_size)
      break;

    bool bResult;
    err = pConfig->matches(pAttrib_list, &bResult);
    if (__eglFailed(err)) {
      __eglLogError("_EGLConfig::matches() failed, err = %d.\r\n", err);
      return err;
    }

    if (bResult) {
      if (pConfigs) {
        auto handle = handles_.findHandle(pConfig);
        assert(handle);
        pConfigs[num_config] = reinterpret_cast<EGLConfig>(handle);
      }

      ++num_config;
    }
  }

  // Update configs size
  *pNum_config = num_config;

  return EGL_SUCCESS;
}

void _EGLDisplay::terminate() {
  if (isInitialized_) {
    auto enumerator = handles_.getEnumerator();
    while (!enumerator.isEnd()) {
      reinterpret_cast<IObject *>(enumerator.getObject())->release();
      enumerator.moveNext();
    }
    handles_.clear();
    isInitialized_ = false;
  }
}