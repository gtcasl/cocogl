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
#include "context.hpp"
#include "context_query.inl"
#include "context_rast.inl"
#include "context_tnl.inl"
#include "driver.hpp"

#ifndef NDEBUG

const TCHAR *FuncToString(GLenum func) {
  switch (func) {
  case GL_NEVER:
    return _T("GL_NEVER");
  case GL_LESS:
    return _T("GL_LESS");
  case GL_EQUAL:
    return _T("GL_EQUAL");
  case GL_LEQUAL:
    return _T("GL_LEQUAL");
  case GL_GREATER:
    return _T("GL_GREATER");
  case GL_NOTEQUAL:
    return _T("GL_NOTEQUAL");
  case GL_GEQUAL:
    return _T("GL_GEQUAL");
  case GL_ALWAYS:
    return _T("GL_ALWAYS");
  }

  return _T("UNKNOWN");
}

const TCHAR *PlaneToString(GLenum plane) {
  switch (plane) {
  case GL_CLIP_PLANE0:
    return _T("GL_CLIP_PLANE0");
  case GL_CLIP_PLANE1:
    return _T("GL_CLIP_PLANE1");
  case GL_CLIP_PLANE2:
    return _T("GL_CLIP_PLANE2");
  case GL_CLIP_PLANE3:
    return _T("GL_CLIP_PLANE3");
  case GL_CLIP_PLANE4:
    return _T("GL_CLIP_PLANE4");
  case GL_CLIP_PLANE5:
    return _T("GL_CLIP_PLANE5");
  }

  return _T("UNKNOWN");
}

const TCHAR *FogParamToString(GLenum pname) {
  switch (pname) {
  case GL_FOG_DENSITY:
    return _T("GL_FOG_DENSITY");
  case GL_FOG_START:
    return _T("GL_FOG_START");
  case GL_FOG_END:
    return _T("GL_FOG_END");
  case GL_FOG_MODE:
    return _T("GL_FOG_MODE");
  case GL_FOG_COLOR:
    return _T("GL_FOG_COLOR");
  case GL_CLIP_PLANE5:
    return _T("GL_CLIP_PLANE5");
  }

  return _T("UNKNOWN");
}

const TCHAR *LightToString(GLenum light) {
  switch (light) {
  case GL_LIGHT0:
    return _T("GL_LIGHT0");
  case GL_LIGHT1:
    return _T("GL_LIGHT1");
  case GL_LIGHT2:
    return _T("GL_LIGHT2");
  case GL_LIGHT3:
    return _T("GL_LIGHT3");
  case GL_LIGHT4:
    return _T("GL_LIGHT4");
  case GL_LIGHT5:
    return _T("GL_LIGHT5");
  case GL_LIGHT6:
    return _T("GL_LIGHT6");
  case GL_LIGHT7:
    return _T("GL_LIGHT7");
  }

  return _T("UNKNOWN");
}

const TCHAR *LightModelToString(GLenum pname) {
  switch (pname) {
  case GL_LIGHT_MODEL_AMBIENT:
    return _T("GL_LIGHT_MODEL_AMBIENT");
  case GL_LIGHT_MODEL_TWO_SIDE:
    return _T("GL_LIGHT_MODEL_TWO_SIDE");
  }

  return _T("UNKNOWN");
}

const TCHAR *LightParamToString(GLenum pname) {
  switch (pname) {
  case GL_AMBIENT:
    return _T("GL_AMBIENT");
  case GL_DIFFUSE:
    return _T("GL_DIFFUSE");
  case GL_SPECULAR:
    return _T("GL_SPECULAR");
  case GL_POSITION:
    return _T("GL_POSITION");
  case GL_SPOT_DIRECTION:
    return _T("GL_SPOT_DIRECTION");
  case GL_SPOT_EXPONENT:
    return _T("GL_SPOT_EXPONENT");
  case GL_SPOT_CUTOFF:
    return _T("GL_SPOT_CUTOFF");
  case GL_CONSTANT_ATTENUATION:
    return _T("GL_CONSTANT_ATTENUATION");
  case GL_LINEAR_ATTENUATION:
    return _T("GL_LINEAR_ATTENUATION");
  case GL_QUADRATIC_ATTENUATION:
    return _T("GL_QUADRATIC_ATTENUATION");
  }

  return _T("UNKNOWN");
}

const TCHAR *MaterialFaceToString(GLenum pname) {
  switch (pname) {
  case GL_FRONT_AND_BACK:
    return _T("GL_FRONT_AND_BACK");
  }

  return _T("UNKNOWN");
}

const TCHAR *MaterialParamToString(GLenum pname) {
  switch (pname) {
  case GL_EMISSION:
    return _T("GL_EMISSION");
  case GL_SHININESS:
    return _T("GL_SHININESS");
  case GL_AMBIENT_AND_DIFFUSE:
    return _T("GL_AMBIENT_AND_DIFFUSE");
  case GL_AMBIENT:
    return _T("GL_AMBIENT");
  case GL_DIFFUSE:
    return _T("GL_DIFFUSE");
  case GL_SPECULAR:
    return _T("GL_SPECULAR");
  }

  return _T("UNKNOWN");
}

const TCHAR *TextureToString(GLenum texture) {
  switch (texture) {
  case GL_TEXTURE0:
    return _T("GL_TEXTURE0");
  case GL_TEXTURE1:
    return _T("GL_TEXTURE1");
  }

  return _T("UNKNOWN");
}

const TCHAR *PNameToString(GLenum pname) {
  switch (pname) {
  case GL_POINT_SIZE_MIN:
    return _T("GL_POINT_SIZE_MIN");

  case GL_POINT_SIZE_MAX:
    return _T("GL_POINT_SIZE_MAX");

  case GL_POINT_FADE_THRESHOLD_SIZE:
    return _T("GL_POINT_FADE_THRESHOLD_SIZE");

  case GL_POINT_DISTANCE_ATTENUATION:
    return _T("GL_POINT_DISTANCE_ATTENUATION");
  }

  return _T("UNKNOWN");
}

