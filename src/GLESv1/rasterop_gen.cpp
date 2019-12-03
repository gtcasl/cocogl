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
#include "stdafx.h"
#include "raster.hpp"
#include "rasterop.hpp"
#include "rasterop_gen.hpp"
#include "rasterop_gen.inl"

#define MAKE_SCANLINE(fog, texture1, texture0, color, depth)                   \
  {                                                                            \
    TGenericScanlineA<depth, color, texture0, texture1, fog>::Execute,         \
        TGenericScanlineP<depth, color, texture0, texture1, fog>::Execute      \
  }

static const struct ScanlineRasterID {
  PfnScanline pfnAffine;
  PfnScanline pfnPerspective;
}

l_genericScanlines[] = {
    MAKE_SCANLINE(false, 0, 0, false, false),
    MAKE_SCANLINE(false, 0, 0, false, true),
    MAKE_SCANLINE(false, 0, 0, true, false),
    MAKE_SCANLINE(false, 0, 0, true, true),
    MAKE_SCANLINE(false, 0, 1, false, false),
    MAKE_SCANLINE(false, 0, 1, false, true),
    MAKE_SCANLINE(false, 0, 1, true, false),
    MAKE_SCANLINE(false, 0, 1, true, true),
    MAKE_SCANLINE(false, 0, 2, false, false),
    MAKE_SCANLINE(false, 0, 2, false, true),
    MAKE_SCANLINE(false, 0, 2, true, false),
    MAKE_SCANLINE(false, 0, 2, true, true),
    MAKE_SCANLINE(false, 1, 0, false, false),
    MAKE_SCANLINE(false, 1, 0, false, true),
    MAKE_SCANLINE(false, 1, 0, true, false),
    MAKE_SCANLINE(false, 1, 0, true, true),
    MAKE_SCANLINE(false, 1, 1, false, false),
    MAKE_SCANLINE(false, 1, 1, false, true),
    MAKE_SCANLINE(false, 1, 1, true, false),
    MAKE_SCANLINE(false, 1, 1, true, true),
    MAKE_SCANLINE(false, 1, 2, false, false),
    MAKE_SCANLINE(false, 1, 2, false, true),
    MAKE_SCANLINE(false, 1, 2, true, false),
    MAKE_SCANLINE(false, 1, 2, true, true),
    MAKE_SCANLINE(false, 2, 0, false, false),
    MAKE_SCANLINE(false, 2, 0, false, true),
    MAKE_SCANLINE(false, 2, 0, true, false),
    MAKE_SCANLINE(false, 2, 0, true, true),
    MAKE_SCANLINE(false, 2, 1, false, false),
    MAKE_SCANLINE(false, 2, 1, false, true),
    MAKE_SCANLINE(false, 2, 1, true, false),
    MAKE_SCANLINE(false, 2, 1, true, true),
    MAKE_SCANLINE(false, 2, 2, false, false),
    MAKE_SCANLINE(false, 2, 2, false, true),
    MAKE_SCANLINE(false, 2, 2, true, false),
    MAKE_SCANLINE(false, 2, 2, true, true),
    MAKE_SCANLINE(true, 0, 0, false, false),
    MAKE_SCANLINE(true, 0, 0, false, true),
    MAKE_SCANLINE(true, 0, 0, true, false),
    MAKE_SCANLINE(true, 0, 0, true, true),
    MAKE_SCANLINE(true, 0, 1, false, false),
    MAKE_SCANLINE(true, 0, 1, false, true),
    MAKE_SCANLINE(true, 0, 1, true, false),
    MAKE_SCANLINE(true, 0, 1, true, true),
    MAKE_SCANLINE(true, 0, 2, false, false),
    MAKE_SCANLINE(true, 0, 2, false, true),
    MAKE_SCANLINE(true, 0, 2, true, false),
    MAKE_SCANLINE(true, 0, 2, true, true),
    MAKE_SCANLINE(true, 1, 0, false, false),
    MAKE_SCANLINE(true, 1, 0, false, true),
    MAKE_SCANLINE(true, 1, 0, true, false),
    MAKE_SCANLINE(true, 1, 0, true, true),
    MAKE_SCANLINE(true, 1, 1, false, false),
    MAKE_SCANLINE(true, 1, 1, false, true),
    MAKE_SCANLINE(true, 1, 1, true, false),
    MAKE_SCANLINE(true, 1, 1, true, true),
    MAKE_SCANLINE(true, 1, 2, false, false),
    MAKE_SCANLINE(true, 1, 2, false, true),
    MAKE_SCANLINE(true, 1, 2, true, false),
    MAKE_SCANLINE(true, 1, 2, true, true),
    MAKE_SCANLINE(true, 2, 0, false, false),
    MAKE_SCANLINE(true, 2, 0, false, true),
    MAKE_SCANLINE(true, 2, 0, true, false),
    MAKE_SCANLINE(true, 2, 0, true, true),
    MAKE_SCANLINE(true, 2, 1, false, false),
    MAKE_SCANLINE(true, 2, 1, false, true),
    MAKE_SCANLINE(true, 2, 1, true, false),
    MAKE_SCANLINE(true, 2, 1, true, true),
    MAKE_SCANLINE(true, 2, 2, false, false),
    MAKE_SCANLINE(true, 2, 2, false, true),
    MAKE_SCANLINE(true, 2, 2, true, false),
    MAKE_SCANLINE(true, 2, 2, true, true),
};

