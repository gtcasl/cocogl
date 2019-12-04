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
#pragma once

class _EGLConfig;

class _EGLDisplay : public Object {
public:
  static EGLint Create(_EGLDisplay **ppDisplay, EGLNativeDisplayType hNative,
                       HandleTable *pHandles);

  EGLint initialize(EGLint *pMajor, EGLint *pMinor);

  EGLint queryString(const char **plpValue, EGLint name);

  EGLint chooseConfig(const EGLint *pAttrib_list, EGLConfig *pConfigs,
                      EGLint config_size, EGLint *pNum_config);

  auto getNativeHandle() const { return hNative_; }

  bool isInitialized() const { return bInitialized_; }

private:
  _EGLDisplay(EGLNativeDisplayType hDC, HandleTable *pHandles);
  ~_EGLDisplay();

  EGLint createConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha,
                      EGLint depth, EGLint stencil);

  HandleTable *handles_;
  EGLNativeDisplayType hNative_;
  std::list<_EGLConfig *> configs_;
  bool bInitialized_;
};