const TCHAR *TexEnvToString(GLenum env) {
  switch (env) {
  case GL_TEXTURE_ENV:
    return _T("GL_TEXTURE_ENV");
  case GL_POINT_SPRITE_OES:
    return _T("GL_POINT_SPRITE_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *TexEnvParamToString(GLenum pname) {
  switch (pname) {
  case GL_TEXTURE_ENV_MODE:
    return _T("GL_TEXTURE_ENV_MODE");
  case GL_TEXTURE_ENV_COLOR:
    return _T("GL_TEXTURE_ENV_COLOR");
  case GL_COORD_REPLACE_OES:
    return _T("GL_COORD_REPLACE_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *TexParamToString(GLenum pname) {
  switch (pname) {
  case GL_TEXTURE_MIN_FILTER:
    return _T("GL_TEXTURE_MIN_FILTER");
  case GL_TEXTURE_MAG_FILTER:
    return _T("GL_TEXTURE_MAG_FILTER");
  case GL_TEXTURE_WRAP_S:
    return _T("GL_TEXTURE_WRAP_S");
  case GL_TEXTURE_WRAP_T:
    return _T("GL_TEXTURE_WRAP_T");
  case GL_GENERATE_MIPMAP:
    return _T("GL_GENERATE_MIPMAP");
  }

  return _T("UNKNOWN");
}

const TCHAR *BufferToString(GLenum pname) {
  switch (pname) {
  case GL_ARRAY_BUFFER:
    return _T("GL_ARRAY_BUFFER");
  case GL_ELEMENT_ARRAY_BUFFER:
    return _T("GL_ELEMENT_ARRAY_BUFFER");
  }

  return _T("UNKNOWN");
}

const TCHAR *BufferParamToString(GLenum pname) {
  switch (pname) {
  case GL_BUFFER_SIZE:
    return _T("GL_BUFFER_SIZE");
  case GL_BUFFER_USAGE:
    return _T("GL_BUFFER_USAGE");
  }

  return _T("UNKNOWN");
}

const TCHAR *BlendFuncToString(GLenum pname) {
  switch (pname) {
  case GL_ZERO:
    return _T("GL_ZERO");
  case GL_ONE:
    return _T("GL_ONE");
  case GL_SRC_ALPHA:
    return _T("GL_SRC_ALPHA");
  case GL_ONE_MINUS_SRC_ALPHA:
    return _T("GL_ONE_MINUS_SRC_ALPHA");
  case GL_DST_ALPHA:
    return _T("GL_DST_ALPHA");
  case GL_ONE_MINUS_DST_ALPHA:
    return _T("GL_ONE_MINUS_DST_ALPHA");
  case GL_DST_COLOR:
    return _T("GL_DST_COLOR");
  case GL_ONE_MINUS_DST_COLOR:
    return _T("GL_ONE_MINUS_DST_COLOR");
  case GL_SRC_ALPHA_SATURATE:
    return _T("GL_SRC_ALPHA_SATURATE");
  case GL_SRC_COLOR:
    return _T("GL_SRC_COLOR");
  case GL_ONE_MINUS_SRC_COLOR:
    return _T("GL_ONE_MINUS_SRC_COLOR");
  }

  return _T("UNKNOWN");
}

const TCHAR *UsageToString(GLenum usage) {
  switch (usage) {
  case GL_STATIC_DRAW:
    return _T("GL_STATIC_DRAW");
  case GL_DYNAMIC_DRAW:
    return _T("GL_DYNAMIC_DRAW");
  }

  return _T("UNKNOWN");
}

const TCHAR *TypeToString(GLenum type) {
  switch (type) {
  case GL_BYTE:
    return _T("GL_BYTE");
  case GL_UNSIGNED_BYTE:
    return _T("GL_UNSIGNED_BYTE");
  case GL_SHORT:
    return _T("GL_SHORT");
  case GL_UNSIGNED_SHORT:
    return _T("GL_UNSIGNED_SHORT");
  case GL_FLOAT:
    return _T("GL_FLOAT");
  case GL_FIXED:
    return _T("GL_FIXED");
  case GL_UNSIGNED_SHORT_4_4_4_4:
    return _T("GL_UNSIGNED_SHORT_4_4_4_4");
  case GL_UNSIGNED_SHORT_5_5_5_1:
    return _T("GL_UNSIGNED_SHORT_5_5_5_1");
  case GL_UNSIGNED_SHORT_5_6_5:
    return _T("GL_UNSIGNED_SHORT_5_6_5");
  }

  return _T("UNKNOWN");
}

const TCHAR *TextureTypeToString(GLenum texture) {
  switch (texture) {
  case GL_TEXTURE_2D:
    return _T("GL_TEXTURE_2D");
  }

  return _T("UNKNOWN");
}

const TCHAR *FormatToString(GLenum format) {
  switch (format) {
  case GL_ALPHA:
    return _T("GL_ALPHA");
  case GL_RGB:
    return _T("GL_RGB");
  case GL_RGBA:
    return _T("GL_RGBA");
  case GL_LUMINANCE:
    return _T("GL_LUMINANCE");
  case GL_LUMINANCE_ALPHA:
    return _T("GL_LUMINANCE_ALPHA");
  case GL_PALETTE4_RGB8_OES:
    return _T("GL_PALETTE4_RGB8_OES");
  case GL_PALETTE4_RGBA8_OES:
    return _T("GL_PALETTE4_RGBA8_OES");
  case GL_PALETTE4_R5_G6_B5_OES:
    return _T("GL_PALETTE4_R5_G6_B5_OES");
  case GL_PALETTE4_RGBA4_OES:
    return _T("GL_PALETTE4_RGBA4_OES");
  case GL_PALETTE4_RGB5_A1_OES:
    return _T("GL_PALETTE4_RGB5_A1_OES");
  case GL_PALETTE8_RGB8_OES:
    return _T("GL_PALETTE8_RGB8_OES");
  case GL_PALETTE8_RGBA8_OES:
    return _T("GL_PALETTE8_RGBA8_OES");
  case GL_PALETTE8_R5_G6_B5_OES:
    return _T("GL_PALETTE8_R5_G6_B5_OES");
  case GL_PALETTE8_RGBA4_OES:
    return _T("GL_PALETTE8_RGBA4_OES");
  case GL_PALETTE8_RGB5_A1_OES:
    return _T("GL_PALETTE8_RGB5_A1_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *CullFaceToString(GLenum mode) {
  switch (mode) {
  case GL_FRONT:
    return _T("GL_FRONT");
  case GL_BACK:
    return _T("GL_BACK");
  case GL_FRONT_AND_BACK:
    return _T("GL_FRONT_AND_BACK");
  }

  return _T("UNKNOWN");
}

const TCHAR *FogToString(GLenum pname) {
  switch (pname) {
  case GL_FOG_MODE:
    return _T("GL_FOG_MODE");
  case GL_FOG_START:
    return _T("GL_FOG_START");
  case GL_FOG_END:
    return _T("GL_FOG_END");
  case GL_FOG_DENSITY:
    return _T("GL_FOG_DENSITY");
  case GL_FOG_COLOR:
    return _T("GL_FOG_COLOR");
  }

  return _T("UNKNOWN");
}

const TCHAR *PrimitiveTypeToString(GLenum mode) {
  switch (mode) {
  case GL_POINTS:
    return _T("GL_POINTS");
  case GL_LINES:
    return _T("GL_LINES");
  case GL_LINE_STRIP:
    return _T("GL_LINE_STRIP");
  case GL_LINE_LOOP:
    return _T("GL_LINE_LOOP");
  case GL_TRIANGLES:
    return _T("GL_TRIANGLES");
  case GL_TRIANGLE_STRIP:
    return _T("GL_TRIANGLE_STRIP");
  case GL_TRIANGLE_FAN:
    return _T("GL_TRIANGLE_FAN");
  }

  return _T("UNKNOWN");
}

const TCHAR *FrontFaceToString(GLenum mode) {
  switch (mode) {
  case GL_CW:
    return _T("GL_CW");
  case GL_CCW:
    return _T("GL_CCW");
  }

  return _T("UNKNOWN");
}

const TCHAR *ClientStateToString(GLenum array) {
  switch (array) {
  case GL_VERTEX_ARRAY:
    return _T("GL_VERTEX_ARRAY");
  case GL_NORMAL_ARRAY:
    return _T("GL_NORMAL_ARRAY");
  case GL_COLOR_ARRAY:
    return _T("GL_COLOR_ARRAY");
  case GL_TEXTURE_COORD_ARRAY:
    return _T("GL_TEXTURE_COORD_ARRAY");
  case GL_POINT_SIZE_ARRAY_OES:
    return _T("GL_POINT_SIZE_ARRAY_OES");
  case GL_WEIGHT_ARRAY_OES:
    return _T("GL_WEIGHT_ARRAY_OES");
  case GL_MATRIX_INDEX_ARRAY_OES:
    return _T("GL_MATRIX_INDEX_ARRAY_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *HintTargetToString(GLenum target) {
  switch (target) {
  case GL_PERSPECTIVE_CORRECTION_HINT:
    return _T("GL_PERSPECTIVE_CORRECTION_HINT");

  case GL_POINT_SMOOTH_HINT:
    return _T("GL_POINT_SMOOTH_HINT");

  case GL_LINE_SMOOTH_HINT:
    return _T("GL_LINE_SMOOTH_HINT");

  case GL_FOG_HINT:
    return _T("GL_FOG_HINT");

  case GL_GENERATE_MIPMAP_HINT:
    return _T("GL_GENERATE_MIPMAP_HINT");
  }

  return _T("UNKNOWN");
}

const TCHAR *HintModeToString(GLenum mode) {
  switch (mode) {
  case GL_DONT_CARE:
    return _T("GL_DONT_CARE");
  case GL_FASTEST:
    return _T("GL_FASTEST");
  case GL_NICEST:
    return _T("GL_NICEST");
  }

  return _T("UNKNOWN");
}

const TCHAR *LogicOpToString(GLenum logicOp) {
  switch (logicOp) {
  case GL_CLEAR:
    return _T("GL_CLEAR");
  case GL_AND:
    return _T("GL_AND");
  case GL_AND_REVERSE:
    return _T("GL_AND_REVERSE");
  case GL_COPY:
    return _T("GL_COPY");
  case GL_AND_INVERTED:
    return _T("GL_AND_INVERTED");
  case GL_NOOP:
    return _T("GL_NOOP");
  case GL_XOR:
    return _T("GL_XOR");
  case GL_OR:
    return _T("GL_OR");
  case GL_NOR:
    return _T("GL_NOR");
  case GL_EQUIV:
    return _T("GL_EQUIV");
  case GL_INVERT:
    return _T("GL_INVERT");
  case GL_OR_REVERSE:
    return _T("GL_OR_REVERSE");
  case GL_COPY_INVERTED:
    return _T("GL_COPY_INVERTED");
  case GL_OR_INVERTED:
    return _T("GL_OR_INVERTED");
  case GL_NAND:
    return _T("GL_NAND");
  case GL_SET:
    return _T("GL_SET");
  }

  return _T("UNKNOWN");
}

const TCHAR *MatrixModeToString(GLenum mode) {
  switch (mode) {
  case GL_MODELVIEW:
    return _T("GL_MODELVIEW");
  case GL_PROJECTION:
    return _T("GL_PROJECTION");
  case GL_TEXTURE:
    return _T("GL_TEXTURE");
  case GL_MATRIX_PALETTE_OES:
    return _T("GL_MATRIX_PALETTE_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *PixelStoreToString(GLenum pname) {
  switch (pname) {
  case GL_PACK_ALIGNMENT:
    return _T("GL_PACK_ALIGNMENT");
  case GL_UNPACK_ALIGNMENT:
    return _T("GL_UNPACK_ALIGNMENT");
  }

  return _T("UNKNOWN");
}

const TCHAR *ShadeModelToString(GLenum mode) {
  switch (mode) {
  case GL_FLAT:
    return _T("GL_FLAT");
  case GL_SMOOTH:
    return _T("GL_SMOOTH");
  }

  return _T("UNKNOWN");
}

const TCHAR *StencilOpToString(GLenum mode) {
  switch (mode) {
  case GL_ZERO:
    return _T("GL_ZERO");
  case GL_INVERT:
    return _T("GL_INVERT");
  case GL_KEEP:
    return _T("GL_KEEP");
  case GL_REPLACE:
    return _T("GL_REPLACE");
  case GL_INCR:
    return _T("GL_INCR");
  case GL_DECR:
    return _T("GL_DECR");
  }

  return _T("UNKNOWN");
}

const TCHAR *PointParameterToString(GLenum mode) {
  switch (mode) {
  case GL_POINT_SIZE_MIN:
    return _T("GL_POINT_SIZE_MIN");

  case GL_POINT_SIZE_MAX:
    return _T("GL_POINT_SIZE_MAX");

  case GL_POINT_FADE_THRESHOLD_SIZE:
    return _T("GL_POINT_FADE_THRESHOLD_SIZE");

  case GL_POINT_DISTANCE_ATTENUATION:
    return _T("GL_POINT_DISTANCE_ATTENUATION");
  }

  return _T("UNKNOWN");
}

const TCHAR *TexParamValueToString(GLint param) {
  switch (param) {
  case GL_FALSE:
    return _T("GL_FALSE");
  case GL_TRUE:
    return _T("GL_TRUE");
  case GL_NEAREST:
    return _T("GL_NEAREST");
  case GL_LINEAR:
    return _T("GL_LINEAR");
  case GL_NEAREST_MIPMAP_NEAREST:
    return _T("GL_NEAREST_MIPMAP_NEAREST");
  case GL_LINEAR_MIPMAP_NEAREST:
    return _T("GL_LINEAR_MIPMAP_NEAREST");
  case GL_NEAREST_MIPMAP_LINEAR:
    return _T("GL_NEAREST_MIPMAP_LINEAR");
  case GL_LINEAR_MIPMAP_LINEAR:
    return _T("GL_LINEAR_MIPMAP_LINEAR");
  case GL_CLAMP_TO_EDGE:
    return _T("GL_CLAMP_TO_EDGE");
  case GL_REPEAT:
    return _T("GL_REPEAT");
  }

  return _T("UNKNOWN");
}

const TCHAR *TexEnvValueToString(GLint param) {
  switch (param) {
  case GL_ADD:
    return _T("GL_ADD");
  case GL_BLEND:
    return _T("GL_BLEND");
  case GL_REPLACE:
    return _T("GL_REPLACE");
  case GL_MODULATE:
    return _T("GL_MODULATE");
  case GL_DECAL:
    return _T("GL_DECAL");
  case GL_COORD_REPLACE_OES:
    return _T("GL_COORD_REPLACE_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *PointerToString(GLenum pname) {
  switch (pname) {
  case GL_VERTEX_ARRAY_POINTER:
    return _T("GL_VERTEX_ARRAY_POINTER");

  case GL_NORMAL_ARRAY_POINTER:
    return _T("GL_NORMAL_ARRAY_POINTER");

  case GL_COLOR_ARRAY_POINTER:
    return _T("GL_COLOR_ARRAY_POINTER");

  case GL_TEXTURE_COORD_ARRAY_POINTER:
    return _T("GL_TEXTURE_COORD_ARRAY_POINTER");

  case GL_POINT_SIZE_ARRAY_OES:
    return _T("GL_POINT_SIZE_ARRAY_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *StringToString(GLenum pname) {
  switch (pname) {
  case GL_VENDOR:
    return _T("GL_VENDOR");
  case GL_VERSION:
    return _T("GL_VERSION");
  case GL_RENDERER:
    return _T("GL_RENDERER");
  case GL_EXTENSIONS:
    return _T("GL_EXTENSIONS");
  }

  return _T("UNKNOWN");
}

const TCHAR *CapToString(GLenum cap) {
  switch (cap) {
  case GL_FOG:
    return _T("GL_FOG");

  case GL_LIGHTING:
    return _T("GL_LIGHTING");

  case GL_TEXTURE_2D:
    return _T("GL_TEXTURE_2D");

  case GL_CULL_FACE:
    return _T("GL_CULL_FACE");

  case GL_ALPHA_TEST:
    return _T("GL_ALPHA_TEST");

  case GL_BLEND:
    return _T("GL_BLEND");

  case GL_COLOR_LOGIC_OP:
    return _T("GL_COLOR_LOGIC_OP");

  case GL_DITHER:
    return _T("GL_DITHER");

  case GL_STENCIL_TEST:
    return _T("GL_STENCIL_TEST");

  case GL_DEPTH_TEST:
    return _T("GL_DEPTH_TEST");

  case GL_POINT_SMOOTH:
    return _T("GL_POINT_SMOOTH");

  case GL_POINT_SPRITE_OES:
    return _T("GL_POINT_SPRITE_OES");

  case GL_LINE_SMOOTH:
    return _T("GL_LINE_SMOOTH");

  case GL_SCISSOR_TEST:
    return _T("GL_SCISSOR_TEST");

  case GL_COLOR_MATERIAL:
    return _T("GL_COLOR_MATERIAL");

  case GL_NORMALIZE:
    return _T("GL_NORMALIZE");

  case GL_CLIP_PLANE0:
    return _T("GL_CLIP_PLANE0");

  case GL_CLIP_PLANE1:
    return _T("GL_CLIP_PLANE1");

  case GL_CLIP_PLANE2:
    return _T("GL_CLIP_PLANE2");

  case GL_CLIP_PLANE3:
    return _T("GL_CLIP_PLANE3");

  case GL_CLIP_PLANE4:
    return _T("GL_CLIP_PLANE4");

  case GL_CLIP_PLANE5:
    return _T("GL_CLIP_PLANE5");

  case GL_LIGHT0:
    return _T("GL_LIGHT0");

  case GL_LIGHT1:
    return _T("GL_LIGHT1");

  case GL_LIGHT2:
    return _T("GL_LIGHT2");

  case GL_LIGHT3:
    return _T("GL_LIGHT3");

  case GL_LIGHT4:
    return _T("GL_LIGHT4");

  case GL_LIGHT5:
    return _T("GL_LIGHT5");

  case GL_LIGHT6:
    return _T("GL_LIGHT6");

  case GL_LIGHT7:
    return _T("GL_LIGHT7");

  case GL_RESCALE_NORMAL:
    return _T("GL_RESCALE_NORMAL");

  case GL_POLYGON_OFFSET_FILL:
    return _T("GL_POLYGON_OFFSET_FILL");

  case GL_MULTISAMPLE:
    return _T("GL_MULTISAMPLE");

  case GL_SAMPLE_ALPHA_TO_COVERAGE:
    return _T("GL_SAMPLE_ALPHA_TO_COVERAGE");

  case GL_SAMPLE_ALPHA_TO_ONE:
    return _T("GL_SAMPLE_ALPHA_TO_ONE");

  case GL_SAMPLE_COVERAGE:
    return _T("GL_SAMPLE_COVERAGE");

  case GL_MATRIX_PALETTE_OES:
    return _T("GL_MATRIX_PALETTE_OES");
  }

  return _T("UNKNOWN");
}

const TCHAR *AttributeToString(GLenum pname) {
  switch (pname) {
  case GL_LIGHT_MODEL_TWO_SIDE:
    return _T("GL_LIGHT_MODEL_TWO_SIDE");

  case GL_COLOR_WRITEMASK:
    return _T("GL_COLOR_WRITEMASK");

  case GL_DEPTH_WRITEMASK:
    return _T("GL_DEPTH_WRITEMASK");

  case GL_SAMPLE_COVERAGE_INVERT:
    return _T("GL_SAMPLE_COVERAGE_INVERT");

  case GL_COORD_REPLACE_OES:
    return _T("GL_COORD_REPLACE_OES");

  case GL_ALIASED_POINT_SIZE_RANGE:
    return _T("GL_ALIASED_POINT_SIZE_RANGE");

  case GL_ALIASED_LINE_WIDTH_RANGE:
    return _T("GL_ALIASED_LINE_WIDTH_RANGE");

  case GL_SMOOTH_LINE_WIDTH_RANGE:
    return _T("GL_SMOOTH_LINE_WIDTH_RANGE");

  case GL_SMOOTH_POINT_SIZE_RANGE:
    return _T("GL_SMOOTH_POINT_SIZE_RANGE");

  case GL_ALPHA_BITS:
    return _T("GL_ALPHA_BITS");

  case GL_BLUE_BITS:
    return _T("GL_BLUE_BITS");

  case GL_DEPTH_BITS:
    return _T("GL_DEPTH_BITS");

  case GL_GREEN_BITS:
    return _T("GL_GREEN_BITS");

  case GL_RED_BITS:
    return _T("GL_RED_BITS");

  case GL_STENCIL_BITS:
    return _T("GL_STENCIL_BITS");

  case GL_SUBPIXEL_BITS:
    return _T("GL_SUBPIXEL_BITS");

  case GL_COMPRESSED_TEXTURE_FORMATS:
    return _T("GL_COMPRESSED_TEXTURE_FORMATS");

  case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
    return _T("GL_NUM_COMPRESSED_TEXTURE_FORMATS");

  case GL_MAX_LIGHTS:
    return _T("GL_MAX_LIGHTS");

  case GL_MAX_CLIP_PLANES:
    return _T("GL_MAX_CLIP_PLANES");

  case GL_MAX_MODELVIEW_STACK_DEPTH:
    return _T("GL_MAX_MODELVIEW_STACK_DEPTH");

  case GL_MAX_PROJECTION_STACK_DEPTH:
    return _T("GL_MAX_PROJECTION_STACK_DEPTH");

  case GL_MAX_TEXTURE_STACK_DEPTH:
    return _T("GL_MAX_TEXTURE_STACK_DEPTH");

  case GL_MAX_TEXTURE_SIZE:
    return _T("GL_MAX_TEXTURE_SIZE");

  case GL_MAX_TEXTURE_UNITS:
    return _T("GL_MAX_TEXTURE_UNITS");

  case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
    return _T("GL_IMPLEMENTATION_COLOR_READ_TYPE_OES");

  case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
    return _T("GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES");

  case GL_MAX_VIEWPORT_DIMS:
    return _T("GL_MAX_VIEWPORT_DIMS");

  case GL_VERTEX_ARRAY_SIZE:
    return _T("GL_VERTEX_ARRAY_SIZE");

  case GL_VERTEX_ARRAY_STRIDE:
    return _T("GL_VERTEX_ARRAY_STRIDE");

  case GL_VERTEX_ARRAY_TYPE:
    return _T("GL_VERTEX_ARRAY_TYPE");

  case GL_NORMAL_ARRAY_STRIDE:
    return _T("GL_NORMAL_ARRAY_STRIDE");

  case GL_NORMAL_ARRAY_TYPE:
    return _T("GL_NORMAL_ARRAY_TYPE");

  case GL_COLOR_ARRAY_SIZE:
    return _T("GL_COLOR_ARRAY_SIZE");

  case GL_COLOR_ARRAY_STRIDE:
    return _T("GL_COLOR_ARRAY_STRIDE");

  case GL_COLOR_ARRAY_TYPE:
    return _T("GL_COLOR_ARRAY_TYPE");

  case GL_TEXTURE_COORD_ARRAY_SIZE:
    return _T("GL_TEXTURE_COORD_ARRAY_SIZE");

  case GL_TEXTURE_COORD_ARRAY_STRIDE:
    return _T("GL_TEXTURE_COORD_ARRAY_STRIDE");

  case GL_TEXTURE_COORD_ARRAY_TYPE:
    return _T("GL_TEXTURE_COORD_ARRAY_TYPE");

  case GL_POINT_SIZE_ARRAY_TYPE_OES:
    return _T("GL_POINT_SIZE_ARRAY_TYPE_OES");

  case GL_POINT_SIZE_ARRAY_STRIDE_OES:
    return _T("GL_POINT_SIZE_ARRAY_STRIDE_OES");

  case GL_VERTEX_ARRAY_BUFFER_BINDING:
    return _T("GL_VERTEX_ARRAY_BUFFER_BINDING");

  case GL_NORMAL_ARRAY_BUFFER_BINDING:
    return _T("GL_NORMAL_ARRAY_BUFFER_BINDING");

  case GL_COLOR_ARRAY_BUFFER_BINDING:
    return _T("GL_COLOR_ARRAY_BUFFER_BINDING");

  case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
    return _T("GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING");

  case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
    return _T("GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES");

  case GL_ARRAY_BUFFER_BINDING:
    return _T("GL_ARRAY_BUFFER_BINDING");

  case GL_ELEMENT_ARRAY_BUFFER_BINDING:
    return _T("GL_ELEMENT_ARRAY_BUFFER_BINDING");

  case GL_TEXTURE_BINDING_2D:
    return _T("GL_TEXTURE_BINDING_2D");

  case GL_UNPACK_ALIGNMENT:
    return _T("GL_UNPACK_ALIGNMENT");

  case GL_PACK_ALIGNMENT:
    return _T("GL_PACK_ALIGNMENT");

  case GL_STENCIL_CLEAR_VALUE:
    return _T("GL_STENCIL_CLEAR_VALUE");

  case GL_SCISSOR_BOX:
    return _T("GL_SCISSOR_BOX");

  case GL_VIEWPORT:
    return _T("GL_VIEWPORT");

  case GL_STENCIL_WRITEMASK:
    return _T("GL_STENCIL_WRITEMASK");

  case GL_STENCIL_VALUE_MASK:
    return _T("GL_STENCIL_VALUE_MASK");

  case GL_STENCIL_REF:
    return _T("GL_STENCIL_REF");

  case GL_ALPHA_TEST_FUNC:
    return _T("GL_ALPHA_TEST_FUNC");

  case GL_STENCIL_FUNC:
    return _T("GL_STENCIL_FUNC");

  case GL_STENCIL_FAIL:
    return _T("GL_STENCIL_FAIL");

  case GL_STENCIL_PASS_DEPTH_FAIL:
    return _T("GL_STENCIL_PASS_DEPTH_FAIL");

  case GL_STENCIL_PASS_DEPTH_PASS:
    return _T("GL_STENCIL_PASS_DEPTH_PASS");

  case GL_DEPTH_FUNC:
    return _T("GL_DEPTH_FUNC");

  case GL_LOGIC_OP_MODE:
    return _T("GL_LOGIC_OP_MODE");

  case GL_BLEND_SRC:
    return _T("GL_BLEND_SRC");

  case GL_BLEND_DST:
    return _T("GL_BLEND_DST");

  case GL_FOG_MODE:
    return _T("GL_FOG_MODE");

  case GL_SHADE_MODEL:
    return _T("GL_SHADE_MODEL");

  case GL_CULL_FACE_MODE:
    return _T("GL_CULL_FACE_MODE");

  case GL_FRONT_FACE:
    return _T("GL_FRONT_FACE");

  case GL_MATRIX_MODE:
    return _T("GL_MATRIX_MODE");

  case GL_ACTIVE_TEXTURE:
    return _T("GL_ACTIVE_TEXTURE");

  case GL_CLIENT_ACTIVE_TEXTURE:
    return _T("GL_CLIENT_ACTIVE_TEXTURE");

  case GL_PERSPECTIVE_CORRECTION_HINT:
    return _T("GL_PERSPECTIVE_CORRECTION_HINT");

  case GL_POINT_SMOOTH_HINT:
    return _T("GL_POINT_SMOOTH_HINT");

  case GL_LINE_SMOOTH_HINT:
    return _T("GL_LINE_SMOOTH_HINT");

  case GL_FOG_HINT:
    return _T("GL_FOG_HINT");

  case GL_GENERATE_MIPMAP_HINT:
    return _T("GL_GENERATE_MIPMAP_HINT");

  case GL_MAX_VERTEX_UNITS_OES:
    return _T("GL_MAX_VERTEX_UNITS_OES");

  case GL_MAX_PALETTE_MATRICES_OES:
    return _T("GL_MAX_PALETTE_MATRICES_OES");

  case GL_CURRENT_COLOR:
    return _T("GL_CURRENT_COLOR");

  case GL_DEPTH_CLEAR_VALUE:
    return _T("GL_DEPTH_CLEAR_VALUE");

  case GL_CURRENT_TEXTURE_COORDS:
    return _T("GL_CURRENT_TEXTURE_COORDS");

  case GL_CURRENT_NORMAL:
    return _T("GL_CURRENT_NORMAL");

  case GL_MODELVIEW_MATRIX:
    return _T("GL_MODELVIEW_MATRIX");

  case GL_PROJECTION_MATRIX:
    return _T("GL_PROJECTION_MATRIX");

  case GL_TEXTURE_MATRIX:
    return _T("GL_TEXTURE_MATRIX");

  case GL_FOG_COLOR:
    return _T("GL_FOG_COLOR");

  case GL_FOG_DENSITY:
    return _T("GL_FOG_DENSITY");

  case GL_FOG_START:
    return _T("GL_FOG_START");

  case GL_FOG_END:
    return _T("GL_FOG_END");

  case GL_ALPHA_TEST_REF:
    return _T("GL_ALPHA_TEST_REF");

  case GL_LIGHT_MODEL_AMBIENT:
    return _T("GL_LIGHT_MODEL_AMBIENT");

  case GL_COLOR_CLEAR_VALUE:
    return _T("GL_COLOR_CLEAR_VALUE");

  case GL_POLYGON_OFFSET_UNITS:
    return _T("GL_POLYGON_OFFSET_UNITS");

  case GL_POLYGON_OFFSET_FACTOR:
    return _T("GL_POLYGON_OFFSET_FACTOR");

  case GL_SAMPLE_COVERAGE_VALUE:
    return _T("GL_SAMPLE_COVERAGE_VALUE");

  case GL_POINT_SIZE_MIN:
    return _T("GL_POINT_SIZE_MIN");

  case GL_POINT_SIZE_MAX:
    return _T("GL_POINT_SIZE_MAX");

  case GL_POINT_FADE_THRESHOLD_SIZE:
    return _T("GL_POINT_FADE_THRESHOLD_SIZE");

  case GL_POINT_DISTANCE_ATTENUATION:
    return _T("GL_POINT_DISTANCE_ATTENUATION");

  case GL_DEPTH_RANGE:
    return _T("GL_DEPTH_RANGE");

  case GL_POINT_SIZE:
    return _T("GL_POINT_SIZE");

  case GL_LINE_WIDTH:
    return _T("GL_LINE_WIDTH");
  }

  return _T("UNKNOWN");
}

#endif

//////////////////////////////////////////////////////////////////////////////

class GLESDll {
public:
  GLESDll() : m_pDriver(nullptr) {
#ifndef NDEBUG
    {
      auto hr = glLogger.Open(_T("CocoGLES.log"), _T("w"));
      if (FAILED(hr)) {
        __glLogError(_T("CLogger::Open() failed, hr = %x.\r\n"), hr);
        return;
      }
    }
#endif

    // Initialize the driver
    auto err = CGLDriver::Create(&m_pDriver);
    if (__glFailed(err)) {
      __glLogError(_T("CGLDriver::Create() failed, err = %x.\r\n"), err);
      return;
    }
  }

  ~GLESDll() {
    __safeRelease(m_pDriver);    
  }

  auto driver() {
    return m_pDriver;
  }

private:
  CGLDriver* m_pDriver;
};

CLogger glLogger;
static GLESDll g_dll;

//////////////////////////////////////////////////////////////////////////////


GL_API GLenum GL_APIENTRY __glCreateSurface(
    const GLSurfaceDesc *pColorDesc, const GLSurfaceDesc *pDepthStencilDesc,
    __GLSurface *pGLSurface) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  auto driver = g_dll.driver();

  if (NULL == pGLSurface) {
    __glLogError(_T("__glCreateSurface() failed, pGLSurface is NULL.\r\n"));
    return GL_INVALID_VALUE;
  }

  CGLSurface *pSurface;
  err = CGLSurface::Create(&pSurface, pColorDesc, pDepthStencilDesc);
  if (__glFailed(err)) {
    __glLogError(_T("CGLSurface::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  uint32_t dwHandle;
  err = driver->AddObject(pSurface, HANDLE_SURFACE, driver, &dwHandle);
  if (__glFailed(err)) {
    __safeRelease(pSurface);
    __glLogError(_T("CGLDriver::AddObject() failed, err = %d.\r\n"), err);
    return err;
  }

  *pGLSurface = reinterpret_cast<__GLSurface>(dwHandle);

  return GL_NO_ERROR;
}


GL_API GLenum GL_APIENTRY
__glUpdateSurface(__GLSurface surface, const GLSurfaceDesc *pColorDesc,
                  const GLSurfaceDesc *pDepthStencilDesc) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  auto driver = g_dll.driver();

  CGLSurface *const pSurface = driver->TGetObject<CGLSurface *>(surface);
  if (NULL == pSurface) {
    __glLogError(_T("__glUpdateSurface() failed, invalid surface handle.\r\n"));
    return GL_INVALID_VALUE;
  }

  err = pSurface->Update(pColorDesc, pDepthStencilDesc);
  if (__glFailed(err)) {
    __glLogError(_T("CGLSurface::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  return GL_NO_ERROR;
}


GL_API GLenum GL_APIENTRY __glDestroySurface(__GLSurface surface) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  // Remove the surface object from the handle table
  CGLSurface *const pSurface = driver->TDeleteObject<CGLSurface *>(surface);
  if (NULL == pSurface) {
    __glLogError(_T("Invalid surface handle.\r\n"));
    return GL_INVALID_VALUE;
  }

  // Delete the surface
  pSurface->Release();

  return GL_NO_ERROR;
}


GL_API GLenum GL_APIENTRY __glCreateContext(__GLContext shared_context,
                                            __GLContext *pGLContext) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  auto driver = g_dll.driver();

  if (NULL == pGLContext) {
    __glLogError(_T("__glCreateContext() failed, pGLContext is NULL.\r\n"));
    return GL_INVALID_VALUE;
  }

  CGLContext *pCtxShared = NULL;
  if (shared_context) {
    pCtxShared = driver->TGetObject<CGLContext *>(shared_context);
    if (NULL == pCtxShared) {
      __glLogError(_T("Invalid context handle.\r\n"));
      return GL_INVALID_VALUE;
    }
  }

  CGLContext *pContext;
  err = CGLContext::Create(&pContext, driver->GetHandles(),
                           driver->GetRasterCache(), pCtxShared);
  if (__glFailed(err)) {
    __glLogError(_T("CGLContext::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  uint32_t dwHandle;
  err = driver->AddObject(pContext, HANDLE_CONTEXT, driver, &dwHandle);
  if (__glFailed(err)) {
    __safeRelease(pContext);
    __glLogError(_T("CGLDriver::AddObject() failed, err = %d.\r\n"), err);
    return err;
  }

  *pGLContext = reinterpret_cast<__GLContext>(dwHandle);

  return GL_NO_ERROR;
}


GL_API GLenum GL_APIENTRY __glDestroyContext(__GLContext context) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  // Remove the context object from the handle table
  auto pContext = driver->TDeleteObject<CGLContext *>(context);
  if (NULL == pContext) {
    __glLogError(_T("Invalid context handle.\r\n"));
    return GL_INVALID_VALUE;
  }

  // Delete the context
  pContext->Release();

  return GL_NO_ERROR;
}


GL_API GLenum GL_APIENTRY __glMakeCurrent(__GLContext context, __GLSurface draw,
                                          __GLSurface read) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  CGLContext *pContext = NULL;
  if (context) {
    pContext = driver->TGetObject<CGLContext *>(context);
    if (NULL == pContext) {
      __glLogError(_T("Invalid context handle.\r\n"));
      return GL_INVALID_VALUE;
    }
  }

  CGLSurface *pSurfDraw = NULL;
  if (draw) {
    pSurfDraw = driver->TGetObject<CGLSurface *>(draw);
    if (NULL == pSurfDraw) {
      __glLogError(_T("Invalid surface handle.\r\n"));
      return GL_INVALID_VALUE;
    }
  }

  CGLSurface *pSurfRead = NULL;
  if (read) {
    pSurfRead = driver->TGetObject<CGLSurface *>(read);
    if (NULL == pSurfRead) {
      __glLogError(_T("Invalid surface handle.\r\n"));
      return GL_INVALID_VALUE;
    }
  }

  driver->MakeCurrent(pContext, pSurfDraw, pSurfRead);

  return GL_NO_ERROR;
}


GL_API GLenum GL_APIENTRY __glCopyBuffers(const GLSurfaceDesc *pDstDesc,
                                          const GLSurfaceDesc *pSrcDesc) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  return CopyBuffers(*pDstDesc, 0, 0, pSrcDesc->Width, pSrcDesc->Height,
                     *pSrcDesc, 0, 0);
}


GL_API GLenum GL_APIENTRY __glBindTexImage(__GLSurface surface,
                                           bool bGenMipMaps) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  if (surface) {
    CGLSurface *const pSurface = driver->TGetObject<CGLSurface *>(surface);
    if (pSurface) {
      auto pContext = driver->GetCurrentContext();
      if (NULL == pContext) {
        __glLogError(_T("__glBindTexImage() failed, no active context.\r\n"));
        return GL_INVALID_OPERATION;
      }

      return pContext->BindTexImage(pSurface, bGenMipMaps);
    }
  }

  __glLogError(_T("__glBindTexImage() failed, invalid surface handle.\r\n"));
  return GL_INVALID_VALUE;
}


GL_API GLenum GL_APIENTRY __glReleaseTexImage(__GLSurface surface) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  if (surface) {
    CGLSurface *const pSurface = driver->TGetObject<CGLSurface *>(surface);
    if (pSurface) {
      auto pContext = driver->GetCurrentContext();
      if (NULL == pContext) {
        __glLogError(_T("__glBindTexImage() failed, no active context.\r\n"));
        return GL_INVALID_OPERATION;
      }

      return pContext->ReleaseTexImage(pSurface);
    }
  }

  __glLogError(_T("__glBindTexImage() failed, invalid surface handle.\r\n"));
  return GL_INVALID_VALUE;
}


GLenum __glSaveBitmap(__GLSurface surface, LPCTSTR lpszFilename) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  if (surface) {
    CGLSurface *const pSurface = driver->TGetObject<CGLSurface *>(surface);
    if (pSurface) {
      return pSurface->SaveBitmap(lpszFilename);
    }
  }

  __glLogError(_T("Invalid surface handle.\r\n"));

  return GL_INVALID_VALUE;
}

//////////////////////////////////////////////////////////////////////////////


GL_API void GL_APIENTRY glAlphaFunc(GLenum func, GLclampf ref) {
  __profileAPI(_T(" - %s( func=%s, ref=%f )\n"), _T(__FUNCTION__),
               FuncToString(func), ref);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->AlphaFunc(func, floatf(ref));
  }
}


GL_API void GL_APIENTRY glClearColor(GLclampf red, GLclampf green,
                                     GLclampf blue, GLclampf alpha) {
  __profileAPI(_T(" - %s( red=%f, green=%f, blue=%f, alpha%f )\n"),
               _T(__FUNCTION__), red, green, blue, alpha);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClearColor(floatf(red), floatf(green), floatf(blue),
                         floatf(alpha));
  }
}


GL_API void GL_APIENTRY glClearDepthf(GLclampf depth) {
  __profileAPI(_T(" - %s( depth=%f )\n"), _T(__FUNCTION__), depth);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClearDepth(floatf(depth));
  }
}


GL_API void GL_APIENTRY glClipPlanef(GLenum plane, const GLfloat *pEquation) {
  __profileAPI(_T(" - %s( plane=%s, pEquation=0x%p )\n"), _T(__FUNCTION__),
               PlaneToString(plane), pEquation);

  if (NULL == pEquation)
    return;

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->ClipPlane(plane,
                        VECTOR4(floatf(pEquation[0]), floatf(pEquation[1]),
                                floatf(pEquation[2]), floatf(pEquation[3])));
#else
    pContext->ClipPlane(plane, *reinterpret_cast<const VECTOR4 *>(pEquation));
#endif
  }
}


GL_API void GL_APIENTRY glColor4f(GLfloat red, GLfloat green, GLfloat blue,
                                  GLfloat alpha) {
  __profileAPI(_T(" - %s( red=%f, green=%f, blue=%f, alpha=%f )\n"),
               _T(__FUNCTION__), red, green, blue, alpha);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Color(floatf(red), floatf(green), floatf(blue), floatf(alpha));
  }
}


GL_API void GL_APIENTRY glDepthRangef(GLclampf zNear, GLclampf zFar) {
  __profileAPI(_T(" - %s( zNear=%f, zFar=%f )\n"), _T(__FUNCTION__), zNear,
               zFar);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DepthRange(floatf(zNear), floatf(zFar));
  }
}


GL_API void GL_APIENTRY glFogf(GLenum pname, GLfloat param) {
  __profileAPI(_T(" - %s( pname=%s, param=%f )\n"), _T(__FUNCTION__),
               FogParamToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TFog<float>(pname, &param);
  }
}


GL_API void GL_APIENTRY glFogfv(GLenum pname, const GLfloat *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               FogParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TFog<float>(pname, pParams);
  }
}


GL_API void GL_APIENTRY glFrustumf(GLfloat left, GLfloat right, GLfloat bottom,
                                   GLfloat top, GLfloat zNear, GLfloat zFar) {
  __profileAPI(
      _T(" - %s( left=%f, right=%f, bottom=%f, top=%f, zNear=%f, zFar=%f )\n"),
      _T(__FUNCTION__), left, right, bottom, top, zNear, zFar);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Frustum(floatf(left), floatf(right), floatf(bottom), floatf(top),
                      floatf(zNear), floatf(zFar));
  }
}


GL_API void GL_APIENTRY glGetClipPlanef(GLenum plane, GLfloat eqn[4]) {
  __profileAPI(_T(" - %s( plane=%s, eqn[4]=0x%p)\n"), _T(__FUNCTION__),
               PlaneToString(plane), eqn);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetClipPlane<float>(plane, eqn);
  }
}


GL_API void GL_APIENTRY glGetFloatv(GLenum pname, GLfloat *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               AttributeToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGet<float>(pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetLightfv(GLenum light, GLenum pname,
                                     GLfloat *pParams) {
  __profileAPI(_T(" - %s( light=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), LightToString(light),
               LightParamToString(pname), pParams);

  if (NULL == pParams)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetLight<float>(light, pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetMaterialfv(GLenum face, GLenum pname,
                                        GLfloat *pParams) {
  __profileAPI(_T(" - %s( face=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), MaterialFaceToString(face),
               MaterialParamToString(pname), pParams);

  if (NULL == pParams)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetMaterial<float>(face, pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetTexEnvfv(GLenum env, GLenum pname,
                                      GLfloat *pParams) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TexEnvToString(env),
               TexEnvParamToString(pname), pParams);

  if (NULL == pParams)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetTexEnv<float>(env, pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetTexParameterfv(GLenum target, GLenum pname,
                                            GLfloat *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target),
               TexParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetTexParameter<float>(target, pname, pParams);
  }
}


GL_API void GL_APIENTRY glLightModelf(GLenum pname, GLfloat param) {
  __profileAPI(_T(" - %s( pname=%s, param=%f )\n"), _T(__FUNCTION__),
               LightModelToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLightModel<float>(pname, &param);
  }
}


GL_API void GL_APIENTRY glLightModelfv(GLenum pname, const GLfloat *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=(%f, %f, %f, %f) )\n"),
               _T(__FUNCTION__), LightModelToString(pname), pParams[0],
               pParams[1], pParams[2], pParams[3]);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLightModel<float>(pname, pParams);
  }
}


GL_API void GL_APIENTRY glLightf(GLenum light, GLenum pname, GLfloat param) {
  __profileAPI(_T(" - %s( light=%s, pname=%s, param=%f )\n"), _T(__FUNCTION__),
               LightToString(light), LightParamToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLight<float>(light, pname, &param);
  }
}


GL_API void GL_APIENTRY glLightfv(GLenum light, GLenum pname,
                                  const GLfloat *pParams) {
  __profileAPI(_T(" - %s( light=%s, pname=%s, pParams=(%f, %f, %f, %f) )\n"),
               _T(__FUNCTION__), LightToString(light),
               LightParamToString(pname), pParams[0], pParams[1], pParams[2],
               pParams[3]);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLight<float>(light, pname, pParams);
  }
}


GL_API void GL_APIENTRY glLineWidth(GLfloat width) {
  __profileAPI(_T(" - %s( width=%f )\n"), _T(__FUNCTION__), width);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->LineWidth(floatf(width));
  }
}


GL_API void GL_APIENTRY glLoadMatrixf(const GLfloat *pM) {
  __profileAPI(_T(" - %s( pM=(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, ")
               _T("%f, %f, %f, %f) )\n"),
               _T(__FUNCTION__), pM[0], pM[1], pM[2], pM[3], pM[4], pM[5],
               pM[6], pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13],
               pM[14], pM[15]);

  if (NULL == pM)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->LoadMatrix(MATRIX44(
        floatf(pM[0]), floatf(pM[1]), floatf(pM[2]), floatf(pM[3]),
        floatf(pM[4]), floatf(pM[5]), floatf(pM[6]), floatf(pM[7]),
        floatf(pM[8]), floatf(pM[9]), floatf(pM[10]), floatf(pM[11]),
        floatf(pM[12]), floatf(pM[13]), floatf(pM[14]), floatf(pM[15])));
#else
    pContext->LoadMatrix(*reinterpret_cast<const MATRIX44 *>(pM));
#endif
  }
}


GL_API void GL_APIENTRY glMaterialf(GLenum face, GLenum pname, GLfloat param) {
  __profileAPI(_T(" - %s( face=%s, pname=%s, param=%f )\n"), _T(__FUNCTION__),
               MaterialFaceToString(face), MaterialParamToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TMaterial<float>(face, pname, &param);
  }
}


GL_API void GL_APIENTRY glMaterialfv(GLenum face, GLenum pname,
                                     const GLfloat *pParams) {
  __profileAPI(_T(" - %s( face=%s, pname=%s, pParams=(%f, %f, %f, %f) )\n"),
               _T(__FUNCTION__), MaterialFaceToString(face),
               MaterialParamToString(pname), pParams[0], pParams[1], pParams[2],
               pParams[3]);

  if (NULL == pParams)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TMaterial<float>(face, pname, pParams);
  }
}


GL_API void GL_APIENTRY glMultMatrixf(const GLfloat *pM) {
  __profileAPI(_T(" - %s( pM=(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, ")
               _T("%f, %f, %f, %f) )\n"),
               _T(__FUNCTION__), pM[0], pM[1], pM[2], pM[3], pM[4], pM[5],
               pM[6], pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13],
               pM[14], pM[15]);

  if (NULL == pM)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->Multiply(MATRIX44(
        floatf(pM[0]), floatf(pM[1]), floatf(pM[2]), floatf(pM[3]),
        floatf(pM[4]), floatf(pM[5]), floatf(pM[6]), floatf(pM[7]),
        floatf(pM[8]), floatf(pM[9]), floatf(pM[10]), floatf(pM[11]),
        floatf(pM[12]), floatf(pM[13]), floatf(pM[14]), floatf(pM[15])));
#else
    pContext->Multiply(*reinterpret_cast<const MATRIX44 *>(pM));
#endif
  }
}


