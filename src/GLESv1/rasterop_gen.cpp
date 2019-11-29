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

#ifndef GL_COCOJIT

#define MAKE_SCANLINE(fog, texture1, texture0, color, depth)                   \
  {                                                                            \
    TGenericScanlineA<depth, color, texture0, texture1, fog>::Execute,         \
        TGenericScanlineP<depth, color, texture0, texture1, fog>::Execute      \
  }

static const struct ScanlineRasterID {
  PFN_Scanline pfnAffine;
  PFN_Scanline pfnPerspective;
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

static const PFN_GetTexelColor l_pfnGetTexelColorTable[] = {
    TGetTexelColorPt<FORMAT_A8, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_A8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorPt<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorPt<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,

    //--
    TGetTexelColorLn<FORMAT_A8, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_A8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_A8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_A8L8, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_A8L8, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_RGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_RGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_ARGB, ADDRESS_WRAP, ADDRESS_CLAMP>,
    TGetTexelColorLn<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_WRAP>,
    TGetTexelColorLn<FORMAT_ARGB, ADDRESS_CLAMP, ADDRESS_CLAMP>,
};

static const PFN_Compare s_pfnCompare[] = {
    TCompare<COMPARE_NEVER>,   TCompare<COMPARE_LESS>,
    TCompare<COMPARE_EQUAL>,   TCompare<COMPARE_LEQUAL>,
    TCompare<COMPARE_GREATER>, TCompare<COMPARE_NOTEQUAL>,
    TCompare<COMPARE_GEQUAL>,  TCompare<COMPARE_ALWAYS>,
};

static const PFN_GetTexEnvColor s_pfnGetTexEnvColorTable[] = {
    TGetTexEnvColorA<ENVMODE_ADD>,        TGetTexEnvColorA<ENVMODE_BLEND>,
    TGetTexEnvColorA<ENVMODE_REPLACE>,    TGetTexEnvColorA<ENVMODE_MODULATE>,
    TGetTexEnvColorA<ENVMODE_DECAL>,

    TGetTexEnvColorRGB<ENVMODE_ADD>,      TGetTexEnvColorRGB<ENVMODE_BLEND>,
    TGetTexEnvColorRGB<ENVMODE_REPLACE>,  TGetTexEnvColorRGB<ENVMODE_MODULATE>,
    TGetTexEnvColorRGB<ENVMODE_DECAL>,

    TGetTexEnvColorARGB<ENVMODE_ADD>,     TGetTexEnvColorARGB<ENVMODE_BLEND>,
    TGetTexEnvColorARGB<ENVMODE_REPLACE>, TGetTexEnvColorARGB<ENVMODE_MODULATE>,
    TGetTexEnvColorARGB<ENVMODE_DECAL>,
};

static const PFN_Blend s_pfnBlendTable[] = {
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
  #elif defined (COCOGL_RASTER_A8R8G8B8)                 
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
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_SRC_COLOR>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_SRC_COLOR>,
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
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_SRC_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_SRC_ALPHA>,
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
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_ALPHA, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_ONE_MINUS_DST_COLOR, BLEND_ONE_MINUS_DST_ALPHA>,
    TBlend<FORMAT_A8R8G8B8, BLEND_SRC_ALPHA_SATURATE, BLEND_ONE_MINUS_DST_ALPHA>,
  #endif
};

static const PFN_WriteColor s_pfnWriteColorTable[] = {
#if defined(COCOGL_RASTER_R5G6B5)
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_CLEAR>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_AND>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_AND_REVERSE>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_COPY>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_AND_INVERTED>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_NOOP>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_XOR>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_OR>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_NOR>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_EQUIV>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_INVERT>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_OR_REVERSE>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_COPY_INVERTED>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_OR_INVERTED>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_NAND>,
    TWriteColor<FORMAT_R5G6B5, false, LOGICOP_SET>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_CLEAR>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_AND>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_AND_REVERSE>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_COPY>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_AND_INVERTED>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_NOOP>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_XOR>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_OR>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_NOR>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_EQUIV>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_INVERT>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_OR_REVERSE>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_COPY_INVERTED>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_OR_INVERTED>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_NAND>,
    TWriteColor<FORMAT_R5G6B5, true, LOGICOP_SET>,
  #elif defined(COCOGL_RASTER_A8R8G8B8)
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_CLEAR>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_AND>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_AND_REVERSE>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_COPY>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_AND_INVERTED>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_NOOP>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_XOR>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_OR>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_NOR>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_EQUIV>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_INVERT>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_OR_REVERSE>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_COPY_INVERTED>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_OR_INVERTED>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_NAND>,
    TWriteColor<FORMAT_A8R8G8B8, false, LOGICOP_SET>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_CLEAR>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_AND>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_AND_REVERSE>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_COPY>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_AND_INVERTED>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_NOOP>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_XOR>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_OR>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_NOR>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_EQUIV>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_INVERT>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_OR_REVERSE>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_COPY_INVERTED>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_OR_INVERTED>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_NAND>,
    TWriteColor<FORMAT_A8R8G8B8, true, LOGICOP_SET>,
  #endif
};

//////////////////////////////////////////////////////////////////////////////

CGenericRasterOp::CGenericRasterOp(const RASTERID &rasterID) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  m_rasterID = rasterID;
  m_colorStride = 0;
  m_depthStencilStride = 0;
  m_pfnAlphaTest = nullptr;
  m_pfnStencilTest = nullptr;
  m_pfnDepthTest = nullptr;
  m_pfnBlend = nullptr;
  m_pfnWriteColor = nullptr;
  m_pfnScanline = nullptr;
}

