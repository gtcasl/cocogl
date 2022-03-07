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

#define MAKE_SCANLINE(fog, texture1, texture0, color, depth)             \
  {                                                                      \
    GenericScanlineA<depth, color, texture0, texture1, fog>::Execute,    \
        GenericScanlineP<depth, color, texture0, texture1, fog>::Execute \
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
    DoCompare<COMPARE_NEVER>,
    DoCompare<COMPARE_LESS>,
    DoCompare<COMPARE_EQUAL>,
    DoCompare<COMPARE_LEQUAL>,
    DoCompare<COMPARE_GREATER>,
    DoCompare<COMPARE_NOTEQUAL>,
    DoCompare<COMPARE_GEQUAL>,
    DoCompare<COMPARE_ALWAYS>,
};

static const PfnGetTexEnvColor s_pfnGetTexEnvColorTable[] = {
    GetTexEnvColorA<ENVMODE_ADD>,
    GetTexEnvColorA<ENVMODE_BLEND>,
    GetTexEnvColorA<ENVMODE_REPLACE>,
    GetTexEnvColorA<ENVMODE_MODULATE>,
    GetTexEnvColorA<ENVMODE_DECAL>,

    GetTexEnvColorRGB<ENVMODE_ADD>,
    GetTexEnvColorRGB<ENVMODE_BLEND>,
    GetTexEnvColorRGB<ENVMODE_REPLACE>,
    GetTexEnvColorRGB<ENVMODE_MODULATE>,
    GetTexEnvColorRGB<ENVMODE_DECAL>,

    GetTexEnvColorARGB<ENVMODE_ADD>,
    GetTexEnvColorARGB<ENVMODE_BLEND>,
    GetTexEnvColorARGB<ENVMODE_REPLACE>,
    GetTexEnvColorARGB<ENVMODE_MODULATE>,
    GetTexEnvColorARGB<ENVMODE_DECAL>,
};

static const PfnBlend s_pfnBlendTable[] = {
#if defined(COCOGL_RASTER_R5G6B5)
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_R5G6B5, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_DST_ALPHA>,
#elif defined(COCOGL_RASTER_A8R8G8B8)
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA,
            BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA,
            BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR,
            BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE,
            BLEND_ONE_MINUS_SRC_COLOR>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA,
            BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA,
            BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR,
            BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE,
            BLEND_ONE_MINUS_SRC_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_ZERO, BLEND_ZERO>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA,
            BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA,
            BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR,
            BLEND_ONE_MINUS_DST_ALPHA>,
    Blender<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE,
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

GenericRasterOp::~GenericRasterOp() {
  __profileAPI(" - %s()\n", __FUNCTION__);
}

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

  auto rasterFlags = rasterID_.Flags;

  colorStride_ = Format::GetInfo((ePixelFormat)rasterFlags.ColorFormat).BytePerPixel;

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

  auto bTexMip0 = (rasterFlags.TextureMips >> 0) & 0x1;
  auto bTexMip1 = (rasterFlags.TextureMips >> 1) & 0x1;

  auto Depth = rasterFlags.DepthTest ? 1 : 0;
  auto Color = rasterFlags.Color ? 1 : 0;
  auto Tex0 = (rasterFlags.NumTextures > 0) ? (bTexMip0 ? 2 : 1) : 0;
  auto Tex1 = (rasterFlags.NumTextures > 1) ? (bTexMip1 ? 2 : 1) : 0;
  auto Fog = rasterFlags.Fog ? 1 : 0;

  int index = 1 * Depth + 2 * Color + 4 * Tex0 + 12 * Tex1 + 36 * Fog;

  assert(index < __countof(l_genericScanlines));
  pfnScanline_ = (rasterFlags.Perspective)
                     ? l_genericScanlines[index].pfnPerspective
                     : l_genericScanlines[index].pfnAffine;

  return GL_NO_ERROR;
}

void GenericRasterOp::selectDepthStencilFunc() {
  auto rasterFlags = rasterID_.Flags;
  auto rasterStates = rasterID_.States;

  if (rasterFlags.DepthTest) {
    int index = rasterStates.DepthFunc;
    assert(index < __countof(s_pfncompare));
    pfnDepthTest_ = s_pfncompare[index];
  } else {
    pfnDepthTest_ = DoCompare<COMPARE_ALWAYS>;
  }

  depthStencilStride_ =
      Format::GetInfo((ePixelFormat)rasterFlags.DepthStencilFormat).BytePerPixel;

  if (rasterFlags.StencilTest) {
    int index = rasterStates.StencilFunc;
    assert(index < __countof(s_pfncompare));
    pfnStencilTest_ = s_pfncompare[index];
  }
}

void GenericRasterOp::selectSamplerFunc() {
  auto rasterFlags = rasterID_.Flags;
  for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
    auto texState = rasterID_.Textures[i];

    auto baseIndex = 1 * texState.AddressV + 2 * texState.AddressU +
                     4 * (texState.Format - 1);

    auto minIndex = baseIndex + 20 * (texState.MinFilter - 1);

    assert(minIndex < __countof(l_pfnGetTexelColorTable));
    samplers_[i].pfnGetTexelColorMin = l_pfnGetTexelColorTable[minIndex];

    auto magIndex = baseIndex + 20 * (texState.MagFilter - 1);

    assert(magIndex < __countof(l_pfnGetTexelColorTable));
    samplers_[i].pfnGetTexelColorMag = l_pfnGetTexelColorTable[magIndex];
  }
}