GL_API void GL_APIENTRY glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t,
                                          GLfloat r, GLfloat q) {
  __profileAPI(_T(" - %s( target=%s, s=%f, t=%f, r=%f, q=%f )\n"),
               _T(__FUNCTION__), TextureToString(target), s, t, r, q);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->MultiTexCoord(target, floatf(s), floatf(t), floatf(r), floatf(q));
  }
}


GL_API void GL_APIENTRY glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz) {
  __profileAPI(_T(" - %s( nx=%f, ny=%f, nz=%f )\n"), _T(__FUNCTION__), nx, ny, nz);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Normal(floatf(nx), floatf(ny), floatf(nz));
  }
}


GL_API void GL_APIENTRY glOrthof(GLfloat left, GLfloat right, GLfloat bottom,
                                 GLfloat top, GLfloat zNear, GLfloat zFar) {
  __profileAPI(
      _T(" - %s( left=%f, right=%f, bottom=%f, top=%f, zNear=%f, zFar=%f )\n"),
      _T(__FUNCTION__), left, right, bottom, top, zNear, zFar);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Ortho(floatf(left), floatf(right), floatf(bottom), floatf(top),
                    floatf(zNear), floatf(zFar));
  }
}


GL_API void GL_APIENTRY glPointParameterf(GLenum pname, GLfloat param) {
  __profileAPI(_T(" - %s( pname=%s, param=%f )\n"), _T(__FUNCTION__),
               PNameToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TPointParameter<float>(pname, &param);
  }
}


