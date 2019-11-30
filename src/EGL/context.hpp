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
class CDisplay;
class CConfig;
class CEGLSurface;

class CEGLContext : public CObject {
public:
  static EGLint Create(CEGLContext **ppContext, CDisplay *pDisplay,
                       CConfig *pConfig, CEGLContext *pCtxShared);

  EGLint GetAttribute(EGLint name, EGLint *pValue);

  void SetBindings(std::thread::id dwThreadID, CEGLSurface *pSurfDraw,
                   CEGLSurface *pSurfRead);

  CDisplay *GetDisplay() const { return pDisplay_; }

  CConfig *GetConfig() const { return pConfig_; }

  auto GetThreadID() const { return dwThreadID_; }

  CEGLSurface *GetDrawSurface() const { return pSurfDraw_; }

  CEGLSurface *GetReadSurface() const { return pSurfRead_; }

  __GLContext GetNativeData() const { return glContext_; }

  bool HasBindings() const { return (pSurfDraw_ || pSurfRead_); }

private:
  CEGLContext(CDisplay *pDisplay, CConfig *pConfig);
  ~CEGLContext();

  EGLint Initialize(CEGLContext *pCtxShared);

  CDisplay *pDisplay_;
  CConfig *pConfig_;
  CEGLSurface *pSurfDraw_;
  CEGLSurface *pSurfRead_;
  std::thread::id dwThreadID_;
  __GLContext glContext_;
};
