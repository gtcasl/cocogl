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

class _EGLDisplay;
class _EGLConfig;
class _EGLSurface;

class _EGLContext : public Object {
public:
  static EGLint Create(_EGLContext **ppContext, _EGLDisplay *pDisplay,
                       _EGLConfig *pConfig, _EGLContext *pCtxShared);

  EGLint getAttribute(EGLint name, EGLint *pValue);

  void setBindings(std::thread::id threadID, _EGLSurface *pSurfDraw,
                   _EGLSurface *pSurfRead);

  auto getDisplay() const { return pDisplay_; }

  auto getConfig() const { return pConfig_; }

  auto getThreadID() const { return threadID_; }

  auto getDrawSurface() const { return pSurfDraw_; }

  auto getReadSurface() const { return pSurfRead_; }

  auto getNativeData() const { return glContext_; }

  bool hasBindings() const { return (pSurfDraw_ || pSurfRead_); }

private:
  _EGLContext(_EGLDisplay *pDisplay, _EGLConfig *pConfig);
  ~_EGLContext();

  EGLint initialize(_EGLContext *pCtxShared);

  _EGLDisplay *pDisplay_;
  _EGLConfig *pConfig_;
  _EGLSurface *pSurfDraw_;
  _EGLSurface *pSurfRead_;
  std::thread::id threadID_;
  __GLContext glContext_;
};