GL_API void GL_APIENTRY glPointParameterfv(GLenum pname,
                                           const GLfloat *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               PNameToString(pname), pParams);

  auto driver = g_dll.driver();

  if (NULL == pParams)
    return;

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TPointParameter<float>(pname, pParams);
  }
}


GL_API void GL_APIENTRY glPointSize(GLfloat size) {
  __profileAPI(_T(" - %s( size=%f )\n"), _T(__FUNCTION__), size);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PointSize(floatf(size));
  }
}


GL_API void GL_APIENTRY glPolygonOffset(GLfloat factor, GLfloat units) {
  __profileAPI(_T(" - %s( factor=%f, units=%f )\n"), _T(__FUNCTION__), factor,
               units);
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PolygonOffset(floatf(factor), floatf(units));
  }
}


GL_API void GL_APIENTRY glRotatef(GLfloat angle, GLfloat x, GLfloat y,
                                  GLfloat z) {
  __profileAPI(_T(" - %s( angle=%f, x=%f, y=%f, z=%f )\n"), _T(__FUNCTION__),
               angle, x, y, z);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Rotate(floatf(angle), floatf(x), floatf(y), floatf(z));
  }
}


GL_API void GL_APIENTRY glScalef(GLfloat x, GLfloat y, GLfloat z) {
  __profileAPI(_T(" - %s( x=%f, y=%f, z=%f )\n"), _T(__FUNCTION__), x, y, z);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Scale(floatf(x), floatf(y), floatf(z));
  }
}


