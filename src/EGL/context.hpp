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

  CDisplay *GetDisplay() const { return m_pDisplay; }

  CConfig *GetConfig() const { return m_pConfig; }

  auto GetThreadID() const { return m_dwThreadID; }

  CEGLSurface *GetDrawSurface() const { return m_pSurfDraw; }

  CEGLSurface *GetReadSurface() const { return m_pSurfRead; }

  __GLContext GetNativeData() const { return m_glContext; }

  bool HasBindings() const { return (m_pSurfDraw || m_pSurfRead); }

private:
  CEGLContext(CDisplay *pDisplay, CConfig *pConfig);
  ~CEGLContext();

  EGLint Initialize(CEGLContext *pCtxShared);

  CDisplay *m_pDisplay;
  CConfig *m_pConfig;
  CEGLSurface *m_pSurfDraw;
  CEGLSurface *m_pSurfRead;
  std::thread::id m_dwThreadID;
  __GLContext m_glContext;
};