void GenericRasterOp::selectTexEnvFunc() {
  auto rasterFlags = rasterID_.Flags;
  for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
    int index;

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
  auto rasterStates = rasterID_.States;
  int index = rasterStates.AlphaFunc;
  assert(index < __countof(s_pfncompare));
  pfnAlphaTest_ = s_pfncompare[index];
}

void GenericRasterOp::selectBlendFunc() {
  auto rasterStates = rasterID_.States;
  int index = rasterStates.BlendDst * BLEND_SIZE_ + rasterStates.BlendSrc;
  assert(index < __countof(s_pfnBlendTable));
  pfnBlend_ = s_pfnBlendTable[index];
}

void GenericRasterOp::selectWriteColorFunc() {
  auto rasterFlags = rasterID_.Flags;
  auto rasterStates = rasterID_.States;

  auto bColorWriteMask = rasterFlags.ColorWriteMask;
  auto logicFunc = (rasterFlags.LogicOp ? rasterStates.LogicFunc
                                        : static_cast<uint32_t>(LOGICOP_COPY));
  int index = 1 * logicFunc + LOGICOP_SIZE_ * bColorWriteMask;
  assert(index < __countof(s_pfnWriteColorTable));
  pfnWriteColor_ = s_pfnWriteColorTable[index];
}

bool GenericRasterOp::doStencilTest(const RasterData &rasterData,
                                    uint32_t depthValue,
                                    void *pDSBuffer) const {
  assert(pDSBuffer);

  auto pRasterOp =
      reinterpret_cast<const GenericRasterOp *>(rasterData.pRasterOp);

  auto &rasterID = pRasterOp->getRasterID();
  auto rasterFlags = rasterID.Flags;
  auto rasterStates = rasterID.States;

  auto depthStencilValue = *reinterpret_cast<uint32_t *>(pDSBuffer);
  auto stencilValue = depthStencilValue >> 16;
  auto _depthValue = depthStencilValue & 0xffff;
  auto stencilRef = rasterData.StencilRef;
  auto _stencilValue = stencilValue & rasterData.StencilMask;
  auto _stencilRef = stencilRef & rasterData.StencilMask;

  eStencilOp stencilOp;

  auto writeMask = rasterData.StencilWriteMask << 16;

  auto bStencilTest = (pRasterOp->pfnStencilTest_)(_stencilRef, _stencilValue);
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
  auto value = (result << 16) | depthValue;
  *reinterpret_cast<uint32_t *>(pDSBuffer) =
      (depthStencilValue & ~writeMask) | (value & writeMask);

  return bStencilTest;
}

ColorARGB GenericRasterOp::getSamplerColor(uint32_t unit,
                                           const RasterData &rasterData, 
                                           fixedRX fU,
                                           fixedRX fV, 
                                           fixedRX fM) const {
  ColorARGB color;
  auto &sampler = rasterData.Samplers[unit];
  auto fJ = fixed16::make(fM.data()); // fM is actually stored as 16:16

  switch (rasterID_.Textures[unit].MipFilter) {
  default:
    __no_default;
  case FILTER_NONE: {
    if (fJ > One<fixed16>()) {
      color = (samplers_[unit].pfnGetTexelColorMin)(sampler.pMipLevels[0], fU, fV);
    } else {
      color = (samplers_[unit].pfnGetTexelColorMag)(sampler.pMipLevels[0], fU, fV);
    }
    break;
  }
  case FILTER_NEAREST: {
    if (fJ > One<fixed16>()) {
      auto mipLevel = std::min<int>(iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
      color = (samplers_[unit].pfnGetTexelColorMin)(sampler.pMipLevels[mipLevel], fU, fV);
    } else {
      color = (samplers_[unit].pfnGetTexelColorMag)(sampler.pMipLevels[0], fU, fV);
    }
    break;
  }

  case FILTER_LINEAR: {
    if (fJ > One<fixed16>()) {
      auto mipLevel0 = std::min<int>(iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
      auto mipLevel1 = std::min<int>(mipLevel0 + 1, sampler.MaxMipLevel);
      auto mipLerp = (fJ.data() >> (mipLevel0 + 8)) - fixed8::ONE;

      auto c1 = (samplers_[unit].pfnGetTexelColorMin)(sampler.pMipLevels[mipLevel0], fU, fV);
      auto c2 = (samplers_[unit].pfnGetTexelColorMin)(sampler.pMipLevels[mipLevel1], fU, fV);

      color.r = Lerp8(c1.r, c2.r, mipLerp);
      color.g = Lerp8(c1.g, c2.g, mipLerp);
      color.b = Lerp8(c1.b, c2.b, mipLerp);
      color.a = Lerp8(c1.a, c2.a, mipLerp);
    } else {
      color = (samplers_[unit].pfnGetTexelColorMag)(sampler.pMipLevels[0], fU, fV);
    }
    break;
  }
  }
  return color;
}

ColorARGB GenericRasterOp::applyFog(const RasterData &rasterData, 
                                    const ColorARGB &cColor,
                                    fixedRX fFactor) {
  ColorARGB ret;
  auto factor = fFactor.data();
  auto cFogColor = rasterData.cFogColor;
  ret.r = cFogColor.r + ((factor * (cColor.r - cFogColor.r)) >> fixedRX::FRAC);
  ret.g = cFogColor.g + ((factor * (cColor.g - cFogColor.g)) >> fixedRX::FRAC);
  ret.b = cFogColor.b + ((factor * (cColor.b - cFogColor.b)) >> fixedRX::FRAC);
  return ret;
}