GL_API void GL_APIENTRY glTexEnvf(GLenum env, GLenum pname, GLfloat param) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, param=%s )\n"), _T(__FUNCTION__),
               TexEnvToString(env), TexEnvParamToString(pname),
               TexEnvValueToString((GLint)param));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexEnv<float>(env, pname, &param);
  }
}


GL_API void GL_APIENTRY glTexEnvfv(GLenum env, GLenum pname,
                                   const GLfloat *pParams) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TexEnvToString(env),
               TexEnvParamToString(pname), pParams);

  if (NULL == pParams)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexEnv<float>(env, pname, pParams);
  }
}


GL_API void GL_APIENTRY glTexParameterf(GLenum target, GLenum pname,
                                        GLfloat param) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, param=%s )\n"), _T(__FUNCTION__),
               TextureTypeToString(target), TexParamToString(pname),
               TexParamValueToString((GLint)param));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexParameter<float>(target, pname, &param);
  }
}


GL_API void GL_APIENTRY glTexParameterfv(GLenum target, GLenum pname,
                                         const GLfloat *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target),
               TexParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexParameter<float>(target, pname, pParams);
  }
}


GL_API void GL_APIENTRY glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
  __profileAPI(_T(" - %s( x=%f, y=%f, z=%f )\n"), _T(__FUNCTION__), x, y, z);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Translate(floatf(x), floatf(y), floatf(z));
  }
}

/* Available in both Common and Common-Lite profiles */


GL_API void GL_APIENTRY glActiveTexture(GLenum texture) {
  __profileAPI(_T(" - %s( texture=%s )\n"), _T(__FUNCTION__),
               TextureToString(texture));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ActiveTexture(texture);
  }
}


GL_API void GL_APIENTRY glAlphaFuncx(GLenum func, GLclampx ref) {
  __profileAPI(_T(" - %s( func=%s, ref=%f )\n"), _T(__FUNCTION__),
               FuncToString(func), ref);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->AlphaFunc(func, Math::TCast<floatf>(fixed16::Make(ref)));
  }
}


GL_API void GL_APIENTRY glBindBuffer(GLenum target, GLuint buffer) {
  __profileAPI(_T(" - %s( target=%s, buffer=%d )\n"), _T(__FUNCTION__),
               BufferToString(target), buffer);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->BindBuffer(target, buffer);
  }
}


GL_API void GL_APIENTRY glBindTexture(GLenum target, GLuint texture) {
  __profileAPI(_T(" - %s( target=%s, texture=%d )\n"), _T(__FUNCTION__),
               TextureTypeToString(target), texture);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->BindTexture(target, texture);
  }
}


GL_API void GL_APIENTRY glBlendFunc(GLenum sfactor, GLenum dfactor) {
  __profileAPI(_T(" - %s( sfactor=%s, dfactor=%s )\n"), _T(__FUNCTION__),
               BlendFuncToString(sfactor), BlendFuncToString(dfactor));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->BlendFunc(sfactor, dfactor);
  }
}


GL_API void GL_APIENTRY glBufferData(GLenum target, GLsizeiptr size,
                                     const GLvoid *pData, GLenum usage) {
  __profileAPI(_T(" - %s( target=%s, size=%d, pData=0x%p, usage=%s )\n"),
               _T(__FUNCTION__), BufferToString(target), size, pData,
               UsageToString(usage));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->BufferData(target, size, pData, usage);
  }
}


GL_API void GL_APIENTRY glBufferSubData(GLenum target, GLintptr offset,
                                        GLsizeiptr size, const GLvoid *pData) {
  __profileAPI(_T(" - %s( target=%s, offset=%d, size=%d, pData=0x%p )\n"),
               _T(__FUNCTION__), BufferToString(target), offset, size, pData);

  if (NULL == pData)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->BufferSubData(target, offset, size, pData);
  }
}


GL_API void GL_APIENTRY glClear(GLbitfield mask) {
  __profileAPI(_T(" - %s( mask=0x%x )\n"), _T(__FUNCTION__), mask);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Clear(mask);
  }
}