CGenericRasterOp::~CGenericRasterOp() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));
}

GLenum CGenericRasterOp::Create(IRasterOp **ppRasterOp,
                                const RASTERID &rasterID) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  ASSERT(ppRasterOp);

  // Create a new rasterOp object
  auto pRasterOp = new CGenericRasterOp(rasterID);
  if (nullptr == pRasterOp) {
    __glLogError(_T("CGenericRasterOp allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  pRasterOp->AddRef();

  // Initialize the rasterOp
  err = pRasterOp->Initialize();
  if (__glFailed(err)) {
    pRasterOp->Release();
    __glLogError(_T("CGenericRasterOp::Initialize() failed, err = %d.\r\n"),
                 err);
    return err;
  }

  *ppRasterOp = pRasterOp;

  return GL_NO_ERROR;
}

GLenum CGenericRasterOp::Initialize() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  RASTERFLAGS rasterFlags = m_rasterID.Flags;

  m_colorStride = Format::GetInfo(rasterFlags.ColorFormat).BytePerPixel;

  // Select depth function
  if (rasterFlags.DepthTest || rasterFlags.StencilTest) {
    this->SelectDepthStencilFunc();
  } else {
    m_depthStencilStride = 0;
  }

  // Select sampler function
  if (rasterFlags.NumTextures) {
    this->SelectSamplerFunc();
    this->SelectTexEnvFunc();
  }

  // Select alpha test function
  if (rasterFlags.AlphaTest) {
    this->SelectAlphaTestFunc();
  }

  if (rasterFlags.Blend) {
    // Select blend function
    this->SelectBlendFunc();
  }

  // Select write color function
  this->SelectWriteColorFunc();

  bool bTexMip0 = ((rasterFlags.TextureMips >> 0) & 0x1);
  bool bTexMip1 = ((rasterFlags.TextureMips >> 1) & 0x1);

  uint32_t Depth = rasterFlags.DepthTest ? 1 : 0;
  uint32_t Color = rasterFlags.Color ? 1 : 0;
  uint32_t Tex0 = (rasterFlags.NumTextures > 0) ? (bTexMip0 ? 2 : 1) : 0;
  uint32_t Tex1 = (rasterFlags.NumTextures > 1) ? (bTexMip1 ? 2 : 1) : 0;
  uint32_t Fog = rasterFlags.Fog ? 1 : 0;

  uint32_t index =
      1 * Depth + 2 * Color + 4 * Tex0 + 12 * Tex1 + 36 * Fog;

  ASSERT(index < __countof(l_genericScanlines));
  m_pfnScanline = (rasterFlags.Perspective)
                      ? l_genericScanlines[index].pfnPerspective
                      : l_genericScanlines[index].pfnAffine;

  return GL_NO_ERROR;
}

void CGenericRasterOp::SelectDepthStencilFunc() {
  RASTERFLAGS rasterFlags = m_rasterID.Flags;
  RASTERSTATES rasterStates = m_rasterID.States;

  if (rasterFlags.DepthTest) {
    uint32_t index = rasterStates.DepthFunc;
    ASSERT(index < __countof(s_pfnCompare));
    m_pfnDepthTest = s_pfnCompare[index];
  } else {
    m_pfnDepthTest = TCompare<COMPARE_ALWAYS>;
  }

  m_depthStencilStride =
      Format::GetInfo(rasterFlags.DepthStencilFormat).BytePerPixel;

  if (rasterFlags.StencilTest) {
    uint32_t index = rasterStates.StencilFunc;
    ASSERT(index < __countof(s_pfnCompare));
    m_pfnStencilTest = s_pfnCompare[index];
  }
}

void CGenericRasterOp::SelectSamplerFunc() {
  RASTERFLAGS rasterFlags = m_rasterID.Flags;
  for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
    TEXTURESTATES texState = m_rasterID.Textures[i];

    uint32_t baseIndex = 1 * texState.AddressV + 2 * texState.AddressU +
                               4 * (texState.Format - 1);

    uint32_t minIndex = baseIndex + 20 * (texState.MinFilter - 1);

    ASSERT(minIndex < __countof(l_pfnGetTexelColorTable));
    m_samplers[i].pfnGetTexelColorMin = l_pfnGetTexelColorTable[minIndex];

    uint32_t magIndex = baseIndex + 20 * (texState.MagFilter - 1);

    ASSERT(magIndex < __countof(l_pfnGetTexelColorTable));
    m_samplers[i].pfnGetTexelColorMag = l_pfnGetTexelColorTable[magIndex];
  }
}