static const PfnGetTexelColor l_pfnGetTexelColorTable[] = {
    GetTexelColorPt<FORMAT_A8, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_A8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorPt<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorPt<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,

    //--
    GetTexelColorLn<FORMAT_A8, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_A8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    GetTexelColorLn<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    GetTexelColorLn<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,
};

static const Pfncompare s_pfncompare[] = {
    DoCompare<COMPARE_NEVER>,   DoCompare<COMPARE_LESS>,
    DoCompare<COMPARE_EQUAL>,   DoCompare<COMPARE_LEQUAL>,
    DoCompare<COMPARE_GREATER>, DoCompare<COMPARE_NOTEQUAL>,
    DoCompare<COMPARE_GEQUAL>,  DoCompare<COMPARE_ALWAYS>,
};

static const PfnGetTexEnvColor s_pfnGetTexEnvColorTable[] = {
    GetTexEnvColorA<ENVMODE_ADD>,        GetTexEnvColorA<ENVMODE_BLEND>,
    GetTexEnvColorA<ENVMODE_REPLACE>,    GetTexEnvColorA<ENVMODE_MODULATE>,
    GetTexEnvColorA<ENVMODE_DECAL>,

    GetTexEnvColorRGB<ENVMODE_ADD>,      GetTexEnvColorRGB<ENVMODE_BLEND>,
    GetTexEnvColorRGB<ENVMODE_REPLACE>,  GetTexEnvColorRGB<ENVMODE_MODULATE>,
    GetTexEnvColorRGB<ENVMODE_DECAL>,

    GetTexEnvColorARGB<ENVMODE_ADD>,     GetTexEnvColorARGB<ENVMODE_BLEND>,
    GetTexEnvColorARGB<ENVMODE_REPLACE>, GetTexEnvColorARGB<ENVMODE_MODULATE>,
    GetTexEnvColorARGB<ENVMODE_DECAL>,
};

static const PfnBlend s_pfnBlendTable[] = {
#if defined(COCOGL_RASTER_R5G6B5)
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_DST_ALPHA>,
#elif defined(COCOGL_RASTER_A8R8G8B8)
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA,
           BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA,
           BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR,
           BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE,
           BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA,
           BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA,
           BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR,
           BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE,
           BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA,
           BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA,
           BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR,
           BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE,
           BLEND_ONE_MINUS_DST_ALPHA>,
#endif
};

static const PfnWriteColor s_pfnWriteColorTable[] = {
#if defined(COCOGL_RASTER_R5G6B5)
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_CLEAR>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_AND>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_AND_REVERSE>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_COPY>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_AND_INVERTED>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_NOOP>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_XOR>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_OR>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_NOR>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_EQUIV>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_INVERT>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_OR_REVERSE>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_COPY_INVERTED>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_OR_INVERTED>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_NAND>,
    DoWriteColor<FORMAT_R5G6B5, false, LOGICOP_SET>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_CLEAR>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_AND>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_AND_REVERSE>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_COPY>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_AND_INVERTED>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_NOOP>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_XOR>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_OR>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_NOR>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_EQUIV>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_INVERT>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_OR_REVERSE>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_COPY_INVERTED>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_OR_INVERTED>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_NAND>,
    DoWriteColor<FORMAT_R5G6B5, true, LOGICOP_SET>,
#elif defined(COCOGL_RASTER_A8R8G8B8)
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_CLEAR>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_AND>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_AND_REVERSE>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_COPY>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_AND_INVERTED>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_NOOP>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_XOR>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_OR>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_NOR>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_EQUIV>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_INVERT>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_OR_REVERSE>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_COPY_INVERTED>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_OR_INVERTED>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_NAND>,
    DoWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_SET>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_CLEAR>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_AND>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_AND_REVERSE>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_COPY>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_AND_INVERTED>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_NOOP>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_XOR>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_OR>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_NOR>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_EQUIV>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_INVERT>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_OR_REVERSE>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_COPY_INVERTED>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_OR_INVERTED>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_NAND>,
    DoWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_SET>,
#endif
};

