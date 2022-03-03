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

#include <fixed.hpp>
#include <math.hpp>

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
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER \
  __pragma(warning(disable : 4100))
#define DISABLE_WARNING_UNREFERENCED_FUNCTION __pragma(warning(disable : 4505))
#define DISABLE_WARNING_ANONYMOUS_STRUCT __pragma(warning(disable : 4201))
#define DISABLE_WARNING_UNUSED_VARIABLE __pragma(warning(disable : 4189))
#elif defined(__GNUC__)
#define DISABLE_WARNING_PUSH _Pragma("GCC diagnostic push")
#define DISABLE_WARNING_POP _Pragma("GCC diagnostic pop")
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER \
  _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
#define DISABLE_WARNING_UNREFERENCED_FUNCTION \
  _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#define DISABLE_WARNING_ANONYMOUS_STRUCT \
  _Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#define DISABLE_WARNING_UNUSED_VARIABLE \
  _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"")
#elif defined(__clang__)
#define DISABLE_WARNING_PUSH _Pragma("clang diagnostic push")
#define DISABLE_WARNING_POP _Pragma("clang diagnostic pop")
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER \
  _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")
#define DISABLE_WARNING_UNREFERENCED_FUNCTION \
  _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#define DISABLE_WARNING_ANONYMOUS_STRUCT \
  _Pragma("clang diagnostic ignored \"-Wgnu-anonymous-struct\"")
#define DISABLE_WARNING_UNUSED_VARIABLE \
  _Pragma("clang diagnostic ignored \"-Wunused-but-set-variable\"")
#else
#define DISABLE_WARNING_PUSH
#define DISABLE_WARNING_POP
#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
#define DISABLE_WARNING_UNREFERENCED_FUNCTION
#define DISABLE_WARNING_ANONYMOUS_STRUCT
#endif

typedef cocogfx::TFixed<1>  fixed1;
typedef cocogfx::TFixed<2>  fixed2;
typedef cocogfx::TFixed<3>  fixed3;
typedef cocogfx::TFixed<4>  fixed4;
typedef cocogfx::TFixed<5>  fixed5;
typedef cocogfx::TFixed<6>  fixed6;
typedef cocogfx::TFixed<7>  fixed7;
typedef cocogfx::TFixed<8>  fixed8;
typedef cocogfx::TFixed<9>  fixed9;
typedef cocogfx::TFixed<10> fixed10;
typedef cocogfx::TFixed<11> fixed11;
typedef cocogfx::TFixed<12> fixed12;
typedef cocogfx::TFixed<13> fixed13;
typedef cocogfx::TFixed<14> fixed14;
typedef cocogfx::TFixed<15> fixed15;
typedef cocogfx::TFixed<16> fixed16;
typedef cocogfx::TFixed<17> fixed17;
typedef cocogfx::TFixed<18> fixed18;
typedef cocogfx::TFixed<19> fixed19;
typedef cocogfx::TFixed<20> fixed20;
typedef cocogfx::TFixed<21> fixed21;
typedef cocogfx::TFixed<22> fixed22;
typedef cocogfx::TFixed<23> fixed23;
typedef cocogfx::TFixed<24> fixed24;
typedef cocogfx::TFixed<25> fixed25;
typedef cocogfx::TFixed<26> fixed26;
typedef cocogfx::TFixed<27> fixed27;
typedef cocogfx::TFixed<28> fixed28;
typedef cocogfx::TFixed<29> fixed29;
typedef cocogfx::TFixed<30> fixed30;

#ifdef COCOGL_PIXEDPOINT

typedef cocogfx::TFixed<1>  float1;
typedef cocogfx::TFixed<2>  float2;
typedef cocogfx::TFixed<3>  float3;
typedef cocogfx::TFixed<4>  float4;
typedef cocogfx::TFixed<5>  float5;
typedef cocogfx::TFixed<6>  float6;
typedef cocogfx::TFixed<7>  float7;
typedef cocogfx::TFixed<8>  float8;
typedef cocogfx::TFixed<9>  float9;
typedef cocogfx::TFixed<10> float10;
typedef cocogfx::TFixed<11> float11;
typedef cocogfx::TFixed<12> float12;
typedef cocogfx::TFixed<13> float13;
typedef cocogfx::TFixed<14> float14;
typedef cocogfx::TFixed<15> float15;
typedef cocogfx::TFixed<16> float16;
typedef cocogfx::TFixed<17> float17;
typedef cocogfx::TFixed<18> float18;
typedef cocogfx::TFixed<19> float19;
typedef cocogfx::TFixed<20> float20;
typedef cocogfx::TFixed<21> float21;
typedef cocogfx::TFixed<22> float22;
typedef cocogfx::TFixed<23> float23;
typedef cocogfx::TFixed<24> float24;
typedef cocogfx::TFixed<25> float25;
typedef cocogfx::TFixed<26> float26;
typedef cocogfx::TFixed<27> float27;
typedef cocogfx::TFixed<28> float28;
typedef cocogfx::TFixed<29> float29;
typedef cocogfx::TFixed<30> float30;

#else

typedef float float1;
typedef float float2;
typedef float float3;
typedef float float4;
typedef float float5;
typedef float float6;
typedef float float7;
typedef float float8;
typedef float float9;
typedef float float10;
typedef float float11;
typedef float float12;
typedef float float13;
typedef float float14;
typedef float float15;
typedef float float16;
typedef float float17;
typedef float float18;
typedef float float19;
typedef float float20;
typedef float float21;
typedef float float22;
typedef float float23;
typedef float float24;
typedef float float25;
typedef float float26;
typedef float float27;
typedef float float28;
typedef float float29;
typedef float float30;

#endif

typedef float16 floatf;

typedef cocogfx::TRect<int32_t> Rect;

typedef cocogfx::TVector1<floatf> VECTOR1;
typedef cocogfx::TVector2<floatf> VECTOR2;
typedef cocogfx::TVector3<floatf> VECTOR3;
typedef cocogfx::TVector4<floatf> VECTOR4;
typedef cocogfx::TMatrix44<floatf> MATRIX44;