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
// installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR
// INDEMNITIES.
//
#pragma once

#define GL_GLEXT_PROTOTYPES

#define COCOGL_RASTER_A8R8G8B8
//#define COCOGL_RASTER_R5G6B5

//#define COCOGL_PIXEDPOINT
//#define COCOGL_RASTER_PROFILE
//#define COCOGL_API_TRACE
//#define COCOGL_SNAPSHOTS 1

#include <assert.h>
#include <functional>
#include <stdint.h>
#include <stdio.h>
#include <type_traits>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#include <unistd.h>

#define HRESULT uint32_t
#define SUCCEEDED(hr) 0 == ((hr) >> 31)
#define FAILED(hr) 0 != ((hr) >> 31)

#define S_OK 0x00000000
#define S_FALSE 0x00000001
#define E_FAIL 0x80004005
#define E_INVALIDARG 0x80070057
#define E_NOTIMPL 0x80004001
#define E_OUTOFMEMORY 0x8007000E

#define __retOnFail(x)  \
  if (FAILED(hr = (x))) \
    return hr;
#define __exitOnFail(x) \
  if (FAILED(hr = (x))) \
    goto L_EXIT;
#define __exitOnOOM(x)  \
  if (nullptr == (x)) { \
    hr = E_OUTOFMEMORY; \
    goto L_EXIT;        \
  }
#endif

#if defined(_MSC_VER)
#define DISABLE_WARNING_PUSH __pragma(warning(push))
#define DISABLE_WARNING_POP __pragma(warning(pop))
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER __pragma(warning(disable : 4100))
#define DISABLE_WARNING_UNREFERENCED_FUNCTION __pragma(warning(disable : 4505))
#define DISABLE_WARNING_ANONYMOUS_STRUCT __pragma(warning(disable : 4201))
#define DISABLE_WARNING_UNUSED_VARIABLE __pragma(warning(disable : 4189))
#define DISABLE_WARNING_ARRAY_BOUNDS __pragma(warning(disable : 6385))
#elif defined(__GNUC__)
#define DISABLE_WARNING_PUSH _Pragma("GCC diagnostic push")
#define DISABLE_WARNING_POP _Pragma("GCC diagnostic pop")
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
#define DISABLE_WARNING_UNREFERENCED_FUNCTION _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#define DISABLE_WARNING_ANONYMOUS_STRUCT _Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#define DISABLE_WARNING_UNUSED_VARIABLE _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"")
#define DISABLE_WARNING_ARRAY_BOUNDS _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#elif defined(__clang__)
#define DISABLE_WARNING_PUSH _Pragma("clang diagnostic push")
#define DISABLE_WARNING_POP _Pragma("clang diagnostic pop")
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")
#define DISABLE_WARNING_UNREFERENCED_FUNCTION _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#define DISABLE_WARNING_ANONYMOUS_STRUCT _Pragma("clang diagnostic ignored \"-Wgnu-anonymous-struct\"")
#define DISABLE_WARNING_UNUSED_VARIABLE _Pragma("clang diagnostic ignored \"-Wunused-but-set-variable\"")
#define DISABLE_WARNING_ARRAY_BOUNDS _Pragma("clang diagnostic ignored \"-Warray-bounds\"")
#else
#define DISABLE_WARNING_PUSH
#define DISABLE_WARNING_POP
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
#define DISABLE_WARNING_UNREFERENCED_FUNCTION
#define DISABLE_WARNING_ANONYMOUS_STRUCT
#endif