//////////////////////////////////////////////////////////////////////////////

GenericRasterOp::GenericRasterOp(const RASTERID &rasterID) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  rasterID_ = rasterID;
  colorStride_ = 0;
  depthStencilStride_ = 0;
  pfnAlphaTest_ = nullptr;
  pfnStencilTest_ = nullptr;
  pfnDepthTest_ = nullptr;
  pfnBlend_ = nullptr;
  pfnWriteColor_ = nullptr;
  pfnScanline_ = nullptr;
}

GenericRasterOp::~GenericRasterOp() { __profileAPI(" - %s()\n", __FUNCTION__); }

GLenum GenericRasterOp::Create(IRasterOp **ppRasterOp,
                               const RASTERID &rasterID) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  assert(ppRasterOp);

  // Create a new rasterOp object
  auto pRasterOp = new GenericRasterOp(rasterID);
  if (nullptr == pRasterOp) {
    __glLogError("GenericRasterOp allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  pRasterOp->addRef();

  // Initialize the rasterOp
  err = pRasterOp->initialize();
  if (__glFailed(err)) {
    pRasterOp->release();
    __glLogError("GenericRasterOp::initialize() failed, err = %d.\r\n", err);
    return err;
  }

  *ppRasterOp = pRasterOp;

  return GL_NO_ERROR;
}

GLenum GenericRasterOp::initialize() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  RASTERFLAGS rasterFlags = rasterID_.Flags;

  colorStride_ = Format::GetInfo(rasterFlags.ColorFormat).BytePerPixel;

  // Select depth function
  if (rasterFlags.DepthTest || rasterFlags.StencilTest) {
    this->selectDepthStencilFunc();
  } else {
    depthStencilStride_ = 0;
  }

  // Select sampler function
  if (rasterFlags.NumTextures) {
    this->selectSamplerFunc();
    this->selectTexEnvFunc();
  }

  // Select alpha test function
  if (rasterFlags.AlphaTest) {
    this->selectAlphaTestFunc();
  }

  if (rasterFlags.Blend) {
    // Select blend function
    this->selectBlendFunc();
  }

  // Select write color function
  this->selectWriteColorFunc();

  bool bTexMip0 = ((rasterFlags.TextureMips >> 0) & 0x1);
  bool bTexMip1 = ((rasterFlags.TextureMips >> 1) & 0x1);

  uint32_t Depth = rasterFlags.DepthTest ? 1 : 0;
  uint32_t Color = rasterFlags.Color ? 1 : 0;
  uint32_t Tex0 = (rasterFlags.NumTextures > 0) ? (bTexMip0 ? 2 : 1) : 0;
  uint32_t Tex1 = (rasterFlags.NumTextures > 1) ? (bTexMip1 ? 2 : 1) : 0;
  uint32_t Fog = rasterFlags.Fog ? 1 : 0;

  uint32_t index = 1 * Depth + 2 * Color + 4 * Tex0 + 12 * Tex1 + 36 * Fog;

  assert(index < __countof(l_genericScanlines));
  pfnScanline_ = (rasterFlags.Perspective)
                     ? l_genericScanlines[index].pfnPerspective
                     : l_genericScanlines[index].pfnAffine;

  return GL_NO_ERROR;
}

void GenericRasterOp::selectDepthStencilFunc() {
  RASTERFLAGS rasterFlags = rasterID_.Flags;
  RASTERSTATES rasterStates = rasterID_.States;

  if (rasterFlags.DepthTest) {
    uint32_t index = rasterStates.DepthFunc;
    assert(index < __countof(s_pfncompare));
    pfnDepthTest_ = s_pfncompare[index];
  } else {
    pfnDepthTest_ = DoCompare<COMPARE_ALWAYS>;
  }

  depthStencilStride_ =
      Format::GetInfo(rasterFlags.DepthStencilFormat).BytePerPixel;

  if (rasterFlags.StencilTest) {
    uint32_t index = rasterStates.StencilFunc;
    assert(index < __countof(s_pfncompare));
    pfnStencilTest_ = s_pfncompare[index];
  }
}