GL_API void GL_APIENTRY glClearColorx(GLclampx red, GLclampx green,
                                      GLclampx blue, GLclampx alpha) {
  __profileAPI(_T(" - %s( red=%f, green=%f, blue=%f, alpha=%f )\n"),
               _T(__FUNCTION__), red, green, blue, alpha);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClearColor(Math::TCast<floatf>(fixed16::Make(red)),
                         Math::TCast<floatf>(fixed16::Make(green)),
                         Math::TCast<floatf>(fixed16::Make(blue)),
                         Math::TCast<floatf>(fixed16::Make(alpha)));
  }
}


GL_API void GL_APIENTRY glClearDepthx(GLclampx depth) {
  __profileAPI(_T(" - %s( depth=%f )\n"), _T(__FUNCTION__), depth);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClearDepth(Math::TCast<floatf>(fixed16::Make(depth)));
  }
}


GL_API void GL_APIENTRY glClearStencil(GLint s) {
  __profileAPI(_T(" - %s( s=%d )\n"), _T(__FUNCTION__), s);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClearStencil(s);
  }
}


GL_API void GL_APIENTRY glClientActiveTexture(GLenum texture) {
  __profileAPI(_T(" - %s( texture=%s )\n"), _T(__FUNCTION__),
               TextureToString(texture));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClientActiveTexture(texture);
  }
}


GL_API void GL_APIENTRY glClipPlanex(GLenum plane, const GLfixed *pEquation) {
  __profileAPI(_T(" - %s( plane=%d, pEquation=0x%p )\n"), _T(__FUNCTION__),
               plane, pEquation);

  if (NULL == pEquation)
    return;
    
  auto driver = g_dll.driver();  

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->ClipPlane(plane, *reinterpret_cast<const VECTOR4 *>(pEquation));
#else
    pContext->ClipPlane(
        plane, VECTOR4(Math::TCast<floatf>(fixed16::Make(pEquation[0])),
                       Math::TCast<floatf>(fixed16::Make(pEquation[1])),
                       Math::TCast<floatf>(fixed16::Make(pEquation[2])),
                       Math::TCast<floatf>(fixed16::Make(pEquation[3]))));
#endif
  }
}


GL_API void GL_APIENTRY glColor4ub(GLubyte red, GLubyte green, GLubyte blue,
                                   GLubyte alpha) {
  __profileAPI(_T(" - %s( red=%d, green=%d, blue=%d, alpha=%d )\n"),
               _T(__FUNCTION__), red, green, blue, alpha);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Color(
        Math::TFromUNORM8<floatf>(red), Math::TFromUNORM8<floatf>(green),
        Math::TFromUNORM8<floatf>(blue), Math::TFromUNORM8<floatf>(alpha));
  }
}


GL_API void GL_APIENTRY glColor4x(GLfixed red, GLfixed green, GLfixed blue,
                                  GLfixed alpha) {
  __profileAPI(_T(" - %s( red=%d, green=%d, blue=%d, alpha=%d )\n"),
               _T(__FUNCTION__), red, green, blue, alpha);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Color(Math::TCast<floatf>(fixed16::Make(red)),
                    Math::TCast<floatf>(fixed16::Make(green)),
                    Math::TCast<floatf>(fixed16::Make(blue)),
                    Math::TCast<floatf>(fixed16::Make(alpha)));
  }
}


GL_API void GL_APIENTRY glColorMask(GLboolean red, GLboolean green,
                                    GLboolean blue, GLboolean alpha) {
  __profileAPI(_T(" - %s( red=%d, green=%d, blue=%d, alpha=%d )\n"),
               _T(__FUNCTION__), red, green, blue, alpha);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ColorMask(red, green, blue, alpha);
  }
}


GL_API void GL_APIENTRY glColorPointer(GLint size, GLenum type, GLsizei stride,
                                       const GLvoid *pPointer) {
  __profileAPI(_T(" - %s( size=%d, type=%s, stride=%d, pPointer=0x%p )\n"),
               _T(__FUNCTION__), size, TypeToString(type), stride, pPointer);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ColorPointer(size, type, stride, pPointer);
  }
}


GL_API void GL_APIENTRY glCompressedTexImage2D(GLenum target, GLint level,
                                               GLenum internalformat,
                                               GLsizei width, GLsizei height,
                                               GLint border, GLsizei imageSize,
                                               const GLvoid *pData) {
  __profileAPI(_T(" - %s( target=%s, level=%d, internalformat=%s, width=%d, ")
               _T("height=%d, border=%d, imageSize=%d, pDdata=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target), level,
               FormatToString(internalformat), width, height, border, imageSize,
               pData);

  if (NULL == pData)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->CompressedTexImage2D(target, level, internalformat, width, height,
                                   border, imageSize, pData);
  }
}


GL_API void GL_APIENTRY glCompressedTexSubImage2D(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
    GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *pData) {
  __profileAPI(
      _T(" - %s( target=%s, level=%d, xoffset=%d, yoffset=%d, width=%d, ")
      _T("height=%d, format=%s, imageSize=%d, pData=0x%p )\n"),
      _T(__FUNCTION__), TextureTypeToString(target), level, xoffset, yoffset,
      width, height, FormatToString(format), imageSize, pData);

  if (NULL == pData)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->CompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                      height, format, imageSize, pData);
  }
}


GL_API void GL_APIENTRY glCopyTexImage2D(GLenum target, GLint level,
                                         GLenum internalformat, GLint x,
                                         GLint y, GLsizei width, GLsizei height,
                                         GLint border) {
  __profileAPI(_T(" - %s( target=%s, level=%d, internalformat=%s, x=%d, y=%d, ")
               _T("width=%d, height=%d, border=%d )\n"),
               _T(__FUNCTION__), TextureTypeToString(target), level,
               FormatToString(internalformat), x, y, width, height, border);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->CopyTexImage2D(target, level, internalformat, x, y, width, height,
                             border);
  }
}


GL_API void GL_APIENTRY glCopyTexSubImage2D(GLenum target, GLint level,
                                            GLint xoffset, GLint yoffset,
                                            GLint x, GLint y, GLsizei width,
                                            GLsizei height) {
  __profileAPI(_T(" - %s( target=%s, level=%d, xoffset=%d, yoffset=%d, x=%d, ")
               _T("y=%d, width=%d, height=%d )\n"),
               _T(__FUNCTION__), TextureTypeToString(target), level, xoffset,
               yoffset, x, y, width, height);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width,
                                height);
  }
}


GL_API void GL_APIENTRY glCullFace(GLenum mode) {
  __profileAPI(_T(" - %s( mode=%s )\n"), _T(__FUNCTION__),
               CullFaceToString(mode));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->CullFace(mode);
  }
}


GL_API void GL_APIENTRY glDeleteBuffers(GLsizei n, const GLuint *pBuffers) {
  __profileAPI(_T(" - %s( n=%d, pBuffers=0x%p )\n"), _T(__FUNCTION__), n,
               pBuffers);

  if (NULL == pBuffers)
    return;

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DeleteBuffers(n, pBuffers);
  }
}


GL_API void GL_APIENTRY glDeleteTextures(GLsizei n, const GLuint *pTextures) {
  __profileAPI(_T(" - %s( n=%d, pTextures=0x%p )\n"), _T(__FUNCTION__), n,
               pTextures);

  if (NULL == pTextures)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DeleteTextures(n, pTextures);
  }
}


GL_API void GL_APIENTRY glDepthFunc(GLenum func) {
  __profileAPI(_T(" - %s( func=%s )\n"), _T(__FUNCTION__), FuncToString(func));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DepthFunc(func);
  }
}


GL_API void GL_APIENTRY glDepthMask(GLboolean flag) {
  __profileAPI(_T(" - %s( flag=%d )\n"), _T(__FUNCTION__), flag);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DepthMask(flag);
  }
}


GL_API void GL_APIENTRY glDepthRangex(GLclampx zNear, GLclampx zFar) {
  __profileAPI(_T(" - %s( zNear=%f, zfar=%f )\n"), _T(__FUNCTION__), zNear,
               zFar);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DepthRange(Math::TCast<floatf>(fixed16::Make(zNear)),
                         Math::TCast<floatf>(fixed16::Make(zFar)));
  }
}


GL_API void GL_APIENTRY glDisable(GLenum cap) {
  __profileAPI(_T(" - %s( cap=%s )\n"), _T(__FUNCTION__), CapToString(cap));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Activate(cap, false);
  }
}


GL_API void GL_APIENTRY glDisableClientState(GLenum array) {
  __profileAPI(_T(" - %s( array=%s )\n"), _T(__FUNCTION__),
               ClientStateToString(array));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClientState(array, false);
  }
}


GL_API void GL_APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count) {
  __profileAPI(_T(" - %s( mode=%s, first=%d, count=%d )\n"), _T(__FUNCTION__),
               PrimitiveTypeToString(mode), first, count);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DrawArrays(mode, first, count);
  }
}


GL_API void GL_APIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type,
                                       const GLvoid *pIndices) {
  __profileAPI(_T(" - %s( mode=%s, count=%d, type=%s, pIndices=0x%p )\n"),
               _T(__FUNCTION__), PrimitiveTypeToString(mode), count,
               TypeToString(type), pIndices);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->DrawElements(mode, count, type, pIndices);
  }
}


GL_API void GL_APIENTRY glEnable(GLenum cap) {
  __profileAPI(_T(" - %s( cap=%s )\n"), _T(__FUNCTION__), CapToString(cap));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Activate(cap, true);
  }
}


GL_API void GL_APIENTRY glEnableClientState(GLenum array) {
  __profileAPI(_T(" - %s( array=%s )\n"), _T(__FUNCTION__),
               ClientStateToString(array));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ClientState(array, true);
  }
}


GL_API void GL_APIENTRY glFinish() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Finish();
  }
}


GL_API void GL_APIENTRY glFlush() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Flush();
  }
}


GL_API void GL_APIENTRY glFogx(GLenum pname, GLfixed param) {
  __profileAPI(_T(" - %s( pname=%s, param=0x%x )\n"), _T(__FUNCTION__),
               FogToString(pname), param);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TFog<fixed16>(pname, reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glFogxv(GLenum pname, const GLfixed *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               FogToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TFog<fixed16>(pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glFrontFace(GLenum mode) {
  __profileAPI(_T(" - %s( mode=%s )\n"), _T(__FUNCTION__),
               FrontFaceToString(mode));

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->FrontFace(mode);
  }
}


GL_API void GL_APIENTRY glFrustumx(GLfixed left, GLfixed right, GLfixed bottom,
                                   GLfixed top, GLfixed zNear, GLfixed zFar) {
  __profileAPI(
      _T(" - %s( left=%d, right=%d, bottom=%d, top=%d, zNear=%d, zFar=%d )\n"),
      _T(__FUNCTION__), left, right, bottom, top, zNear, zFar);

  auto driver = g_dll.driver();
  
  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Frustum(Math::TCast<floatf>(fixed16::Make(left)),
                      Math::TCast<floatf>(fixed16::Make(right)),
                      Math::TCast<floatf>(fixed16::Make(bottom)),
                      Math::TCast<floatf>(fixed16::Make(top)),
                      Math::TCast<floatf>(fixed16::Make(zNear)),
                      Math::TCast<floatf>(fixed16::Make(zFar)));
  }
}


GL_API void GL_APIENTRY glGetBooleanv(GLenum pname, GLboolean *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               AttributeToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGet<bool>(pname, reinterpret_cast<bool *>(pParams));
  }
}


GL_API void GL_APIENTRY glGetBufferParameteriv(GLenum target, GLenum pname,
                                               GLint *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), BufferToString(target),
               BufferParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->GetBufferParameter(target, pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetClipPlanex(GLenum plane, GLfixed eqn[4]) {
  __profileAPI(_T(" - %s( plane=%s, eqn[4]=0x%p )\n"), _T(__FUNCTION__),
               PlaneToString(plane), eqn);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetClipPlane<fixed16>(plane, reinterpret_cast<fixed16 *>(eqn));
  }
}


GL_API void GL_APIENTRY glGenBuffers(GLsizei n, GLuint *pBuffers) {
  __profileAPI(_T(" - %s( n=%d, pBuffers=0x%p )\n"), _T(__FUNCTION__), n,
               pBuffers);

  if (NULL == pBuffers)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->GenBuffers(n, pBuffers);
  }
}


