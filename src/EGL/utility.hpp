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

#ifndef COCOGL_API_PROFILE
#define __profileAPI(func, ...)
#else
#define __profileAPI(func, ...) AutoLog profiler(g_logger, func, __VA_ARGS__);
#endif

#ifndef NDEBUG
#define __eglLog(...) g_logger.write(__VA_ARGS__);

#define __eglLogError(...)                                                    \
  g_logger.write("*** Error in file %s at line %d.\r\n", __FILE__, __LINE__); \
  g_logger.write(__VA_ARGS__);                                                \
  assert(false);
#else
#define __eglLog(...)
#define __eglLogError(...)
#endif

#define __eglError(error, ...)  \
  __eglLogError(__VA_ARGS__);   \
  g_driver.setError(error);

inline EGLint EGLERROR_FROM_HRESULT(HRESULT hr) {
   if (SUCCEEDED(hr)) {
    return EGL_SUCCESS;
  } else if (E_INVALIDARG == hr) {
    return EGL_BAD_PARAMETER;
  }
  return EGL_BAD_ALLOC;
}