void GenericRasterOp::selectSamplerFunc() {
  RASTERFLAGS rasterFlags = rasterID_.Flags;
  for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
    TEXTURESTATES texState = rasterID_.Textures[i];

    uint32_t baseIndex = 1 * texState.AddressV + 2 * texState.AddressU +
                         4 * (texState.Format - 1);

    uint32_t minIndex = baseIndex + 20 * (texState.MinFilter - 1);

    assert(minIndex < __countof(l_pfnGetTexelColorTable));
    samplers_[i].pfnGetTexelColorMin = l_pfnGetTexelColorTable[minIndex];

    uint32_t magIndex = baseIndex + 20 * (texState.MagFilter - 1);

    assert(magIndex < __countof(l_pfnGetTexelColorTable));
    samplers_[i].pfnGetTexelColorMag = l_pfnGetTexelColorTable[magIndex];
  }
}

void GenericRasterOp::selectTexEnvFunc() {
  RASTERFLAGS rasterFlags = rasterID_.Flags;
  for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
    uint32_t index;

    switch (rasterID_.Textures[i].Format) {
    default:
      __no_default;

    case FORMAT_A8:
      index = 0;
      break;

    case FORMAT_L8:
    case FORMAT_R5G6B5:
    case FORMAT_R8G8B8:
      index = 1;
      break;

    case FORMAT_A8L8:
    case FORMAT_A1R5G5B5:
    case FORMAT_A4R4G4B4:
    case FORMAT_A8R8G8B8:
      index = 2;
      break;
    }

    index = rasterID_.Textures[i].EnvMode + index * ENVMODE_SIZE_;
    assert(index < __countof(s_pfnGetTexEnvColorTable));
    samplers_[i].pfnGetTexEnvColor = s_pfnGetTexEnvColorTable[index];
  }
}

void GenericRasterOp::selectAlphaTestFunc() {
  RASTERSTATES rasterStates = rasterID_.States;

  uint32_t index = rasterStates.AlphaFunc;
  assert(index < __countof(s_pfncompare));
  pfnAlphaTest_ = s_pfncompare[index];
}

void GenericRasterOp::selectBlendFunc() {
  RASTERSTATES rasterStates = rasterID_.States;
  uint32_t index = rasterStates.BlendDst * BLEND_SIZE_ + rasterStates.BlendSrc;

  assert(index < __countof(s_pfnBlendTable));
  pfnBlend_ = s_pfnBlendTable[index];
}

void GenericRasterOp::selectWriteColorFunc() {
  RASTERFLAGS rasterFlags = rasterID_.Flags;
  RASTERSTATES rasterStates = rasterID_.States;

  bool bColorWriteMask = rasterFlags.ColorWriteMask;
  uint32_t logicFunc =
      (rasterFlags.LogicOp ? rasterStates.LogicFunc
                           : static_cast<uint32_t>(LOGICOP_COPY));
  uint32_t index = 1 * logicFunc + LOGICOP_SIZE_ * bColorWriteMask;

  assert(index < __countof(s_pfnWriteColorTable));
  pfnWriteColor_ = s_pfnWriteColorTable[index];
}

bool GenericRasterOp::doStencilTest(const RasterData &rasterData,
                                    uint32_t depthValue,
                                    void *pDSBuffer) const {
  assert(pDSBuffer);

  auto pRasterOp =
      reinterpret_cast<const GenericRasterOp *>(rasterData.pRasterOp);

  const RASTERID &rasterID = pRasterOp->getRasterID();
  RASTERFLAGS rasterFlags = rasterID.Flags;
  RASTERSTATES rasterStates = rasterID.States;

  uint32_t depthStencilValue = *reinterpret_cast<uint32_t *>(pDSBuffer);
  uint32_t stencilValue = depthStencilValue >> 16;
  uint32_t _depthValue = depthStencilValue & 0xffff;
  uint32_t stencilRef = rasterData.StencilRef;
  uint32_t _stencilValue = stencilValue & rasterData.StencilMask;
  uint32_t _stencilRef = stencilRef & rasterData.StencilMask;

  eStencilOp stencilOp;

  uint32_t writeMask = rasterData.StencilWriteMask << 16;

  bool bStencilTest = (pRasterOp->pfnStencilTest_)(_stencilRef, _stencilValue);
  if (bStencilTest) {
    if ((pRasterOp->pfnDepthTest_)(depthValue, _depthValue)) {
      stencilOp = static_cast<eStencilOp>(rasterStates.StencilZPass);

      if (rasterFlags.DepthWrite) {
        writeMask |= 0xffff;
      }
    } else {
      stencilOp = static_cast<eStencilOp>(rasterStates.StencilZFail);
      bStencilTest = false;
    }
  } else {
    stencilOp = static_cast<eStencilOp>(rasterStates.StencilFail);
  }

  uint32_t result;

  switch (stencilOp) {
  default:
    __no_default;

  case STENCIL_KEEP:
    result = stencilValue;
    break;

  case STENCIL_REPLACE:
    result = stencilRef;
    break;

  case STENCIL_INCR:
    result = (stencilValue < 255) ? (stencilValue + 1) : stencilValue;
    break;

  case STENCIL_DECR:
    result = (stencilValue > 0) ? (stencilValue - 1) : stencilValue;
    break;

  case STENCIL_ZERO:
    result = 0;
    break;

  case STENCIL_INVERT:
    result = ~stencilValue;
    break;
  }

  // Write the depth stencil value
  uint32_t value = (result << 16) | depthValue;
  *reinterpret_cast<uint32_t *>(pDSBuffer) =
      (depthStencilValue & ~writeMask) | (value & writeMask);

  return bStencilTest;
}