GL_API void GL_APIENTRY glGenTextures(GLsizei n, GLuint *pTextures) {
  __profileAPI(_T(" - %s( n=%d, pTextures=0x%p )\n"), _T(__FUNCTION__), n,
               pTextures);

  if (NULL == pTextures)
    return;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->GenTextures(n, pTextures);
  }
}


GL_API GLenum GL_APIENTRY glGetError() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err = GL_NO_ERROR;

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    err = pContext->GetError();
  }

  __glLog(_T(" - glGetError()=%d\r\n"), err);

  return err;
}


GL_API void GL_APIENTRY glGetFixedv(GLenum pname, GLfixed *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               AttributeToString(pname), pParams);

  if (NULL == pParams)
    return;
  
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGet<fixed16>(pname, reinterpret_cast<fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glGetIntegerv(GLenum pname, GLint *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               AttributeToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGet<int>(pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetLightxv(GLenum light, GLenum pname,
                                     GLfixed *pParams) {
  __profileAPI(_T(" - %s( light=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), LightToString(light),
               LightParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetLight<fixed16>(light, pname,
                                 reinterpret_cast<fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glGetMaterialxv(GLenum face, GLenum pname,
                                        GLfixed *pParams) {
  __profileAPI(_T(" - %s( face=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), MaterialFaceToString(face),
               MaterialParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetMaterial<fixed16>(face, pname,
                                    reinterpret_cast<fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glGetPointerv(GLenum pname, GLvoid **ppParams) {
  __profileAPI(_T(" - %s( pname=%s, ppParams=0x%p )\n"), _T(__FUNCTION__),
               PointerToString(pname), ppParams);

  if (NULL == ppParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->GetPointer(ppParams, pname);
  }
}


GL_API const GLubyte *GL_APIENTRY glGetString(GLenum name) {
  __profileAPI(_T(" - %s( name=%s )\n"), _T(__FUNCTION__),
               StringToString(name));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    return pContext->GetString(name);
  }

  return NULL;
}


GL_API void GL_APIENTRY glGetTexEnviv(GLenum env, GLenum pname,
                                      GLint *pParams) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TexEnvToString(env),
               TexEnvParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetTexEnv<int>(env, pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetTexEnvxv(GLenum env, GLenum pname,
                                      GLfixed *pParams) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TexEnvToString(env),
               TexEnvParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetTexEnv<fixed16>(env, pname,
                                  reinterpret_cast<fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glGetTexParameteriv(GLenum target, GLenum pname,
                                            GLint *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target),
               TexParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetTexParameter<int>(target, pname, pParams);
  }
}


GL_API void GL_APIENTRY glGetTexParameterxv(GLenum target, GLenum pname,
                                            GLfixed *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target),
               TexParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TGetTexParameter<fixed16>(target, pname,
                                        reinterpret_cast<fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glHint(GLenum target, GLenum mode) {
  __profileAPI(_T(" - %s( target=%s, mode=%s )\n"), _T(__FUNCTION__),
               HintTargetToString(target), HintModeToString(mode));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    return pContext->Hint(target, mode);
  }
}


GL_API GLboolean GL_APIENTRY glIsBuffer(GLuint buffer) {
  __profileAPI(_T(" - %s( buffer=%d )\n"), _T(__FUNCTION__), buffer);

  auto driver = g_dll.driver();

  bool bValue = false;

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    bValue = pContext->IsBuffer(buffer);
  }

  __glLog(_T(" - glIsBuffer()=%d\r\n"), bValue);

  return bValue;
}


GL_API GLboolean GL_APIENTRY glIsEnabled(GLenum cap) {
  __profileAPI(_T(" - %s( cap=%s )\n"), _T(__FUNCTION__), CapToString(cap));

  auto driver = g_dll.driver();

  bool bValue = false;

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    bValue = pContext->IsEnabled(cap);
  }

  __glLog(_T(" - glIsEnabled()=%d\r\n"), bValue);

  return bValue;
}


GL_API GLboolean GL_APIENTRY glIsTexture(GLuint texture) {
  __profileAPI(_T(" - %s( texture=%d )\n"), _T(__FUNCTION__), texture);

  auto driver = g_dll.driver();

  bool bValue = false;

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    bValue = pContext->IsTexture(texture);
  }

  __glLog(_T(" - glIsTexture()=%d\r\n"), bValue);

  return bValue;
}


GL_API void GL_APIENTRY glLightModelx(GLenum pname, GLfixed param) {
  __profileAPI(_T(" - %s( pname=%s, param=%d )\n"), _T(__FUNCTION__),
               LightModelToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLightModel<fixed16>(pname,
                                   reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glLightModelxv(GLenum pname, const GLfixed *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=(0x%x ,0x%x, 0x%x, 0x%x) )\n"),
               _T(__FUNCTION__), LightModelToString(pname), pParams[0],
               pParams[1], pParams[2], pParams[3]);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLightModel<fixed16>(pname,
                                   reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glLightx(GLenum light, GLenum pname, GLfixed param) {
  __profileAPI(_T(" - %s( light=%s, pname=%s, param=%d )\n"), _T(__FUNCTION__),
               LightToString(light), LightParamToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLight<fixed16>(light, pname,
                              reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glLightxv(GLenum light, GLenum pname,
                                  const GLfixed *pParams) {
  __profileAPI(
      _T(" - %s( light=%s, pname=%s, pParams=(0x%x, 0x%x, 0x%x, 0x%x) )\n"),
      _T(__FUNCTION__), LightToString(light), LightParamToString(pname),
      pParams[0], pParams[1], pParams[2], pParams[3]);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TLight<fixed16>(light, pname,
                              reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glLineWidthx(GLfixed width) {
  __profileAPI(_T(" - %s( width=%d )\n"), _T(__FUNCTION__), width);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->LineWidth(Math::TCast<floatf>(fixed16::Make(width)));
  }
}


GL_API void GL_APIENTRY glLoadIdentity() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->LoadIdentity();
  }
}


GL_API void GL_APIENTRY glLoadMatrixx(const GLfixed *pM) {
  __profileAPI(_T(" - %s( pM=(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, ")
               _T("0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x) )\n"),
               _T(__FUNCTION__), pM[0], pM[1], pM[2], pM[3], pM[4], pM[5],
               pM[6], pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13],
               pM[14], pM[15]);

  if (NULL == pM)
    return;

  auto driver = g_dll.driver();    

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->LoadMatrix(*reinterpret_cast<const MATRIX44 *>(pM));
#else
    pContext->LoadMatrix(MATRIX44(Math::TCast<floatf>(fixed16::Make(pM[0])),
                                  Math::TCast<floatf>(fixed16::Make(pM[1])),
                                  Math::TCast<floatf>(fixed16::Make(pM[2])),
                                  Math::TCast<floatf>(fixed16::Make(pM[3])),
                                  Math::TCast<floatf>(fixed16::Make(pM[4])),
                                  Math::TCast<floatf>(fixed16::Make(pM[5])),
                                  Math::TCast<floatf>(fixed16::Make(pM[6])),
                                  Math::TCast<floatf>(fixed16::Make(pM[7])),
                                  Math::TCast<floatf>(fixed16::Make(pM[8])),
                                  Math::TCast<floatf>(fixed16::Make(pM[9])),
                                  Math::TCast<floatf>(fixed16::Make(pM[10])),
                                  Math::TCast<floatf>(fixed16::Make(pM[11])),
                                  Math::TCast<floatf>(fixed16::Make(pM[12])),
                                  Math::TCast<floatf>(fixed16::Make(pM[13])),
                                  Math::TCast<floatf>(fixed16::Make(pM[14])),
                                  Math::TCast<floatf>(fixed16::Make(pM[15]))));
#endif
  }
}


GL_API void GL_APIENTRY glLogicOp(GLenum opcode) {
  __profileAPI(_T(" - %s( opcode=%s )\n"), _T(__FUNCTION__),
               LogicOpToString(opcode));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->LogicOp(opcode);
  }
}


GL_API void GL_APIENTRY glMaterialx(GLenum face, GLenum pname, GLfixed param) {
  __profileAPI(_T(" - %s( face=%s, pname=%s, param=%d )\n"), _T(__FUNCTION__),
               MaterialFaceToString(face), MaterialParamToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TMaterial<fixed16>(face, pname,
                                 reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glMaterialxv(GLenum face, GLenum pname,
                                     const GLfixed *pParams) {
  __profileAPI(
      _T(" - %s( face=%s, pname=%s, pParams=(0x%x, 0x%x, 0x%x, 0x%x) )\n"),
      _T(__FUNCTION__), MaterialFaceToString(face),
      MaterialParamToString(pname), pParams[0], pParams[1], pParams[2],
      pParams[3]);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TMaterial<fixed16>(face, pname,
                                 reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glMatrixMode(GLenum mode) {
  __profileAPI(_T(" - %s( mode=%s )\n"), _T(__FUNCTION__),
               MatrixModeToString(mode));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->MatrixMode(mode);
  }

  return;
}


GL_API void GL_APIENTRY glMultMatrixx(const GLfixed *pM) {
  __profileAPI(_T(" - %s( pM=(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, ")
               _T("0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x) )\n"),
               _T(__FUNCTION__), pM[0], pM[1], pM[2], pM[3], pM[4], pM[5],
               pM[6], pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13],
               pM[14], pM[15]);

  if (NULL == pM)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->Multiply(*reinterpret_cast<const MATRIX44 *>(pM));
#else
    pContext->Multiply(MATRIX44(Math::TCast<floatf>(fixed16::Make(pM[0])),
                                Math::TCast<floatf>(fixed16::Make(pM[1])),
                                Math::TCast<floatf>(fixed16::Make(pM[2])),
                                Math::TCast<floatf>(fixed16::Make(pM[3])),
                                Math::TCast<floatf>(fixed16::Make(pM[4])),
                                Math::TCast<floatf>(fixed16::Make(pM[5])),
                                Math::TCast<floatf>(fixed16::Make(pM[6])),
                                Math::TCast<floatf>(fixed16::Make(pM[7])),
                                Math::TCast<floatf>(fixed16::Make(pM[8])),
                                Math::TCast<floatf>(fixed16::Make(pM[9])),
                                Math::TCast<floatf>(fixed16::Make(pM[10])),
                                Math::TCast<floatf>(fixed16::Make(pM[11])),
                                Math::TCast<floatf>(fixed16::Make(pM[12])),
                                Math::TCast<floatf>(fixed16::Make(pM[13])),
                                Math::TCast<floatf>(fixed16::Make(pM[14])),
                                Math::TCast<floatf>(fixed16::Make(pM[15]))));
#endif
  }
}


GL_API void GL_APIENTRY glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t,
                                          GLfixed r, GLfixed q) {
  __profileAPI(_T(" - %s( target=%s, s=%d, t=%d, r=%d, q=%d )\n"),
               _T(__FUNCTION__), TextureToString(target), s, t, r, q);
  
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->MultiTexCoord(target, Math::TCast<floatf>(fixed16::Make(s)),
                            Math::TCast<floatf>(fixed16::Make(t)),
                            Math::TCast<floatf>(fixed16::Make(r)),
                            Math::TCast<floatf>(fixed16::Make(q)));
  }
}


GL_API void GL_APIENTRY glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz) {
  __profileAPI(_T(" - %s( nx=%d, ny=%d, nz=%d )\n"), _T(__FUNCTION__), nx, ny,
               nz);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Normal(Math::TCast<floatf>(fixed16::Make(nx)),
                     Math::TCast<floatf>(fixed16::Make(ny)),
                     Math::TCast<floatf>(fixed16::Make(nz)));
  }
}


GL_API void GL_APIENTRY glNormalPointer(GLenum type, GLsizei stride,
                                        const GLvoid *pPointer) {
  __profileAPI(_T(" - %s( type=%s, stride=%d, pPointer=0x%p )\n"),
               _T(__FUNCTION__), TypeToString(type), stride, pPointer);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->NormalPointer(type, stride, pPointer);
  }
}


GL_API void GL_APIENTRY glOrthox(GLfixed left, GLfixed right, GLfixed bottom,
                                 GLfixed top, GLfixed zNear, GLfixed zFar) {
  __profileAPI(
      _T(" - %s( left=%d, right=%d, bottom=%d, top=%d, zNear=%d, zFar=%d )\n"),
      _T(__FUNCTION__), left, right, bottom, top, zNear, zFar);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Ortho(Math::TCast<floatf>(fixed16::Make(left)),
                    Math::TCast<floatf>(fixed16::Make(right)),
                    Math::TCast<floatf>(fixed16::Make(bottom)),
                    Math::TCast<floatf>(fixed16::Make(top)),
                    Math::TCast<floatf>(fixed16::Make(zNear)),
                    Math::TCast<floatf>(fixed16::Make(zFar)));
  }
}


GL_API void GL_APIENTRY glPixelStorei(GLenum pname, GLint param) {
  __profileAPI(_T(" - %s( pname=%s, param=%d )\n"), _T(__FUNCTION__),
               PixelStoreToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PixelStorei(pname, param);
  }
}


GL_API void GL_APIENTRY glPointParameterx(GLenum pname, GLfixed param) {
  __profileAPI(_T(" - %s( pname=%s, param=%d )\n"), _T(__FUNCTION__),
               PointParameterToString(pname), param);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TPointParameter<fixed16>(
        pname, reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glPointParameterxv(GLenum pname,
                                           const GLfixed *pParams) {
  __profileAPI(_T(" - %s( pname=%s, pParams=0x%p )\n"), _T(__FUNCTION__),
               PointParameterToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TPointParameter<fixed16>(
        pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glPointSizex(GLfixed size) {
  __profileAPI(_T(" - %s( size=%d )\n"), _T(__FUNCTION__), size);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PointSize(Math::TCast<floatf>(fixed16::Make(size)));
  }
}


GL_API void GL_APIENTRY glPolygonOffsetx(GLfixed factor, GLfixed units) {
  __profileAPI(_T(" - %s( factor=%d, units=%d )\n"), _T(__FUNCTION__), factor,
               units);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PolygonOffset(Math::TCast<floatf>(fixed16::Make(factor)),
                            Math::TCast<floatf>(fixed16::Make(units)));
  }
}


GL_API void GL_APIENTRY glPopMatrix() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PopMatrix();
  }
}


GL_API void GL_APIENTRY glPushMatrix() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PushMatrix();
  }
}


GL_API void GL_APIENTRY glReadPixels(GLint x, GLint y, GLsizei width,
                                     GLsizei height, GLenum format, GLenum type,
                                     GLvoid *pPixels) {
  __profileAPI(_T(" - %s( x=%d, y=%d, width=%d, height=%d, format=%s, ")
               _T("type=%s, pPixels=0x%p )\n"),
               _T(__FUNCTION__), x, y, width, height, FormatToString(format),
               TypeToString(type), pPixels);

  if (NULL == pPixels)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ReadPixels(x, y, width, height, format, type, pPixels);
  }
}


GL_API void GL_APIENTRY glRotatex(GLfixed angle, GLfixed x, GLfixed y,
                                  GLfixed z) {
  __profileAPI(_T(" - %s( angle=%d, x=%d, y=%d, z=%d )\n"), _T(__FUNCTION__),
               angle, x, y, z);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Rotate(Math::TCast<floatf>(fixed16::Make(angle)),
                     Math::TCast<floatf>(fixed16::Make(x)),
                     Math::TCast<floatf>(fixed16::Make(y)),
                     Math::TCast<floatf>(fixed16::Make(z)));
  }
}


GL_API void GL_APIENTRY glSampleCoverage(GLclampf value, GLboolean invert) {
  __profileAPI(_T(" - %s( value=%f, invert=%d )\n"), _T(__FUNCTION__), value,
               invert);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->SampleCoverage(floatf(value), invert);
  }
}


GL_API void GL_APIENTRY glSampleCoveragex(GLclampx value, GLboolean invert) {
  __profileAPI(_T(" - %s( value=%f, invert=%d )\n"), _T(__FUNCTION__), value,
               invert);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->SampleCoverage(Math::TCast<floatf>(fixed16::Make(value)), invert);
  }
}


