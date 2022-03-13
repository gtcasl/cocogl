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

//#define FRAME_TRACE 1

#include <list>
#include <map>
#include <cstring>
#include <vector>
#include <thread>

#include <platform.hpp>
#include <utils.hpp>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <glcommon.hpp>

#include <color.hpp>
#include <fixed.hpp>
#include <format.hpp>
#include <uint24.hpp>
#include <math.hpp>

#include <handle.hpp>
#include <logger.hpp>
#include <object.hpp>

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

typedef cocogfx::TRect<int32_t>     Rect;
typedef cocogfx::TVector1<floatf>   VECTOR1;
typedef cocogfx::TVector2<floatf>   VECTOR2;
typedef cocogfx::TVector3<floatf>   VECTOR3;
typedef cocogfx::TVector4<floatf>   VECTOR4;
typedef cocogfx::TMatrix44<floatf>  MATRIX44;

#define FORMAT_A      FORMAT_A8
#define FORMAT_RGB    FORMAT_R5G6B5
#define FORMAT_ARGB   FORMAT_A8R8G8B8

using namespace cocogfx;

#include "types.hpp"
#include "utility.hpp"

extern Logger g_logger;