void GenericRasterOp::getSamplerColor(Color4 *pOut, uint32_t unit,
                                      const RasterData &rasterData, fixedRX fU,
                                      fixedRX fV, fixedRX fM) const {
  const Sampler &sampler = rasterData.Samplers[unit];

  switch (rasterID_.Textures[unit].MipFilter) {
  default:
    __no_default;
  case FILTER_NONE: {
    auto fJ = fixed16::make(fM.data());
    if (fJ > TConst<fixed16>::One()) {
      (samplers_[unit].pfnGetTexelColorMin)(pOut, sampler.pMipLevels[0], fU,
                                            fV);
    } else {
      (samplers_[unit].pfnGetTexelColorMag)(pOut, sampler.pMipLevels[0], fU,
                                            fV);
    }

    break;
  }
  case FILTER_NEAREST: {
    auto fJ = fixed16::make(fM.data());
    if (fJ > TConst<fixed16>::One()) {
      int mipLevel = std::min<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                     sampler.MaxMipLevel);

      (samplers_[unit].pfnGetTexelColorMin)(pOut, sampler.pMipLevels[mipLevel],
                                            fU, fV);
    } else {
      (samplers_[unit].pfnGetTexelColorMag)(pOut, sampler.pMipLevels[0], fU,
                                            fV);
    }

    break;
  }

  case FILTER_LINEAR: {
    auto fJ = fixed16::make(fM.data());
    if (fJ > TConst<fixed16>::One()) {
      Color4 tmp;

      int mipLevel0 = std::min<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                      sampler.MaxMipLevel);

      int mipLevel1 = std::min<int>(mipLevel0 + 1, sampler.MaxMipLevel);
      int mipLerp = (fJ.data() >> (mipLevel0 + 8)) - fixed8::ONE;

      (samplers_[unit].pfnGetTexelColorMin)(pOut, sampler.pMipLevels[mipLevel0],
                                            fU, fV);
      (samplers_[unit].pfnGetTexelColorMin)(&tmp, sampler.pMipLevels[mipLevel1],
                                            fU, fV);

      pOut->r = Math::Lerp8(pOut->r, tmp.r, mipLerp);
      pOut->g = Math::Lerp8(pOut->g, tmp.g, mipLerp);
      pOut->b = Math::Lerp8(pOut->b, tmp.b, mipLerp);
      pOut->a = Math::Lerp8(pOut->a, tmp.a, mipLerp);
    } else {
      (samplers_[unit].pfnGetTexelColorMag)(pOut, sampler.pMipLevels[0], fU,
                                            fV);
    }

    break;
  }
  }
}

void GenericRasterOp::applyFog(const RasterData &rasterData, Color4 *pInOut,
                               fixedRX fFactor) {
  const Color4 &cFogColor = rasterData.cFogColor;
  int factor = fFactor.data();

  pInOut->r =
      cFogColor.r + ((factor * (pInOut->r - cFogColor.r)) >> fixedRX::FRAC);
  pInOut->g =
      cFogColor.g + ((factor * (pInOut->g - cFogColor.g)) >> fixedRX::FRAC);
  pInOut->b =
      cFogColor.b + ((factor * (pInOut->b - cFogColor.b)) >> fixedRX::FRAC);
}