void CGenericRasterOp::SelectTexEnvFunc() {
  RASTERFLAGS rasterFlags = m_rasterID.Flags;
  for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
    uint32_t index;

    switch (m_rasterID.Textures[i].Format) {
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

    index = m_rasterID.Textures[i].EnvMode + index * ENVMODE_SIZE_;
    ASSERT(index < __countof(s_pfnGetTexEnvColorTable));
    m_samplers[i].pfnGetTexEnvColor = s_pfnGetTexEnvColorTable[index];
  }
}

void CGenericRasterOp::SelectAlphaTestFunc() {
  RASTERSTATES rasterStates = m_rasterID.States;

  uint32_t index = rasterStates.AlphaFunc;
  ASSERT(index < __countof(s_pfnCompare));
  m_pfnAlphaTest = s_pfnCompare[index];
}

void CGenericRasterOp::SelectBlendFunc() {
  RASTERSTATES rasterStates = m_rasterID.States;
  uint32_t index =
      rasterStates.BlendDst * BLEND_SIZE_ + rasterStates.BlendSrc;

  ASSERT(index < __countof(s_pfnBlendTable));
  m_pfnBlend = s_pfnBlendTable[index];
}

void CGenericRasterOp::SelectWriteColorFunc() {
  RASTERFLAGS rasterFlags = m_rasterID.Flags;
  RASTERSTATES rasterStates = m_rasterID.States;

  bool bColorWriteMask = rasterFlags.ColorWriteMask;
  uint32_t logicFunc =
      (rasterFlags.LogicOp ? rasterStates.LogicFunc : static_cast<uint32_t>(LOGICOP_COPY));
  uint32_t index = 1 * logicFunc + LOGICOP_SIZE_ * bColorWriteMask;

  ASSERT(index < __countof(s_pfnWriteColorTable));
  m_pfnWriteColor = s_pfnWriteColorTable[index];
}