GL_API void GL_APIENTRY glScalex(GLfixed x, GLfixed y, GLfixed z) {
  __profileAPI(_T(" - %s( x=%d, y=%d, z=%d )\n"), _T(__FUNCTION__), x, y, z);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Scale(Math::TCast<floatf>(fixed16::Make(x)),
                    Math::TCast<floatf>(fixed16::Make(y)),
                    Math::TCast<floatf>(fixed16::Make(z)));
  }
}


GL_API void GL_APIENTRY glScissor(GLint x, GLint y, GLsizei width,
                                  GLsizei height) {
  __profileAPI(_T(" - %s( x=%d, y=%d, width=%d, height=%d )\n"),
               _T(__FUNCTION__), x, y, width, height);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Scissor(x, y, width, height);
  }
}


GL_API void GL_APIENTRY glShadeModel(GLenum mode) {
  __profileAPI(_T(" - %s( mode=%s )\n"), _T(__FUNCTION__),
               ShadeModelToString(mode));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->ShadeModel(mode);
  }
}


GL_API void GL_APIENTRY glStencilFunc(GLenum func, GLint ref, GLuint mask) {
  __profileAPI(_T(" - %s( func=%s, ref=%d, mask=0x%x )\n"), _T(__FUNCTION__),
               FuncToString(func), ref, mask);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->StencilFunc(func, ref, mask);
  }
}


GL_API void GL_APIENTRY glStencilMask(GLuint mask) {
  __profileAPI(_T(" - %s( mask=0x%x )\n"), _T(__FUNCTION__), mask);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->StencilMask(mask);
  }
}


GL_API void GL_APIENTRY glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
  __profileAPI(_T(" - %s( fail=%s, zfail=%s, zpass=%s )\n"), _T(__FUNCTION__),
               StencilOpToString(fail), StencilOpToString(zfail),
               StencilOpToString(zpass));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->StencilOp(fail, zfail, zpass);
  }
}


GL_API void GL_APIENTRY glTexCoordPointer(GLint size, GLenum type,
                                          GLsizei stride,
                                          const GLvoid *pPointer) {
  __profileAPI(_T(" - %s( size=%d, type=%s, stride=%d, pPointer=0x%p )\n"),
               _T(__FUNCTION__), size, TypeToString(type), stride, pPointer);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TexCoordPointer(size, type, stride, pPointer);
  }
}


GL_API void GL_APIENTRY glTexEnvi(GLenum env, GLenum pname, GLint param) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, param=%s )\n"), _T(__FUNCTION__),
               TexEnvToString(env), TexEnvParamToString(pname),
               TexEnvValueToString(param));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexEnv<int>(env, pname, &param);
  }
}


GL_API void GL_APIENTRY glTexEnvx(GLenum env, GLenum pname, GLfixed param) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, param=%s )\n"), _T(__FUNCTION__),
               TexEnvToString(env), TexEnvParamToString(pname),
               TexEnvValueToString(param));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexEnv<fixed16>(env, pname,
                               reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glTexEnviv(GLenum env, GLenum pname,
                                   const GLint *pParams) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TexEnvToString(env),
               TexEnvParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexEnv<int>(env, pname, pParams);
  }
}


GL_API void GL_APIENTRY glTexEnvxv(GLenum env, GLenum pname,
                                   const GLfixed *pParams) {
  __profileAPI(_T(" - %s( env=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TexEnvToString(env),
               TexEnvParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexEnv<fixed16>(env, pname,
                               reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glTexImage2D(GLenum target, GLint level,
                                     GLint internalformat, GLsizei width,
                                     GLsizei height, GLint border,
                                     GLenum format, GLenum type,
                                     const GLvoid *pPixels) {
  __profileAPI(_T(" - %s( target=%s, level=%d, internalformat=%s, width=%d, ")
               _T("height=%d, border=%d, format=%s, type=%s, pPixels=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target), level,
               FormatToString(internalformat), width, height, border,
               FormatToString(format), TypeToString(type), pPixels);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TexImage2D(target, level, internalformat, width, height, border,
                         format, type, pPixels);
  }
}


GL_API void GL_APIENTRY glTexParameteri(GLenum target, GLenum pname,
                                        GLint param) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, param=%s )\n"), _T(__FUNCTION__),
               TextureTypeToString(target), TexParamToString(pname),
               TexParamValueToString(param));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexParameter<int>(target, pname, &param);
  }
}


GL_API void GL_APIENTRY glTexParameterx(GLenum target, GLenum pname,
                                        GLfixed param) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, param=%s )\n"), _T(__FUNCTION__),
               TextureTypeToString(target), TexParamToString(pname),
               TexParamValueToString(param));

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexParameter<fixed16>(target, pname,
                                     reinterpret_cast<const fixed16 *>(&param));
  }
}


GL_API void GL_APIENTRY glTexParameteriv(GLenum target, GLenum pname,
                                         const GLint *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target),
               TexParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexParameter<int>(target, pname, pParams);
  }
}


GL_API void GL_APIENTRY glTexParameterxv(GLenum target, GLenum pname,
                                         const GLfixed *pParams) {
  __profileAPI(_T(" - %s( target=%s, pname=%s, pParams=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target),
               TexParamToString(pname), pParams);

  if (NULL == pParams)
    return;
    
  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TTexParameter<fixed16>(
        target, pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}


GL_API void GL_APIENTRY glTexSubImage2D(GLenum target, GLint level,
                                        GLint xoffset, GLint yoffset,
                                        GLsizei width, GLsizei height,
                                        GLenum format, GLenum type,
                                        const GLvoid *pPixels) {
  __profileAPI(_T(" - %s( target=%s, level=%d, xoffset=%d, yoffset=%d, ")
               _T("width=%d, height=%d, format=%s, type=%s, pPixels=0x%p )\n"),
               _T(__FUNCTION__), TextureTypeToString(target), level, xoffset,
               yoffset, width, height, FormatToString(format),
               TypeToString(type), pPixels);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->TexSubImage2D(target, level, xoffset, yoffset, width, height,
                            format, type, pPixels);
  }
}


GL_API void GL_APIENTRY glTranslatex(GLfixed x, GLfixed y, GLfixed z) {
  __profileAPI(_T(" - %s( x=%d, y=%d, z=%d )\n"), _T(__FUNCTION__), x, y, z);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Translate(Math::TCast<floatf>(fixed16::Make(x)),
                        Math::TCast<floatf>(fixed16::Make(y)),
                        Math::TCast<floatf>(fixed16::Make(z)));
  }
}


GL_API void GL_APIENTRY glVertexPointer(GLint size, GLenum type, GLsizei stride,
                                        const GLvoid *pPointer) {
  __profileAPI(_T(" - %s( size=%d, type=%s, stride=%d, pPointer=0x%p )\n"),
               _T(__FUNCTION__), size, TypeToString(type), stride, pPointer);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->VertexPointer(size, type, stride, pPointer);
  }
}


GL_API void GL_APIENTRY glViewport(GLint x, GLint y, GLsizei width,
                                   GLsizei height) {
  __profileAPI(_T(" - %s( x=%d, y=%d, width=%d, height=%d )\n"),
               _T(__FUNCTION__), x, y, width, height);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->Viewport(x, y, width, height);
  }
}


GL_API void GL_APIENTRY glPointSizePointerOES(GLenum type, GLsizei stride,
                                              const GLvoid *pPointer) {
  __profileAPI(_T(" - %s( type=%s, stride=%d, pPointer=0x%p )\n"),
               _T(__FUNCTION__), TypeToString(type), stride, pPointer);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (pContext) {
    pContext->PointSizePointerOES(type, stride, pPointer);
  }
}


GL_API GLbitfield GL_APIENTRY glQueryMatrixxOES(GLfixed mantissa[16],
                                          GLint exponent[16]) {
  __profileAPI(_T(" - %s( mantissa=0x%p, exponent=0x%p )\n"), _T(__FUNCTION__),
               mantissa, exponent);

  auto driver = g_dll.driver();

  auto pContext = driver->GetCurrentContext();
  if (nullptr == pContext)
    return GL_INVALID_OPERATION;

  return pContext->TQueryMatrix<fixed16>(reinterpret_cast<fixed16 *>(mantissa), exponent);  
}
