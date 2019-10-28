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

inline bool __eglFailed(EGLint err) { return err != EGL_SUCCESS; }

inline bool __eglSucceeded(EGLint err) { return err == EGL_SUCCESS; }

#ifndef NDEBUG
#define __eglLog(...) CLogger::Instance().Write(__VA_ARGS__);

#define __eglLogError(...)                                              \
  CLogger::Instance().Write(_T("*** Error in file %s at line %d.\r\n"), \
                            _T(__FILE__), __LINE__);                    \
  CLogger::Instance().Write(__VA_ARGS__);                               \
  ASSERT(false);
#else
#define __eglLog(...)
#define __eglLogError(...)
#endif

#define __eglError(error, ...)                                          \
  __eglLogError(__VA_ARGS__);                                           \
  g_dll.driver()->SetError(error);

inline bool __glFailed(GLenum err) { return err != GL_NO_ERROR; }

inline bool __glSucceeded(GLenum err) { return err == GL_NO_ERROR; }

EGLint EGLERROR_FROM_HRESULT(HRESULT hr);

EGLint EGLERROR_FROM_GLERROR(GLenum glErr);