bool CGenericRasterOp::DoStencilTest(const RasterData &rasterData,
                                     uint32_t depthValue,
                                     void *pDSBuffer) const {
  ASSERT(pDSBuffer);

  auto pRasterOp =
      reinterpret_cast<const CGenericRasterOp *>(rasterData.pRasterOp);

  const RASTERID &rasterID = pRasterOp->GetRasterID();
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

  bool bStencilTest = (pRasterOp->m_pfnStencilTest)(_stencilRef, _stencilValue);
  if (bStencilTest) {
    if ((pRasterOp->m_pfnDepthTest)(depthValue, _depthValue)) {
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

void CGenericRasterOp::GetSamplerColor(Color4 *pOut, uint32_t unit,
                                       const RasterData &rasterData, fixedRX fU,
                                       fixedRX fV, fixedRX fM) const {
  const Sampler &sampler = rasterData.Samplers[unit];

  switch (m_rasterID.Textures[unit].MipFilter) {
  default:
    __no_default;
  case FILTER_NONE: {
    auto fJ = fixed16::Make(fM.GetRaw());
    if (fJ > TConst<fixed16>::One()) {
      (m_samplers[unit].pfnGetTexelColorMin)(pOut, sampler.pMipLevels[0], fU,
                                             fV);
    } else {
      (m_samplers[unit].pfnGetTexelColorMag)(pOut, sampler.pMipLevels[0], fU,
                                             fV);
    }

    break;
  }
  case FILTER_NEAREST: {
    auto fJ = fixed16::Make(fM.GetRaw());
    if (fJ > TConst<fixed16>::One()) {
      int mipLevel = Math::TMin<int>(
          Math::iLog2(fJ.GetRaw()) - fixed16::FRAC, sampler.MaxMipLevel);

      (m_samplers[unit].pfnGetTexelColorMin)(pOut, sampler.pMipLevels[mipLevel],
                                             fU, fV);
    } else {
      (m_samplers[unit].pfnGetTexelColorMag)(pOut, sampler.pMipLevels[0], fU,
                                             fV);
    }

    break;
  }

  case FILTER_LINEAR: {
    auto fJ = fixed16::Make(fM.GetRaw());
    if (fJ > TConst<fixed16>::One()) {
      Color4 tmp;

      int mipLevel0 = Math::TMin<int>(
          Math::iLog2(fJ.GetRaw()) - fixed16::FRAC, sampler.MaxMipLevel);

      int mipLevel1 = Math::TMin<int>(mipLevel0 + 1, sampler.MaxMipLevel);
      int mipLerp = (fJ.GetRaw() >> (mipLevel0 + 8)) - fixed8::ONE;

      (m_samplers[unit].pfnGetTexelColorMin)(
          pOut, sampler.pMipLevels[mipLevel0], fU, fV);
      (m_samplers[unit].pfnGetTexelColorMin)(
          &tmp, sampler.pMipLevels[mipLevel1], fU, fV);

      pOut->r = Math::Lerp8(pOut->r, tmp.r, mipLerp);
      pOut->g = Math::Lerp8(pOut->g, tmp.g, mipLerp);
      pOut->b = Math::Lerp8(pOut->b, tmp.b, mipLerp);
      pOut->a = Math::Lerp8(pOut->a, tmp.a, mipLerp);
    } else {
      (m_samplers[unit].pfnGetTexelColorMag)(pOut, sampler.pMipLevels[0], fU,
                                             fV);
    }

    break;
  }
  }
}

void CGenericRasterOp::ApplyFog(const RasterData &rasterData, Color4 *pInOut,
                                fixedRX fFactor) {
  const Color4 &cFogColor = rasterData.cFogColor;
  int factor = fFactor.GetRaw();

  pInOut->r =
      cFogColor.r + ((factor * (pInOut->r - cFogColor.r)) >> fixedRX::FRAC);
  pInOut->g =
      cFogColor.g + ((factor * (pInOut->g - cFogColor.g)) >> fixedRX::FRAC);
  pInOut->b =
      cFogColor.b + ((factor * (pInOut->b - cFogColor.b)) >> fixedRX::FRAC);
}

#endif