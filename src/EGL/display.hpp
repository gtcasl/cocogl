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
#include "config.hpp"
#include "context.hpp"
#include "surface.hpp"

class CConfig;

class CDisplay : public CObject {
public:
  typedef TList<CConfig *> ConfigList;

  static EGLint Create(CDisplay **ppDisplay, EGLNativeDisplayType hNative,
                       CHandleTable *pHandles);

  EGLint Initialize(EGLint *pMajor, EGLint *pMinor);

  EGLint QueryString(LPCSTR *plpValue, EGLint name);

  EGLint ChooseConfig(const EGLint *pAttrib_list, EGLConfig *pConfigs,
                      EGLint config_size, EGLint *pNum_config);

  EGLNativeDisplayType GetNativeHandle() const { return m_hNative; }

  bool IsInitialized() const { return m_bInitialized; }

private:
  CDisplay(EGLNativeDisplayType hDC, CHandleTable *pHandles);
  ~CDisplay();

  EGLint CreateConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha,
                      EGLint depth, EGLint stencil);

  std::mutex m_CS;
  CHandleTable *m_pHandles;
  EGLNativeDisplayType m_hNative;
  ConfigList m_configs;
  bool m_bInitialized;
};
