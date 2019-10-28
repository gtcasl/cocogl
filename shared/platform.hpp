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

#include <stdio.h>
#include <stdint.h>
#include <type_traits>
#include <assert.h>

#if defined(_WIN32)

#include <windows.h>
#include <tchar.h>

HDC GetDefaultDisplay();

void ReleaseDisplay(HDC display);

void DestroyPixmap(HBITMAP pixmap);

#elif defined(__linux__)

#include <unistd.h>
#include <X11/Xlib.h>

Display* GetDefaultDisplay();

void ReleaseDisplay(Display* display);

void DestroyPixmap(Pixmap pixmap);

#define _T(x) x
typedef char TCHAR;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef char *LPTSTR;
typedef const char *LPCTSTR;
#define vftprintf vfprintf
#define _tfopen   fopen
#define _tprintf  printf
#define _ftprintf fprintf
#define _vftprintf vfprintf

#ifndef ASSERT
#ifndef NDEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif
#endif

#define HRESULT         int
#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#define FAILED(hr)      (((HRESULT)(hr)) < 0)

#define S_OK            0x00000000
#define S_FALSE         0x00000001
#define E_FAIL          0x80004005
#define E_INVALIDARG    0x80070057
#define E_NOTIMPL       0x80004001
#define E_OUTOFMEMORY   0x8007000E

#define __retOnFail(x)  if (FAILED(hr = (x))) return hr;
#define __exitOnFail(x) if (FAILED(hr = (x))) goto L_EXIT;
#define __exitOnOOM(x)  if (NULL == (x)) { hr = E_OUTOFMEMORY; goto L_EXIT; }

#endif // __linux__

#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH                              __pragma(warning(push))
    #define DISABLE_WARNING_POP                               __pragma(warning(pop)) 
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER     __pragma(warning(disable: 4100))
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION             __pragma(warning(disable: 4505))    
    #define DISABLE_WARNING_ANONYMOUS_STRUCT                  __pragma(warning(disable: 4201)) 
#elif defined(__GNUC__)
    #define DISABLE_WARNING_PUSH                              _Pragma("GCC diagnostic push")
    #define DISABLE_WARNING_POP                               _Pragma("GCC diagnostic pop")    
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER     _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION             _Pragma("GCC diagnostic ignored \"-Wunused-function\"")    
    #define DISABLE_WARNING_ANONYMOUS_STRUCT                  _Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#elif defined(__clang__)
    #define DISABLE_WARNING_PUSH                              _Pragma("clang diagnostic push")
    #define DISABLE_WARNING_POP                               _Pragma("clang diagnostic pop")
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER     _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION             _Pragma("clang diagnostic ignored \"-Wunused-function\"")
    #define DISABLE_WARNING_ANONYMOUS_STRUCT                  _Pragma("clang diagnostic ignored \"-Wgnu-anonymous-struct\"")
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION
    #define DISABLE_WARNING_ANONYMOUS_STRUCT
#endif

#define DISABLE_WARNING \
#define IGNORE_ANONYMOUS_STRUCT \
  #pragma warning(push) \
  #pragma warning(disable : 4201)

#define RESTORE_WARNING \
  #pragma warning(pop) \

#ifndef NDEBUG
#define __debugMsg(level, ...) DbgPrintf(level, __VA_ARGS__);
#else
#define __debugMsg(...)
#endif

#ifdef NDEBUG
#define __no_default ASSERT(0)
#else
#define __no_default ASSERT(false);
#endif

template <typename... Args>
void __unreferenced(Args&&...) {}