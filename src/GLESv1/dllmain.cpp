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

Logger g_logger("CocoGL.log");
static GLDriver g_driver;

#ifndef NDEBUG

const char *FuncToString(GLenum func) {
  switch (func) {
  case GL_NEVER:
    return "GL_NEVER";
  case GL_LESS:
    return "GL_LESS";
  case GL_EQUAL:
    return "GL_EQUAL";
  case GL_LEQUAL:
    return "GL_LEQUAL";
  case GL_GREATER:
    return "GL_GREATER";
  case GL_NOTEQUAL:
    return "GL_NOTEQUAL";
  case GL_GEQUAL:
    return "GL_GEQUAL";
  case GL_ALWAYS:
    return "GL_ALWAYS";
  }

  return "UNKNOWN";
}

const char *PlaneToString(GLenum plane) {
  switch (plane) {
  case GL_CLIP_PLANE0:
    return "GL_CLIP_PLANE0";
  case GL_CLIP_PLANE1:
    return "GL_CLIP_PLANE1";
  case GL_CLIP_PLANE2:
    return "GL_CLIP_PLANE2";
  case GL_CLIP_PLANE3:
    return "GL_CLIP_PLANE3";
  case GL_CLIP_PLANE4:
    return "GL_CLIP_PLANE4";
  case GL_CLIP_PLANE5:
    return "GL_CLIP_PLANE5";
  }

  return "UNKNOWN";
}

const char *FogParamToString(GLenum pname) {
  switch (pname) {
  case GL_FOG_DENSITY:
    return "GL_FOG_DENSITY";
  case GL_FOG_START:
    return "GL_FOG_START";
  case GL_FOG_END:
    return "GL_FOG_END";
  case GL_FOG_MODE:
    return "GL_FOG_MODE";
  case GL_FOG_COLOR:
    return "GL_FOG_COLOR";
  case GL_CLIP_PLANE5:
    return "GL_CLIP_PLANE5";
  }

  return "UNKNOWN";
}

const char *LightToString(GLenum light) {
  switch (light) {
  case GL_LIGHT0:
    return "GL_LIGHT0";
  case GL_LIGHT1:
    return "GL_LIGHT1";
  case GL_LIGHT2:
    return "GL_LIGHT2";
  case GL_LIGHT3:
    return "GL_LIGHT3";
  case GL_LIGHT4:
    return "GL_LIGHT4";
  case GL_LIGHT5:
    return "GL_LIGHT5";
  case GL_LIGHT6:
    return "GL_LIGHT6";
  case GL_LIGHT7:
    return "GL_LIGHT7";
  }

  return "UNKNOWN";
}

const char *LightModelToString(GLenum pname) {
  switch (pname) {
  case GL_LIGHT_MODEL_AMBIENT:
    return "GL_LIGHT_MODEL_AMBIENT";
  case GL_LIGHT_MODEL_TWO_SIDE:
    return "GL_LIGHT_MODEL_TWO_SIDE";
  }

  return "UNKNOWN";
}

const char *LightParamToString(GLenum pname) {
  switch (pname) {
  case GL_AMBIENT:
    return "GL_AMBIENT";
  case GL_DIFFUSE:
    return "GL_DIFFUSE";
  case GL_SPECULAR:
    return "GL_SPECULAR";
  case GL_POSITION:
    return "GL_POSITION";
  case GL_SPOT_DIRECTION:
    return "GL_SPOT_DIRECTION";
  case GL_SPOT_EXPONENT:
    return "GL_SPOT_EXPONENT";
  case GL_SPOT_CUTOFF:
    return "GL_SPOT_CUTOFF";
  case GL_CONSTANT_ATTENUATION:
    return "GL_CONSTANT_ATTENUATION";
  case GL_LINEAR_ATTENUATION:
    return "GL_LINEAR_ATTENUATION";
  case GL_QUADRATIC_ATTENUATION:
    return "GL_QUADRATIC_ATTENUATION";
  }

  return "UNKNOWN";
}

const char *MaterialFaceToString(GLenum pname) {
  switch (pname) {
  case GL_FRONT_AND_BACK:
    return "GL_FRONT_AND_BACK";
  }

  return "UNKNOWN";
}

const char *MaterialParamToString(GLenum pname) {
  switch (pname) {
  case GL_EMISSION:
    return "GL_EMISSION";
  case GL_SHININESS:
    return "GL_SHININESS";
  case GL_AMBIENT_AND_DIFFUSE:
    return "GL_AMBIENT_AND_DIFFUSE";
  case GL_AMBIENT:
    return "GL_AMBIENT";
  case GL_DIFFUSE:
    return "GL_DIFFUSE";
  case GL_SPECULAR:
    return "GL_SPECULAR";
  }

  return "UNKNOWN";
}

const char *TextureToString(GLenum texture) {
  switch (texture) {
  case GL_TEXTURE0:
    return "GL_TEXTURE0";
  case GL_TEXTURE1:
    return "GL_TEXTURE1";
  }

  return "UNKNOWN";
}

const char *PNameToString(GLenum pname) {
  switch (pname) {
  case GL_POINT_SIZE_MIN:
    return "GL_POINT_SIZE_MIN";

  case GL_POINT_SIZE_MAX:
    return "GL_POINT_SIZE_MAX";

  case GL_POINT_FADE_THRESHOLD_SIZE:
    return "GL_POINT_FADE_THRESHOLD_SIZE";

  case GL_POINT_DISTANCE_ATTENUATION:
    return "GL_POINT_DISTANCE_ATTENUATION";
  }

  return "UNKNOWN";
}

const char *TexEnvToString(GLenum env) {
  switch (env) {
  case GL_TEXTURE_ENV:
    return "GL_TEXTURE_ENV";
  case GL_POINT_SPRITE_OES:
    return "GL_POINT_SPRITE_OES";
  }

  return "UNKNOWN";
}

const char *TexEnvParamToString(GLenum pname) {
  switch (pname) {
  case GL_TEXTURE_ENV_MODE:
    return "GL_TEXTURE_ENV_MODE";
  case GL_TEXTURE_ENV_COLOR:
    return "GL_TEXTURE_ENV_COLOR";
  case GL_COORD_REPLACE_OES:
    return "GL_COORD_REPLACE_OES";
  }

  return "UNKNOWN";
}

const char *TexParamToString(GLenum pname) {
  switch (pname) {
  case GL_TEXTURE_MIN_FILTER:
    return "GL_TEXTURE_MIN_FILTER";
  case GL_TEXTURE_MAG_FILTER:
    return "GL_TEXTURE_MAG_FILTER";
  case GL_TEXTURE_WRAP_S:
    return "GL_TEXTURE_WRAP_S";
  case GL_TEXTURE_WRAP_T:
    return "GL_TEXTURE_WRAP_T";
  case GL_GENERATE_MIPMAP:
    return "GL_GENERATE_MIPMAP";
  }

  return "UNKNOWN";
}

const char *BufferToString(GLenum pname) {
  switch (pname) {
  case GL_ARRAY_BUFFER:
    return "GL_ARRAY_BUFFER";
  case GL_ELEMENT_ARRAY_BUFFER:
    return "GL_ELEMENT_ARRAY_BUFFER";
  }

  return "UNKNOWN";
}

const char *BufferParamToString(GLenum pname) {
  switch (pname) {
  case GL_BUFFER_SIZE:
    return "GL_BUFFER_SIZE";
  case GL_BUFFER_USAGE:
    return "GL_BUFFER_USAGE";
  }

  return "UNKNOWN";
}

const char *BlendFuncToString(GLenum pname) {
  switch (pname) {
  case GL_ZERO:
    return "GL_ZERO";
  case GL_ONE:
    return "GL_ONE";
  case GL_SRC_ALPHA:
    return "GL_SRC_ALPHA";
  case GL_ONE_MINUS_SRC_ALPHA:
    return "GL_ONE_MINUS_SRC_ALPHA";
  case GL_DST_ALPHA:
    return "GL_DST_ALPHA";
  case GL_ONE_MINUS_DST_ALPHA:
    return "GL_ONE_MINUS_DST_ALPHA";
  case GL_DST_COLOR:
    return "GL_DST_COLOR";
  case GL_ONE_MINUS_DST_COLOR:
    return "GL_ONE_MINUS_DST_COLOR";
  case GL_SRC_ALPHA_SATURATE:
    return "GL_SRC_ALPHA_SATURATE";
  case GL_SRC_COLOR:
    return "GL_SRC_COLOR";
  case GL_ONE_MINUS_SRC_COLOR:
    return "GL_ONE_MINUS_SRC_COLOR";
  }

  return "UNKNOWN";
}

const char *UsageToString(GLenum usage) {
  switch (usage) {
  case GL_STATIC_DRAW:
    return "GL_STATIC_DRAW";
  case GL_DYNAMIC_DRAW:
    return "GL_DYNAMIC_DRAW";
  }

  return "UNKNOWN";
}

const char *TypeToString(GLenum type) {
  switch (type) {
  case GL_BYTE:
    return "GL_BYTE";
  case GL_UNSIGNED_BYTE:
    return "GL_UNSIGNED_BYTE";
  case GL_SHORT:
    return "GL_SHORT";
  case GL_UNSIGNED_SHORT:
    return "GL_UNSIGNED_SHORT";
  case GL_FLOAT:
    return "GL_FLOAT";
  case GL_FIXED:
    return "GL_FIXED";
  case GL_UNSIGNED_SHORT_4_4_4_4:
    return "GL_UNSIGNED_SHORT_4_4_4_4";
  case GL_UNSIGNED_SHORT_5_5_5_1:
    return "GL_UNSIGNED_SHORT_5_5_5_1";
  case GL_UNSIGNED_SHORT_5_6_5:
    return "GL_UNSIGNED_SHORT_5_6_5";
  }

  return "UNKNOWN";
}

const char *TextureTypeToString(GLenum texture) {
  switch (texture) {
  case GL_TEXTURE_2D:
    return "GL_TEXTURE_2D";
  }

  return "UNKNOWN";
}

const char *FormatToString(GLenum format) {
  switch (format) {
  case GL_ALPHA:
    return "GL_ALPHA";
  case GL_RGB:
    return "GL_RGB";
  case GL_RGBA:
    return "GL_RGBA";
  case GL_LUMINANCE:
    return "GL_LUMINANCE";
  case GL_LUMINANCE_ALPHA:
    return "GL_LUMINANCE_ALPHA";
  case GL_PALETTE4_RGB8_OES:
    return "GL_PALETTE4_RGB8_OES";
  case GL_PALETTE4_RGBA8_OES:
    return "GL_PALETTE4_RGBA8_OES";
  case GL_PALETTE4_R5_G6_B5_OES:
    return "GL_PALETTE4_R5_G6_B5_OES";
  case GL_PALETTE4_RGBA4_OES:
    return "GL_PALETTE4_RGBA4_OES";
  case GL_PALETTE4_RGB5_A1_OES:
    return "GL_PALETTE4_RGB5_A1_OES";
  case GL_PALETTE8_RGB8_OES:
    return "GL_PALETTE8_RGB8_OES";
  case GL_PALETTE8_RGBA8_OES:
    return "GL_PALETTE8_RGBA8_OES";
  case GL_PALETTE8_R5_G6_B5_OES:
    return "GL_PALETTE8_R5_G6_B5_OES";
  case GL_PALETTE8_RGBA4_OES:
    return "GL_PALETTE8_RGBA4_OES";
  case GL_PALETTE8_RGB5_A1_OES:
    return "GL_PALETTE8_RGB5_A1_OES";
  }

  return "UNKNOWN";
}

const char *CullFaceToString(GLenum mode) {
  switch (mode) {
  case GL_FRONT:
    return "GL_FRONT";
  case GL_BACK:
    return "GL_BACK";
  case GL_FRONT_AND_BACK:
    return "GL_FRONT_AND_BACK";
  }

  return "UNKNOWN";
}

const char *FogToString(GLenum pname) {
  switch (pname) {
  case GL_FOG_MODE:
    return "GL_FOG_MODE";
  case GL_FOG_START:
    return "GL_FOG_START";
  case GL_FOG_END:
    return "GL_FOG_END";
  case GL_FOG_DENSITY:
    return "GL_FOG_DENSITY";
  case GL_FOG_COLOR:
    return "GL_FOG_COLOR";
  }

  return "UNKNOWN";
}

const char *PrimitiveTypeToString(GLenum mode) {
  switch (mode) {
  case GL_POINTS:
    return "GL_POINTS";
  case GL_LINES:
    return "GL_LINES";
  case GL_LINE_STRIP:
    return "GL_LINE_STRIP";
  case GL_LINE_LOOP:
    return "GL_LINE_LOOP";
  case GL_TRIANGLES:
    return "GL_TRIANGLES";
  case GL_TRIANGLE_STRIP:
    return "GL_TRIANGLE_STRIP";
  case GL_TRIANGLE_FAN:
    return "GL_TRIANGLE_FAN";
  }

  return "UNKNOWN";
}

const char *FrontFaceToString(GLenum mode) {
  switch (mode) {
  case GL_CW:
    return "GL_CW";
  case GL_CCW:
    return "GL_CCW";
  }

  return "UNKNOWN";
}

const char *ClientStateToString(GLenum array) {
  switch (array) {
  case GL_VERTEX_ARRAY:
    return "GL_VERTEX_ARRAY";
  case GL_NORMAL_ARRAY:
    return "GL_NORMAL_ARRAY";
  case GL_COLOR_ARRAY:
    return "GL_COLOR_ARRAY";
  case GL_TEXTURE_COORD_ARRAY:
    return "GL_TEXTURE_COORD_ARRAY";
  case GL_POINT_SIZE_ARRAY_OES:
    return "GL_POINT_SIZE_ARRAY_OES";
  case GL_WEIGHT_ARRAY_OES:
    return "GL_WEIGHT_ARRAY_OES";
  case GL_MATRIX_INDEX_ARRAY_OES:
    return "GL_MATRIX_INDEX_ARRAY_OES";
  }

  return "UNKNOWN";
}

const char *HintTargetToString(GLenum target) {
  switch (target) {
  case GL_PERSPECTIVE_CORRECTION_HINT:
    return "GL_PERSPECTIVE_CORRECTION_HINT";

  case GL_POINT_SMOOTH_HINT:
    return "GL_POINT_SMOOTH_HINT";

  case GL_LINE_SMOOTH_HINT:
    return "GL_LINE_SMOOTH_HINT";

  case GL_FOG_HINT:
    return "GL_FOG_HINT";

  case GL_GENERATE_MIPMAP_HINT:
    return "GL_GENERATE_MIPMAP_HINT";
  }

  return "UNKNOWN";
}

const char *HintModeToString(GLenum mode) {
  switch (mode) {
  case GL_DONT_CARE:
    return "GL_DONT_CARE";
  case GL_FASTEST:
    return "GL_FASTEST";
  case GL_NICEST:
    return "GL_NICEST";
  }

  return "UNKNOWN";
}

const char *LogicOpToString(GLenum logicOp) {
  switch (logicOp) {
  case GL_CLEAR:
    return "GL_CLEAR";
  case GL_AND:
    return "GL_AND";
  case GL_AND_REVERSE:
    return "GL_AND_REVERSE";
  case GL_COPY:
    return "GL_COPY";
  case GL_AND_INVERTED:
    return "GL_AND_INVERTED";
  case GL_NOOP:
    return "GL_NOOP";
  case GL_XOR:
    return "GL_XOR";
  case GL_OR:
    return "GL_OR";
  case GL_NOR:
    return "GL_NOR";
  case GL_EQUIV:
    return "GL_EQUIV";
  case GL_INVERT:
    return "GL_INVERT";
  case GL_OR_REVERSE:
    return "GL_OR_REVERSE";
  case GL_COPY_INVERTED:
    return "GL_COPY_INVERTED";
  case GL_OR_INVERTED:
    return "GL_OR_INVERTED";
  case GL_NAND:
    return "GL_NAND";
  case GL_SET:
    return "GL_SET";
  }

  return "UNKNOWN";
}

const char *MatrixModeToString(GLenum mode) {
  switch (mode) {
  case GL_MODELVIEW:
    return "GL_MODELVIEW";
  case GL_PROJECTION:
    return "GL_PROJECTION";
  case GL_TEXTURE:
    return "GL_TEXTURE";
  case GL_MATRIX_PALETTE_OES:
    return "GL_MATRIX_PALETTE_OES";
  }

  return "UNKNOWN";
}

const char *PixelStoreToString(GLenum pname) {
  switch (pname) {
  case GL_PACK_ALIGNMENT:
    return "GL_PACK_ALIGNMENT";
  case GL_UNPACK_ALIGNMENT:
    return "GL_UNPACK_ALIGNMENT";
  }

  return "UNKNOWN";
}

const char *ShadeModelToString(GLenum mode) {
  switch (mode) {
  case GL_FLAT:
    return "GL_FLAT";
  case GL_SMOOTH:
    return "GL_SMOOTH";
  }

  return "UNKNOWN";
}

const char *StencilOpToString(GLenum mode) {
  switch (mode) {
  case GL_ZERO:
    return "GL_ZERO";
  case GL_INVERT:
    return "GL_INVERT";
  case GL_KEEP:
    return "GL_KEEP";
  case GL_REPLACE:
    return "GL_REPLACE";
  case GL_INCR:
    return "GL_INCR";
  case GL_DECR:
    return "GL_DECR";
  }

  return "UNKNOWN";
}

const char *PointParameterToString(GLenum mode) {
  switch (mode) {
  case GL_POINT_SIZE_MIN:
    return "GL_POINT_SIZE_MIN";

  case GL_POINT_SIZE_MAX:
    return "GL_POINT_SIZE_MAX";

  case GL_POINT_FADE_THRESHOLD_SIZE:
    return "GL_POINT_FADE_THRESHOLD_SIZE";

  case GL_POINT_DISTANCE_ATTENUATION:
    return "GL_POINT_DISTANCE_ATTENUATION";
  }

  return "UNKNOWN";
}

const char *TexParamValueToString(GLint param) {
  switch (param) {
  case GL_FALSE:
    return "GL_FALSE";
  case GL_TRUE:
    return "GL_TRUE";
  case GL_NEAREST:
    return "GL_NEAREST";
  case GL_LINEAR:
    return "GL_LINEAR";
  case GL_NEAREST_MIPMAP_NEAREST:
    return "GL_NEAREST_MIPMAP_NEAREST";
  case GL_LINEAR_MIPMAP_NEAREST:
    return "GL_LINEAR_MIPMAP_NEAREST";
  case GL_NEAREST_MIPMAP_LINEAR:
    return "GL_NEAREST_MIPMAP_LINEAR";
  case GL_LINEAR_MIPMAP_LINEAR:
    return "GL_LINEAR_MIPMAP_LINEAR";
  case GL_CLAMP_TO_EDGE:
    return "GL_CLAMP_TO_EDGE";
  case GL_REPEAT:
    return "GL_REPEAT";
  }

  return "UNKNOWN";
}

const char *TexEnvValueToString(GLint param) {
  switch (param) {
  case GL_ADD:
    return "GL_ADD";
  case GL_BLEND:
    return "GL_BLEND";
  case GL_REPLACE:
    return "GL_REPLACE";
  case GL_MODULATE:
    return "GL_MODULATE";
  case GL_DECAL:
    return "GL_DECAL";
  case GL_COORD_REPLACE_OES:
    return "GL_COORD_REPLACE_OES";
  }

  return "UNKNOWN";
}

const char *PointerToString(GLenum pname) {
  switch (pname) {
  case GL_VERTEX_ARRAY_POINTER:
    return "GL_VERTEX_ARRAY_POINTER";

  case GL_NORMAL_ARRAY_POINTER:
    return "GL_NORMAL_ARRAY_POINTER";

  case GL_COLOR_ARRAY_POINTER:
    return "GL_COLOR_ARRAY_POINTER";

  case GL_TEXTURE_COORD_ARRAY_POINTER:
    return "GL_TEXTURE_COORD_ARRAY_POINTER";

  case GL_POINT_SIZE_ARRAY_OES:
    return "GL_POINT_SIZE_ARRAY_OES";
  }

  return "UNKNOWN";
}

const char *StringToString(GLenum pname) {
  switch (pname) {
  case GL_VENDOR:
    return "GL_VENDOR";
  case GL_VERSION:
    return "GL_VERSION";
  case GL_RENDERER:
    return "GL_RENDERER";
  case GL_EXTENSIONS:
    return "GL_EXTENSIONS";
  }

  return "UNKNOWN";
}

const char *CapToString(GLenum cap) {
  switch (cap) {
  case GL_FOG:
    return "GL_FOG";

  case GL_LIGHTING:
    return "GL_LIGHTING";

  case GL_TEXTURE_2D:
    return "GL_TEXTURE_2D";

  case GL_CULL_FACE:
    return "GL_CULL_FACE";

  case GL_ALPHA_TEST:
    return "GL_ALPHA_TEST";

  case GL_BLEND:
    return "GL_BLEND";

  case GL_COLOR_LOGIC_OP:
    return "GL_COLOR_LOGIC_OP";

  case GL_DITHER:
    return "GL_DITHER";

  case GL_STENCIL_TEST:
    return "GL_STENCIL_TEST";

  case GL_DEPTH_TEST:
    return "GL_DEPTH_TEST";

  case GL_POINT_SMOOTH:
    return "GL_POINT_SMOOTH";

  case GL_POINT_SPRITE_OES:
    return "GL_POINT_SPRITE_OES";

  case GL_LINE_SMOOTH:
    return "GL_LINE_SMOOTH";

  case GL_SCISSOR_TEST:
    return "GL_SCISSOR_TEST";

  case GL_COLOR_MATERIAL:
    return "GL_COLOR_MATERIAL";

  case GL_NORMALIZE:
    return "GL_NORMALIZE";

  case GL_CLIP_PLANE0:
    return "GL_CLIP_PLANE0";

  case GL_CLIP_PLANE1:
    return "GL_CLIP_PLANE1";

  case GL_CLIP_PLANE2:
    return "GL_CLIP_PLANE2";

  case GL_CLIP_PLANE3:
    return "GL_CLIP_PLANE3";

  case GL_CLIP_PLANE4:
    return "GL_CLIP_PLANE4";

  case GL_CLIP_PLANE5:
    return "GL_CLIP_PLANE5";

  case GL_LIGHT0:
    return "GL_LIGHT0";

  case GL_LIGHT1:
    return "GL_LIGHT1";

  case GL_LIGHT2:
    return "GL_LIGHT2";

  case GL_LIGHT3:
    return "GL_LIGHT3";

  case GL_LIGHT4:
    return "GL_LIGHT4";

  case GL_LIGHT5:
    return "GL_LIGHT5";

  case GL_LIGHT6:
    return "GL_LIGHT6";

  case GL_LIGHT7:
    return "GL_LIGHT7";

  case GL_RESCALE_NORMAL:
    return "GL_RESCALE_NORMAL";

  case GL_POLYGON_OFFSET_FILL:
    return "GL_POLYGON_OFFSET_FILL";

  case GL_MULTISAMPLE:
    return "GL_MULTISAMPLE";

  case GL_SAMPLE_ALPHA_TO_COVERAGE:
    return "GL_SAMPLE_ALPHA_TO_COVERAGE";

  case GL_SAMPLE_ALPHA_TO_ONE:
    return "GL_SAMPLE_ALPHA_TO_ONE";

  case GL_SAMPLE_COVERAGE:
    return "GL_SAMPLE_COVERAGE";

  case GL_MATRIX_PALETTE_OES:
    return "GL_MATRIX_PALETTE_OES";
  }

  return "UNKNOWN";
}

const char *AttributeToString(GLenum pname) {
  switch (pname) {
  case GL_LIGHT_MODEL_TWO_SIDE:
    return "GL_LIGHT_MODEL_TWO_SIDE";

  case GL_COLOR_WRITEMASK:
    return "GL_COLOR_WRITEMASK";

  case GL_DEPTH_WRITEMASK:
    return "GL_DEPTH_WRITEMASK";

  case GL_SAMPLE_COVERAGE_INVERT:
    return "GL_SAMPLE_COVERAGE_INVERT";

  case GL_COORD_REPLACE_OES:
    return "GL_COORD_REPLACE_OES";

  case GL_ALIASED_POINT_SIZE_RANGE:
    return "GL_ALIASED_POINT_SIZE_RANGE";

  case GL_ALIASED_LINE_WIDTH_RANGE:
    return "GL_ALIASED_LINE_WIDTH_RANGE";

  case GL_SMOOTH_LINE_WIDTH_RANGE:
    return "GL_SMOOTH_LINE_WIDTH_RANGE";

  case GL_SMOOTH_POINT_SIZE_RANGE:
    return "GL_SMOOTH_POINT_SIZE_RANGE";

  case GL_ALPHA_BITS:
    return "GL_ALPHA_BITS";

  case GL_BLUE_BITS:
    return "GL_BLUE_BITS";

  case GL_DEPTH_BITS:
    return "GL_DEPTH_BITS";

  case GL_GREEN_BITS:
    return "GL_GREEN_BITS";

  case GL_RED_BITS:
    return "GL_RED_BITS";

  case GL_STENCIL_BITS:
    return "GL_STENCIL_BITS";

  case GL_SUBPIXEL_BITS:
    return "GL_SUBPIXEL_BITS";

  case GL_COMPRESSED_TEXTURE_FORMATS:
    return "GL_COMPRESSED_TEXTURE_FORMATS";

  case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
    return "GL_NUM_COMPRESSED_TEXTURE_FORMATS";

  case GL_MAX_LIGHTS:
    return "GL_MAX_LIGHTS";

  case GL_MAX_CLIP_PLANES:
    return "GL_MAX_CLIP_PLANES";

  case GL_MAX_MODELVIEW_STACK_DEPTH:
    return "GL_MAX_MODELVIEW_STACK_DEPTH";

  case GL_MAX_PROJECTION_STACK_DEPTH:
    return "GL_MAX_PROJECTION_STACK_DEPTH";

  case GL_MAX_TEXTURE_STACK_DEPTH:
    return "GL_MAX_TEXTURE_STACK_DEPTH";

  case GL_MAX_TEXTURE_SIZE:
    return "GL_MAX_TEXTURE_SIZE";

  case GL_MAX_TEXTURE_UNITS:
    return "GL_MAX_TEXTURE_UNITS";

  case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
    return "GL_IMPLEMENTATION_COLOR_READ_TYPE_OES";

  case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
    return "GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES";

  case GL_MAX_VIEWPORT_DIMS:
    return "GL_MAX_VIEWPORT_DIMS";

  case GL_VERTEX_ARRAY_SIZE:
    return "GL_VERTEX_ARRAY_SIZE";

  case GL_VERTEX_ARRAY_STRIDE:
    return "GL_VERTEX_ARRAY_STRIDE";

  case GL_VERTEX_ARRAY_TYPE:
    return "GL_VERTEX_ARRAY_TYPE";

  case GL_NORMAL_ARRAY_STRIDE:
    return "GL_NORMAL_ARRAY_STRIDE";

  case GL_NORMAL_ARRAY_TYPE:
    return "GL_NORMAL_ARRAY_TYPE";

  case GL_COLOR_ARRAY_SIZE:
    return "GL_COLOR_ARRAY_SIZE";

  case GL_COLOR_ARRAY_STRIDE:
    return "GL_COLOR_ARRAY_STRIDE";

  case GL_COLOR_ARRAY_TYPE:
    return "GL_COLOR_ARRAY_TYPE";

  case GL_TEXTURE_COORD_ARRAY_SIZE:
    return "GL_TEXTURE_COORD_ARRAY_SIZE";

  case GL_TEXTURE_COORD_ARRAY_STRIDE:
    return "GL_TEXTURE_COORD_ARRAY_STRIDE";

  case GL_TEXTURE_COORD_ARRAY_TYPE:
    return "GL_TEXTURE_COORD_ARRAY_TYPE";

  case GL_POINT_SIZE_ARRAY_TYPE_OES:
    return "GL_POINT_SIZE_ARRAY_TYPE_OES";

  case GL_POINT_SIZE_ARRAY_STRIDE_OES:
    return "GL_POINT_SIZE_ARRAY_STRIDE_OES";

  case GL_VERTEX_ARRAY_BUFFER_BINDING:
    return "GL_VERTEX_ARRAY_BUFFER_BINDING";

  case GL_NORMAL_ARRAY_BUFFER_BINDING:
    return "GL_NORMAL_ARRAY_BUFFER_BINDING";

  case GL_COLOR_ARRAY_BUFFER_BINDING:
    return "GL_COLOR_ARRAY_BUFFER_BINDING";

  case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
    return "GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING";

  case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
    return "GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES";

  case GL_ARRAY_BUFFER_BINDING:
    return "GL_ARRAY_BUFFER_BINDING";

  case GL_ELEMENT_ARRAY_BUFFER_BINDING:
    return "GL_ELEMENT_ARRAY_BUFFER_BINDING";

  case GL_TEXTURE_BINDING_2D:
    return "GL_TEXTURE_BINDING_2D";

  case GL_UNPACK_ALIGNMENT:
    return "GL_UNPACK_ALIGNMENT";

  case GL_PACK_ALIGNMENT:
    return "GL_PACK_ALIGNMENT";

  case GL_STENCIL_CLEAR_VALUE:
    return "GL_STENCIL_CLEAR_VALUE";

  case GL_SCISSOR_BOX:
    return "GL_SCISSOR_BOX";

  case GL_VIEWPORT:
    return "GL_VIEWPORT";

  case GL_STENCIL_WRITEMASK:
    return "GL_STENCIL_WRITEMASK";

  case GL_STENCIL_VALUE_MASK:
    return "GL_STENCIL_VALUE_MASK";

  case GL_STENCIL_REF:
    return "GL_STENCIL_REF";

  case GL_ALPHA_TEST_FUNC:
    return "GL_ALPHA_TEST_FUNC";

  case GL_STENCIL_FUNC:
    return "GL_STENCIL_FUNC";

  case GL_STENCIL_FAIL:
    return "GL_STENCIL_FAIL";

  case GL_STENCIL_PASS_DEPTH_FAIL:
    return "GL_STENCIL_PASS_DEPTH_FAIL";

  case GL_STENCIL_PASS_DEPTH_PASS:
    return "GL_STENCIL_PASS_DEPTH_PASS";

  case GL_DEPTH_FUNC:
    return "GL_DEPTH_FUNC";

  case GL_LOGIC_OP_MODE:
    return "GL_LOGIC_OP_MODE";

  case GL_BLEND_SRC:
    return "GL_BLEND_SRC";

  case GL_BLEND_DST:
    return "GL_BLEND_DST";

  case GL_FOG_MODE:
    return "GL_FOG_MODE";

  case GL_SHADE_MODEL:
    return "GL_SHADE_MODEL";

  case GL_CULL_FACE_MODE:
    return "GL_CULL_FACE_MODE";

  case GL_FRONT_FACE:
    return "GL_FRONT_FACE";

  case GL_MATRIX_MODE:
    return "GL_MATRIX_MODE";

  case GL_ACTIVE_TEXTURE:
    return "GL_ACTIVE_TEXTURE";

  case GL_CLIENT_ACTIVE_TEXTURE:
    return "GL_CLIENT_ACTIVE_TEXTURE";

  case GL_PERSPECTIVE_CORRECTION_HINT:
    return "GL_PERSPECTIVE_CORRECTION_HINT";

  case GL_POINT_SMOOTH_HINT:
    return "GL_POINT_SMOOTH_HINT";

  case GL_LINE_SMOOTH_HINT:
    return "GL_LINE_SMOOTH_HINT";

  case GL_FOG_HINT:
    return "GL_FOG_HINT";

  case GL_GENERATE_MIPMAP_HINT:
    return "GL_GENERATE_MIPMAP_HINT";

  case GL_MAX_VERTEX_UNITS_OES:
    return "GL_MAX_VERTEX_UNITS_OES";

  case GL_MAX_PALETTE_MATRICES_OES:
    return "GL_MAX_PALETTE_MATRICES_OES";

  case GL_CURRENT_COLOR:
    return "GL_CURRENT_COLOR";

  case GL_DEPTH_CLEAR_VALUE:
    return "GL_DEPTH_CLEAR_VALUE";

  case GL_CURRENT_TEXTURE_COORDS:
    return "GL_CURRENT_TEXTURE_COORDS";

  case GL_CURRENT_NORMAL:
    return "GL_CURRENT_NORMAL";

  case GL_MODELVIEW_MATRIX:
    return "GL_MODELVIEW_MATRIX";

  case GL_PROJECTION_MATRIX:
    return "GL_PROJECTION_MATRIX";

  case GL_TEXTURE_MATRIX:
    return "GL_TEXTURE_MATRIX";

  case GL_FOG_COLOR:
    return "GL_FOG_COLOR";

  case GL_FOG_DENSITY:
    return "GL_FOG_DENSITY";

  case GL_FOG_START:
    return "GL_FOG_START";

  case GL_FOG_END:
    return "GL_FOG_END";

  case GL_ALPHA_TEST_REF:
    return "GL_ALPHA_TEST_REF";

  case GL_LIGHT_MODEL_AMBIENT:
    return "GL_LIGHT_MODEL_AMBIENT";

  case GL_COLOR_CLEAR_VALUE:
    return "GL_COLOR_CLEAR_VALUE";

  case GL_POLYGON_OFFSET_UNITS:
    return "GL_POLYGON_OFFSET_UNITS";

  case GL_POLYGON_OFFSET_FACTOR:
    return "GL_POLYGON_OFFSET_FACTOR";

  case GL_SAMPLE_COVERAGE_VALUE:
    return "GL_SAMPLE_COVERAGE_VALUE";

  case GL_POINT_SIZE_MIN:
    return "GL_POINT_SIZE_MIN";

  case GL_POINT_SIZE_MAX:
    return "GL_POINT_SIZE_MAX";

  case GL_POINT_FADE_THRESHOLD_SIZE:
    return "GL_POINT_FADE_THRESHOLD_SIZE";

  case GL_POINT_DISTANCE_ATTENUATION:
    return "GL_POINT_DISTANCE_ATTENUATION";

  case GL_DEPTH_RANGE:
    return "GL_DEPTH_RANGE";

  case GL_POINT_SIZE:
    return "GL_POINT_SIZE";

  case GL_LINE_WIDTH:
    return "GL_LINE_WIDTH";
  }

  return "UNKNOWN";
}

#endif

//////////////////////////////////////////////////////////////////////////////

GL_API GLenum GL_APIENTRY __glCreateSurface(
    const GLSurfaceDesc *pColorDesc, const GLSurfaceDesc *pDepthStencilDesc,
    __GLSurface *pGLSurface) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  if (nullptr == pGLSurface) {
    __glLogError("__glCreateSurface() failed, pGLSurface is nullptr.\r\n");
    return GL_INVALID_VALUE;
  }

  GLSurface *pSurface;
  err = GLSurface::Create(&pSurface, pColorDesc, pDepthStencilDesc);
  if (__glFailed(err)) {
    __glLogError("GLSurface::Create() failed, err = %d.\r\n", err);
    return err;
  }

  uint32_t handle;
  err = g_driver.registerObject(&handle, pSurface, HANDLE_SURFACE);
  if (__glFailed(err)) {
    __glLogError("GLDriver::registerObject() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return err;
  }

  *pGLSurface = reinterpret_cast<__GLSurface>(handle);

  return GL_NO_ERROR;
}

GL_API GLenum GL_APIENTRY
__glUpdateSurface(__GLSurface surface, const GLSurfaceDesc *pColorDesc,
                  const GLSurfaceDesc *pDepthStencilDesc) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  auto pSurface = g_driver.getObject<GLSurface *>(surface);
  if (nullptr == pSurface) {
    __glLogError("__glUpdateSurface() failed, invalid surface handle.\r\n");
    return GL_INVALID_VALUE;
  }

  err = pSurface->update(pColorDesc, pDepthStencilDesc);
  if (__glFailed(err)) {
    __glLogError("GLSurface::initialize() failed, err = %d.\r\n", err);
    return err;
  }

  return GL_NO_ERROR;
}

GL_API GLenum GL_APIENTRY __glDestroySurface(__GLSurface surface) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  // Remove the surface object from the handle table
  auto pSurface = g_driver.unregisterObject<GLSurface *>(surface);
  if (nullptr == pSurface) {
    __glLogError("Invalid surface handle.\r\n");
    return GL_INVALID_VALUE;
  }

  // Delete the surface
  pSurface->release();

  return GL_NO_ERROR;
}

GL_API GLenum GL_APIENTRY __glCreateContext(__GLContext shared_context,
                                            __GLContext *pGLContext) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  if (nullptr == pGLContext) {
    __glLogError("__glCreateContext() failed, pGLContext is nullptr.\r\n");
    return GL_INVALID_VALUE;
  }

  GLContext *pCtxShared = nullptr;
  if (shared_context) {
    pCtxShared = g_driver.getObject<GLContext *>(shared_context);
    if (nullptr == pCtxShared) {
      __glLogError("Invalid context handle.\r\n");
      return GL_INVALID_VALUE;
    }
  }

  GLContext *pContext;
  err = GLContext::Create(&pContext, g_driver.getRasterCache(), pCtxShared);
  if (__glFailed(err)) {
    __glLogError("GLContext::Create() failed, err = %d.\r\n", err);
    return err;
  }

  uint32_t handle;
  err = g_driver.registerObject(&handle, pContext, HANDLE_CONTEXT);
  if (__glFailed(err)) {
    __glLogError("GLDriver::registerObject() failed, err = %d.\r\n", err);
    __safeRelease(pContext);
    return err;
  }

  *pGLContext = reinterpret_cast<__GLContext>(handle);

  return GL_NO_ERROR;
}

GL_API GLenum GL_APIENTRY __glDestroyContext(__GLContext context) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  // Remove the context object from the handle table
  auto pContext = g_driver.unregisterObject<GLContext *>(context);
  if (nullptr == pContext) {
    __glLogError("Invalid context handle.\r\n");
    return GL_INVALID_VALUE;
  }

  // Delete the context
  pContext->release();

  return GL_NO_ERROR;
}

GL_API GLenum GL_APIENTRY __glMakeCurrent(__GLContext context, __GLSurface draw,
                                          __GLSurface read) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLContext *pContext = nullptr;
  if (context) {
    pContext = g_driver.getObject<GLContext *>(context);
    if (nullptr == pContext) {
      __glLogError("Invalid context handle.\r\n");
      return GL_INVALID_VALUE;
    }
  }

  GLSurface *pSurfDraw = nullptr;
  if (draw) {
    pSurfDraw = g_driver.getObject<GLSurface *>(draw);
    if (nullptr == pSurfDraw) {
      __glLogError("Invalid surface handle.\r\n");
      return GL_INVALID_VALUE;
    }
  }

  GLSurface *pSurfRead = nullptr;
  if (read) {
    pSurfRead = g_driver.getObject<GLSurface *>(read);
    if (nullptr == pSurfRead) {
      __glLogError("Invalid surface handle.\r\n");
      return GL_INVALID_VALUE;
    }
  }

  g_driver.makeCurrent(pContext, pSurfDraw, pSurfRead);

  return GL_NO_ERROR;
}

GL_API GLenum GL_APIENTRY __glCopyBuffers(const GLSurfaceDesc *pDstDesc,
                                          const GLSurfaceDesc *pSrcDesc) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  return CopyBuffers(*pDstDesc, 0, 0, pSrcDesc->Width, pSrcDesc->Height,
                     *pSrcDesc, 0, 0);
}

GL_API GLenum GL_APIENTRY __glBindTexImage(__GLSurface surface,
                                           bool bGenMipMaps) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (surface) {
    auto pSurface = g_driver.getObject<GLSurface *>(surface);
    if (pSurface) {
      auto pContext = g_driver.getCurrentContext();
      if (nullptr == pContext) {
        __glLogError("__glBindTexImage() failed, no active context.\r\n");
        return GL_INVALID_OPERATION;
      }

      return pContext->bindTexImage(pSurface, bGenMipMaps);
    }
  }

  __glLogError("__glBindTexImage() failed, invalid surface handle.\r\n");
  return GL_INVALID_VALUE;
}

GL_API GLenum GL_APIENTRY __glReleaseTexImage(__GLSurface surface) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (surface) {
    auto pSurface = g_driver.getObject<GLSurface *>(surface);
    if (pSurface) {
      auto pContext = g_driver.getCurrentContext();
      if (nullptr == pContext) {
        __glLogError("__glBindTexImage() failed, no active context.\r\n");
        return GL_INVALID_OPERATION;
      }

      return pContext->releaseTexImage(pSurface);
    }
  }

  __glLogError("__glBindTexImage() failed, invalid surface handle.\r\n");
  return GL_INVALID_VALUE;
}

GLenum __glSaveBitmap(__GLSurface surface, const char *filename) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  if (surface) {
    auto pSurface = g_driver.getObject<GLSurface *>(surface);
    if (pSurface) {
      return pSurface->saveBitmap(filename);
    }
  }

  __glLogError("Invalid surface handle.\r\n");

  return GL_INVALID_VALUE;
}

//////////////////////////////////////////////////////////////////////////////

GL_API void GL_APIENTRY glAlphaFunc(GLenum func, GLclampf ref) {
  __profileAPI(" - %s( func=%s, ref=%f )\n", __FUNCTION__, FuncToString(func),
               ref);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setAlphaFunc(func, floatf(ref));
  }
}

GL_API void GL_APIENTRY glClearColor(GLclampf red, GLclampf green,
                                     GLclampf blue, GLclampf alpha) {
  __profileAPI(" - %s( red=%f, green=%f, blue=%f, alpha%f )\n", __FUNCTION__,
               red, green, blue, alpha);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->clearColor(floatf(red), floatf(green), floatf(blue),
                         floatf(alpha));
  }
}

GL_API void GL_APIENTRY glClearDepthf(GLclampf depth) {
  __profileAPI(" - %s( depth=%f )\n", __FUNCTION__, depth);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->clearDepth(floatf(depth));
  }
}

GL_API void GL_APIENTRY glClipPlanef(GLenum plane, const GLfloat *pEquation) {
  __profileAPI(" - %s( plane=%s, pEquation=0x%p )\n", __FUNCTION__,
               PlaneToString(plane), pEquation);

  if (nullptr == pEquation)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->setClipPlane(plane,
                           VECTOR4(floatf(pEquation[0]), floatf(pEquation[1]),
                                   floatf(pEquation[2]), floatf(pEquation[3])));
#else
    pContext->setClipPlane(plane,
                           *reinterpret_cast<const VECTOR4 *>(pEquation));
#endif
  }
}

GL_API void GL_APIENTRY glColor4f(GLfloat red, GLfloat green, GLfloat blue,
                                  GLfloat alpha) {
  __profileAPI(" - %s( red=%f, green=%f, blue=%f, alpha=%f )\n", __FUNCTION__,
               red, green, blue, alpha);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setColor(floatf(red), floatf(green), floatf(blue), floatf(alpha));
  }
}

GL_API void GL_APIENTRY glDepthRangef(GLclampf zNear, GLclampf zFar) {
  __profileAPI(" - %s( zNear=%f, zFar=%f )\n", __FUNCTION__, zNear, zFar);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setDepthRange(floatf(zNear), floatf(zFar));
  }
}

GL_API void GL_APIENTRY glFogf(GLenum pname, GLfloat param) {
  __profileAPI(" - %s( pname=%s, param=%f )\n", __FUNCTION__,
               FogParamToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setFog<float>(pname, &param);
  }
}

GL_API void GL_APIENTRY glFogfv(GLenum pname, const GLfloat *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               FogParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setFog<float>(pname, pParams);
  }
}

GL_API void GL_APIENTRY glFrustumf(GLfloat left, GLfloat right, GLfloat bottom,
                                   GLfloat top, GLfloat zNear, GLfloat zFar) {
  __profileAPI(
      " - %s( left=%f, right=%f, bottom=%f, top=%f, zNear=%f, zFar=%f )\n",
      __FUNCTION__, left, right, bottom, top, zNear, zFar);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->frustum(floatf(left), floatf(right), floatf(bottom), floatf(top),
                      floatf(zNear), floatf(zFar));
  }
}

GL_API void GL_APIENTRY glGetClipPlanef(GLenum plane, GLfloat eqn[4]) {
  __profileAPI(" - %s( plane=%s, eqn[4]=0x%p)\n", __FUNCTION__,
               PlaneToString(plane), eqn);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getClipPlane<float>(plane, eqn);
  }
}

GL_API void GL_APIENTRY glGetFloatv(GLenum pname, GLfloat *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               AttributeToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->get<float>(pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetLightfv(GLenum light, GLenum pname,
                                     GLfloat *pParams) {
  __profileAPI(" - %s( light=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               LightToString(light), LightParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getLight<float>(light, pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetMaterialfv(GLenum face, GLenum pname,
                                        GLfloat *pParams) {
  __profileAPI(" - %s( face=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               MaterialFaceToString(face), MaterialParamToString(pname),
               pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getMaterial<float>(face, pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetTexEnvfv(GLenum env, GLenum pname,
                                      GLfloat *pParams) {
  __profileAPI(" - %s( env=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getTexEnv<float>(env, pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetTexParameterfv(GLenum target, GLenum pname,
                                            GLfloat *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getTexParameter<float>(target, pname, pParams);
  }
}

GL_API void GL_APIENTRY glLightModelf(GLenum pname, GLfloat param) {
  __profileAPI(" - %s( pname=%s, param=%f )\n", __FUNCTION__,
               LightModelToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameterModel<float>(pname, &param);
  }
}

GL_API void GL_APIENTRY glLightModelfv(GLenum pname, const GLfloat *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=(%f, %f, %f, %f) )\n", __FUNCTION__,
               LightModelToString(pname), pParams[0], pParams[1], pParams[2],
               pParams[3]);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameterModel<float>(pname, pParams);
  }
}

GL_API void GL_APIENTRY glLightf(GLenum light, GLenum pname, GLfloat param) {
  __profileAPI(" - %s( light=%s, pname=%s, param=%f )\n", __FUNCTION__,
               LightToString(light), LightParamToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameter<float>(light, pname, &param);
  }
}

GL_API void GL_APIENTRY glLightfv(GLenum light, GLenum pname,
                                  const GLfloat *pParams) {
  __profileAPI(" - %s( light=%s, pname=%s, pParams=(%f, %f, %f, %f) )\n",
               __FUNCTION__, LightToString(light), LightParamToString(pname),
               pParams[0], pParams[1], pParams[2], pParams[3]);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameter<float>(light, pname, pParams);
  }
}

GL_API void GL_APIENTRY glLineWidth(GLfloat width) {
  __profileAPI(" - %s( width=%f )\n", __FUNCTION__, width);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLineWidth(floatf(width));
  }
}

GL_API void GL_APIENTRY glLoadMatrixf(const GLfloat *pM) {
  __profileAPI(" - %s( pM=(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, "
               "%f, %f, %f, %f) )\n",
               __FUNCTION__, pM[0], pM[1], pM[2], pM[3], pM[4], pM[5], pM[6],
               pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13], pM[14],
               pM[15]);

  if (nullptr == pM)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->loadMatrix(MATRIX44(
        floatf(pM[0]), floatf(pM[1]), floatf(pM[2]), floatf(pM[3]),
        floatf(pM[4]), floatf(pM[5]), floatf(pM[6]), floatf(pM[7]),
        floatf(pM[8]), floatf(pM[9]), floatf(pM[10]), floatf(pM[11]),
        floatf(pM[12]), floatf(pM[13]), floatf(pM[14]), floatf(pM[15])));
#else
    pContext->loadMatrix(*reinterpret_cast<const MATRIX44 *>(pM));
#endif
  }
}

GL_API void GL_APIENTRY glMaterialf(GLenum face, GLenum pname, GLfloat param) {
  __profileAPI(" - %s( face=%s, pname=%s, param=%f )\n", __FUNCTION__,
               MaterialFaceToString(face), MaterialParamToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMaterial<float>(face, pname, &param);
  }
}

GL_API void GL_APIENTRY glMaterialfv(GLenum face, GLenum pname,
                                     const GLfloat *pParams) {
  __profileAPI(" - %s( face=%s, pname=%s, pParams=(%f, %f, %f, %f) )\n",
               __FUNCTION__, MaterialFaceToString(face),
               MaterialParamToString(pname), pParams[0], pParams[1], pParams[2],
               pParams[3]);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMaterial<float>(face, pname, pParams);
  }
}

GL_API void GL_APIENTRY glMultMatrixf(const GLfloat *pM) {
  __profileAPI(" - %s( pM=(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, "
               "%f, %f, %f, %f) )\n",
               __FUNCTION__, pM[0], pM[1], pM[2], pM[3], pM[4], pM[5], pM[6],
               pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13], pM[14],
               pM[15]);

  if (nullptr == pM)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->multiply(MATRIX44(
        floatf(pM[0]), floatf(pM[1]), floatf(pM[2]), floatf(pM[3]),
        floatf(pM[4]), floatf(pM[5]), floatf(pM[6]), floatf(pM[7]),
        floatf(pM[8]), floatf(pM[9]), floatf(pM[10]), floatf(pM[11]),
        floatf(pM[12]), floatf(pM[13]), floatf(pM[14]), floatf(pM[15])));
#else
    pContext->multiply(*reinterpret_cast<const MATRIX44 *>(pM));
#endif
  }
}

GL_API void GL_APIENTRY glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t,
                                          GLfloat r, GLfloat q) {
  __profileAPI(" - %s( target=%s, s=%f, t=%f, r=%f, q=%f )\n", __FUNCTION__,
               TextureToString(target), s, t, r, q);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMultiTexCoord(target, floatf(s), floatf(t), floatf(r),
                               floatf(q));
  }
}

GL_API void GL_APIENTRY glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz) {
  __profileAPI(" - %s( nx=%f, ny=%f, nz=%f )\n", __FUNCTION__, nx, ny, nz);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setNormal(floatf(nx), floatf(ny), floatf(nz));
  }
}

GL_API void GL_APIENTRY glOrthof(GLfloat left, GLfloat right, GLfloat bottom,
                                 GLfloat top, GLfloat zNear, GLfloat zFar) {
  __profileAPI(
      " - %s( left=%f, right=%f, bottom=%f, top=%f, zNear=%f, zFar=%f )\n",
      __FUNCTION__, left, right, bottom, top, zNear, zFar);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->ortho(floatf(left), floatf(right), floatf(bottom), floatf(top),
                    floatf(zNear), floatf(zFar));
  }
}

GL_API void GL_APIENTRY glPointParameterf(GLenum pname, GLfloat param) {
  __profileAPI(" - %s( pname=%s, param=%f )\n", __FUNCTION__,
               PNameToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointParameter<float>(pname, &param);
  }
}

GL_API void GL_APIENTRY glPointParameterfv(GLenum pname,
                                           const GLfloat *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               PNameToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointParameter<float>(pname, pParams);
  }
}

GL_API void GL_APIENTRY glPointSize(GLfloat size) {
  __profileAPI(" - %s( size=%f )\n", __FUNCTION__, size);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointSize(floatf(size));
  }
}

GL_API void GL_APIENTRY glPolygonOffset(GLfloat factor, GLfloat units) {
  __profileAPI(" - %s( factor=%f, units=%f )\n", __FUNCTION__, factor, units);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPolygonOffset(floatf(factor), floatf(units));
  }
}

GL_API void GL_APIENTRY glRotatef(GLfloat angle, GLfloat x, GLfloat y,
                                  GLfloat z) {
  __profileAPI(" - %s( angle=%f, x=%f, y=%f, z=%f )\n", __FUNCTION__, angle, x,
               y, z);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->rotate(floatf(angle), floatf(x), floatf(y), floatf(z));
  }
}

GL_API void GL_APIENTRY glScalef(GLfloat x, GLfloat y, GLfloat z) {
  __profileAPI(" - %s( x=%f, y=%f, z=%f )\n", __FUNCTION__, x, y, z);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->scale(floatf(x), floatf(y), floatf(z));
  }
}

GL_API void GL_APIENTRY glTexEnvf(GLenum env, GLenum pname, GLfloat param) {
  __profileAPI(" - %s( env=%s, pname=%s, param=%s )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname),
               TexEnvValueToString(static_cast<GLint>(param)));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexEnv<float>(env, pname, &param);
  }
}

GL_API void GL_APIENTRY glTexEnvfv(GLenum env, GLenum pname,
                                   const GLfloat *pParams) {
  __profileAPI(" - %s( env=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexEnv<float>(env, pname, pParams);
  }
}

GL_API void GL_APIENTRY glTexParameterf(GLenum target, GLenum pname,
                                        GLfloat param) {
  __profileAPI(" - %s( target=%s, pname=%s, param=%s )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname),
               TexParamValueToString(static_cast<float>(GLint > (param))));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexParameter<float>(target, pname, &param);
  }
}

GL_API void GL_APIENTRY glTexParameterfv(GLenum target, GLenum pname,
                                         const GLfloat *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexParameter<float>(target, pname, pParams);
  }
}

GL_API void GL_APIENTRY glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
  __profileAPI(" - %s( x=%f, y=%f, z=%f )\n", __FUNCTION__, x, y, z);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->translate(floatf(x), floatf(y), floatf(z));
  }
}

/* Available in both Common and Common-Lite profiles */

GL_API void GL_APIENTRY glActiveTexture(GLenum texture) {
  __profileAPI(" - %s( texture=%s )\n", __FUNCTION__, TextureToString(texture));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setActiveTexture(texture);
  }
}

GL_API void GL_APIENTRY glAlphaFuncx(GLenum func, GLclampx ref) {
  __profileAPI(" - %s( func=%s, ref=%f )\n", __FUNCTION__, FuncToString(func),
               ref);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setAlphaFunc(func, static_cast<floatf>(fixed16::make(ref)));
  }
}

GL_API void GL_APIENTRY glBindBuffer(GLenum target, GLuint buffer) {
  __profileAPI(" - %s( target=%s, buffer=%d )\n", __FUNCTION__,
               BufferToString(target), buffer);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->bindBuffer(target, buffer);
  }
}

GL_API void GL_APIENTRY glBindTexture(GLenum target, GLuint texture) {
  __profileAPI(" - %s( target=%s, texture=%d )\n", __FUNCTION__,
               TextureTypeToString(target), texture);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->bindTexture(target, texture);
  }
}

GL_API void GL_APIENTRY glBlendFunc(GLenum sfactor, GLenum dfactor) {
  __profileAPI(" - %s( sfactor=%s, dfactor=%s )\n", __FUNCTION__,
               BlendFuncToString(sfactor), BlendFuncToString(dfactor));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setBlendFunc(sfactor, dfactor);
  }
}

GL_API void GL_APIENTRY glBufferData(GLenum target, GLsizeiptr size,
                                     const GLvoid *pData, GLenum usage) {
  __profileAPI(" - %s( target=%s, size=%d, pData=0x%p, usage=%s )\n",
               __FUNCTION__, BufferToString(target), size, pData,
               UsageToString(usage));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setBufferData(target, size, pData, usage);
  }
}

GL_API void GL_APIENTRY glBufferSubData(GLenum target, GLintptr offset,
                                        GLsizeiptr size, const GLvoid *pData) {
  __profileAPI(" - %s( target=%s, offset=%d, size=%d, pData=0x%p )\n",
               __FUNCTION__, BufferToString(target), offset, size, pData);

  if (nullptr == pData)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setBufferSubData(target, offset, size, pData);
  }
}

GL_API void GL_APIENTRY glClear(GLbitfield mask) {
  __profileAPI(" - %s( mask=0x%x )\n", __FUNCTION__, mask);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->clear(mask);
  }
}

GL_API void GL_APIENTRY glClearColorx(GLclampx red, GLclampx green,
                                      GLclampx blue, GLclampx alpha) {
  __profileAPI(" - %s( red=%f, green=%f, blue=%f, alpha=%f )\n", __FUNCTION__,
               red, green, blue, alpha);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->clearColor(static_cast<floatf>(fixed16::make(red)),
                         static_cast<floatf>(fixed16::make(green)),
                         static_cast<floatf>(fixed16::make(blue)),
                         static_cast<floatf>(fixed16::make(alpha)));
  }
}

GL_API void GL_APIENTRY glClearDepthx(GLclampx depth) {
  __profileAPI(" - %s( depth=%f )\n", __FUNCTION__, depth);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->clearDepth(static_cast<floatf>(fixed16::make(depth)));
  }
}

GL_API void GL_APIENTRY glClearStencil(GLint s) {
  __profileAPI(" - %s( s=%d )\n", __FUNCTION__, s);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->clearStencil(s);
  }
}

GL_API void GL_APIENTRY glClientActiveTexture(GLenum texture) {
  __profileAPI(" - %s( texture=%s )\n", __FUNCTION__, TextureToString(texture));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setClientActiveTexture(texture);
  }
}

GL_API void GL_APIENTRY glClipPlanex(GLenum plane, const GLfixed *pEquation) {
  __profileAPI(" - %s( plane=%d, pEquation=0x%p )\n", __FUNCTION__, plane,
               pEquation);

  if (nullptr == pEquation)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->setClipPlane(plane,
                           *reinterpret_cast<const VECTOR4 *>(pEquation));
#else
    pContext->setClipPlane(
        plane, VECTOR4(static_cast<floatf>(fixed16::make(pEquation[0])),
                       static_cast<floatf>(fixed16::make(pEquation[1])),
                       static_cast<floatf>(fixed16::make(pEquation[2])),
                       static_cast<floatf>(fixed16::make(pEquation[3]))));
#endif
  }
}

GL_API void GL_APIENTRY glColor4ub(GLubyte red, GLubyte green, GLubyte blue,
                                   GLubyte alpha) {
  __profileAPI(" - %s( red=%d, green=%d, blue=%d, alpha=%d )\n", __FUNCTION__,
               red, green, blue, alpha);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setColor(
        Math::FromUNORM8<floatf>(red), Math::FromUNORM8<floatf>(green),
        Math::FromUNORM8<floatf>(blue), Math::FromUNORM8<floatf>(alpha));
  }
}

GL_API void GL_APIENTRY glColor4x(GLfixed red, GLfixed green, GLfixed blue,
                                  GLfixed alpha) {
  __profileAPI(" - %s( red=%d, green=%d, blue=%d, alpha=%d )\n", __FUNCTION__,
               red, green, blue, alpha);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setColor(static_cast<floatf>(fixed16::make(red)),
                       static_cast<floatf>(fixed16::make(green)),
                       static_cast<floatf>(fixed16::make(blue)),
                       static_cast<floatf>(fixed16::make(alpha)));
  }
}

GL_API void GL_APIENTRY glColorMask(GLboolean red, GLboolean green,
                                    GLboolean blue, GLboolean alpha) {
  __profileAPI(" - %s( red=%d, green=%d, blue=%d, alpha=%d )\n", __FUNCTION__,
               red, green, blue, alpha);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setColorMask(red, green, blue, alpha);
  }
}

GL_API void GL_APIENTRY glColorPointer(GLint size, GLenum type, GLsizei stride,
                                       const GLvoid *pPointer) {
  __profileAPI(" - %s( size=%d, type=%s, stride=%d, pPointer=0x%p )\n",
               __FUNCTION__, size, TypeToString(type), stride, pPointer);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setColorPointer(size, type, stride, pPointer);
  }
}

GL_API void GL_APIENTRY glCompressedTexImage2D(GLenum target, GLint level,
                                               GLenum internalformat,
                                               GLsizei width, GLsizei height,
                                               GLint border, GLsizei imageSize,
                                               const GLvoid *pData) {
  __profileAPI(" - %s( target=%s, level=%d, internalformat=%s, width=%d, "
               "height=%d, border=%d, imageSize=%d, pDdata=0x%p )\n",
               __FUNCTION__, TextureTypeToString(target), level,
               FormatToString(internalformat), width, height, border, imageSize,
               pData);

  if (nullptr == pData)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->compressedTexImage2D(target, level, internalformat, width, height,
                                   border, imageSize, pData);
  }
}

GL_API void GL_APIENTRY glCompressedTexSubImage2D(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
    GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *pData) {
  __profileAPI(" - %s( target=%s, level=%d, xoffset=%d, yoffset=%d, width=%d, "
               "height=%d, format=%s, imageSize=%d, pData=0x%p )\n",
               __FUNCTION__, TextureTypeToString(target), level, xoffset,
               yoffset, width, height, FormatToString(format), imageSize,
               pData);

  if (nullptr == pData)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->compressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                      height, format, imageSize, pData);
  }
}

GL_API void GL_APIENTRY glCopyTexImage2D(GLenum target, GLint level,
                                         GLenum internalformat, GLint x,
                                         GLint y, GLsizei width, GLsizei height,
                                         GLint border) {
  __profileAPI(" - %s( target=%s, level=%d, internalformat=%s, x=%d, y=%d, "
               "width=%d, height=%d, border=%d )\n",
               __FUNCTION__, TextureTypeToString(target), level,
               FormatToString(internalformat), x, y, width, height, border);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->copyTexImage2D(target, level, internalformat, x, y, width, height,
                             border);
  }
}

GL_API void GL_APIENTRY glCopyTexSubImage2D(GLenum target, GLint level,
                                            GLint xoffset, GLint yoffset,
                                            GLint x, GLint y, GLsizei width,
                                            GLsizei height) {
  __profileAPI(" - %s( target=%s, level=%d, xoffset=%d, yoffset=%d, x=%d, "
               "y=%d, width=%d, height=%d )\n",
               __FUNCTION__, TextureTypeToString(target), level, xoffset,
               yoffset, x, y, width, height);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width,
                                height);
  }
}

GL_API void GL_APIENTRY glCullFace(GLenum mode) {
  __profileAPI(" - %s( mode=%s )\n", __FUNCTION__, CullFaceToString(mode));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setCullFace(mode);
  }
}

GL_API void GL_APIENTRY glDeleteBuffers(GLsizei n, const GLuint *pBuffers) {
  __profileAPI(" - %s( n=%d, pBuffers=0x%p )\n", __FUNCTION__, n, pBuffers);

  if (nullptr == pBuffers)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->deleteBuffers(n, pBuffers);
  }
}

GL_API void GL_APIENTRY glDeleteTextures(GLsizei n, const GLuint *pTextures) {
  __profileAPI(" - %s( n=%d, pTextures=0x%p )\n", __FUNCTION__, n, pTextures);

  if (nullptr == pTextures)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->deleteTextures(n, pTextures);
  }
}

GL_API void GL_APIENTRY glDepthFunc(GLenum func) {
  __profileAPI(" - %s( func=%s )\n", __FUNCTION__, FuncToString(func));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setDepthFunc(func);
  }
}

GL_API void GL_APIENTRY glDepthMask(GLboolean flag) {
  __profileAPI(" - %s( flag=%d )\n", __FUNCTION__, flag);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setDepthMask(flag);
  }
}

GL_API void GL_APIENTRY glDepthRangex(GLclampx zNear, GLclampx zFar) {
  __profileAPI(" - %s( zNear=%f, zfar=%f )\n", __FUNCTION__, zNear, zFar);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setDepthRange(static_cast<floatf>(fixed16::make(zNear)),
                            static_cast<floatf>(fixed16::make(zFar)));
  }
}

GL_API void GL_APIENTRY glDisable(GLenum cap) {
  __profileAPI(" - %s( cap=%s )\n", __FUNCTION__, CapToString(cap));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->activate(cap, false);
  }
}

GL_API void GL_APIENTRY glDisableClientState(GLenum array) {
  __profileAPI(" - %s( array=%s )\n", __FUNCTION__, ClientStateToString(array));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setClientState(array, false);
  }
}

GL_API void GL_APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count) {
  __profileAPI(" - %s( mode=%s, first=%d, count=%d )\n", __FUNCTION__,
               PrimitiveTypeToString(mode), first, count);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->drawArrays(mode, first, count);
  }
}

GL_API void GL_APIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type,
                                       const GLvoid *pIndices) {
  __profileAPI(" - %s( mode=%s, count=%d, type=%s, pIndices=0x%p )\n",
               __FUNCTION__, PrimitiveTypeToString(mode), count,
               TypeToString(type), pIndices);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->drawElements(mode, count, type, pIndices);
  }
}

GL_API void GL_APIENTRY glEnable(GLenum cap) {
  __profileAPI(" - %s( cap=%s )\n", __FUNCTION__, CapToString(cap));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->activate(cap, true);
  }
}

GL_API void GL_APIENTRY glEnableClientState(GLenum array) {
  __profileAPI(" - %s( array=%s )\n", __FUNCTION__, ClientStateToString(array));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setClientState(array, true);
  }
}

GL_API void GL_APIENTRY glFinish() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->finish();
  }
}

GL_API void GL_APIENTRY glFlush() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->flush();
  }
}

GL_API void GL_APIENTRY glFogx(GLenum pname, GLfixed param) {
  __profileAPI(" - %s( pname=%s, param=0x%x )\n", __FUNCTION__,
               FogToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setFog<fixed16>(pname, reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glFogxv(GLenum pname, const GLfixed *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               FogToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setFog<fixed16>(pname,
                              reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glFrontFace(GLenum mode) {
  __profileAPI(" - %s( mode=%s )\n", __FUNCTION__, FrontFaceToString(mode));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setFrontFace(mode);
  }
}

GL_API void GL_APIENTRY glFrustumx(GLfixed left, GLfixed right, GLfixed bottom,
                                   GLfixed top, GLfixed zNear, GLfixed zFar) {
  __profileAPI(
      " - %s( left=%d, right=%d, bottom=%d, top=%d, zNear=%d, zFar=%d )\n",
      __FUNCTION__, left, right, bottom, top, zNear, zFar);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->frustum(static_cast<floatf>(fixed16::make(left)),
                      static_cast<floatf>(fixed16::make(right)),
                      static_cast<floatf>(fixed16::make(bottom)),
                      static_cast<floatf>(fixed16::make(top)),
                      static_cast<floatf>(fixed16::make(zNear)),
                      static_cast<floatf>(fixed16::make(zFar)));
  }
}

GL_API void GL_APIENTRY glGetBooleanv(GLenum pname, GLboolean *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               AttributeToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->get<bool>(pname, reinterpret_cast<bool *>(pParams));
  }
}

GL_API void GL_APIENTRY glGetBufferParameteriv(GLenum target, GLenum pname,
                                               GLint *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               BufferToString(target), BufferParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getBufferParameter(target, pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetClipPlanex(GLenum plane, GLfixed eqn[4]) {
  __profileAPI(" - %s( plane=%s, eqn[4]=0x%p )\n", __FUNCTION__,
               PlaneToString(plane), eqn);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getClipPlane<fixed16>(plane, reinterpret_cast<fixed16 *>(eqn));
  }
}

GL_API void GL_APIENTRY glGenBuffers(GLsizei n, GLuint *pBuffers) {
  __profileAPI(" - %s( n=%d, pBuffers=0x%p )\n", __FUNCTION__, n, pBuffers);

  if (nullptr == pBuffers)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->genBuffers(n, pBuffers);
  }
}

GL_API void GL_APIENTRY glGenTextures(GLsizei n, GLuint *pTextures) {
  __profileAPI(" - %s( n=%d, pTextures=0x%p )\n", __FUNCTION__, n, pTextures);

  if (nullptr == pTextures)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->genTextures(n, pTextures);
  }
}

GL_API GLenum GL_APIENTRY glGetError() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err = GL_NO_ERROR;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    err = pContext->getError();
  }

  __glLog(" - glGetError()=%d\r\n", err);

  return err;
}

GL_API void GL_APIENTRY glGetFixedv(GLenum pname, GLfixed *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               AttributeToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->get<fixed16>(pname, reinterpret_cast<fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glGetIntegerv(GLenum pname, GLint *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               AttributeToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->get<int>(pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetLightxv(GLenum light, GLenum pname,
                                     GLfixed *pParams) {
  __profileAPI(" - %s( light=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               LightToString(light), LightParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getLight<fixed16>(light, pname,
                                reinterpret_cast<fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glGetMaterialxv(GLenum face, GLenum pname,
                                        GLfixed *pParams) {
  __profileAPI(" - %s( face=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               MaterialFaceToString(face), MaterialParamToString(pname),
               pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getMaterial<fixed16>(face, pname,
                                   reinterpret_cast<fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glGetPointerv(GLenum pname, GLvoid **ppParams) {
  __profileAPI(" - %s( pname=%s, ppParams=0x%p )\n", __FUNCTION__,
               PointerToString(pname), ppParams);

  if (nullptr == ppParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getPointer(ppParams, pname);
  }
}

GL_API const GLubyte *GL_APIENTRY glGetString(GLenum name) {
  __profileAPI(" - %s( name=%s )\n", __FUNCTION__, StringToString(name));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    return pContext->getString(name);
  }

  return nullptr;
}

GL_API void GL_APIENTRY glGetTexEnviv(GLenum env, GLenum pname,
                                      GLint *pParams) {
  __profileAPI(" - %s( env=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getTexEnv<int>(env, pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetTexEnvxv(GLenum env, GLenum pname,
                                      GLfixed *pParams) {
  __profileAPI(" - %s( env=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getTexEnv<fixed16>(env, pname,
                                 reinterpret_cast<fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glGetTexParameteriv(GLenum target, GLenum pname,
                                            GLint *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getTexParameter<int>(target, pname, pParams);
  }
}

GL_API void GL_APIENTRY glGetTexParameterxv(GLenum target, GLenum pname,
                                            GLfixed *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->getTexParameter<fixed16>(target, pname,
                                       reinterpret_cast<fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glHint(GLenum target, GLenum mode) {
  __profileAPI(" - %s( target=%s, mode=%s )\n", __FUNCTION__,
               HintTargetToString(target), HintModeToString(mode));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    return pContext->hint(target, mode);
  }
}

GL_API GLboolean GL_APIENTRY glIsBuffer(GLuint buffer) {
  __profileAPI(" - %s( buffer=%d )\n", __FUNCTION__, buffer);

  bool bValue = false;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    bValue = pContext->isBuffer(buffer);
  }

  __glLog(" - glIsBuffer()=%d\r\n", bValue);

  return bValue;
}

GL_API GLboolean GL_APIENTRY glIsEnabled(GLenum cap) {
  __profileAPI(" - %s( cap=%s )\n", __FUNCTION__, CapToString(cap));

  bool bValue = false;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    bValue = pContext->isEnabled(cap);
  }

  __glLog(" - glIsEnabled()=%d\r\n", bValue);

  return bValue;
}

GL_API GLboolean GL_APIENTRY glIsTexture(GLuint texture) {
  __profileAPI(" - %s( texture=%d )\n", __FUNCTION__, texture);

  bool bValue = false;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    bValue = pContext->isTexture(texture);
  }

  __glLog(" - glIsTexture()=%d\r\n", bValue);

  return bValue;
}

GL_API void GL_APIENTRY glLightModelx(GLenum pname, GLfixed param) {
  __profileAPI(" - %s( pname=%s, param=%d )\n", __FUNCTION__,
               LightModelToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameterModel<fixed16>(
        pname, reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glLightModelxv(GLenum pname, const GLfixed *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=(0x%x ,0x%x, 0x%x, 0x%x) )\n",
               __FUNCTION__, LightModelToString(pname), pParams[0], pParams[1],
               pParams[2], pParams[3]);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameterModel<fixed16>(
        pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glLightx(GLenum light, GLenum pname, GLfixed param) {
  __profileAPI(" - %s( light=%s, pname=%s, param=%d )\n", __FUNCTION__,
               LightToString(light), LightParamToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameter<fixed16>(
        light, pname, reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glLightxv(GLenum light, GLenum pname,
                                  const GLfixed *pParams) {
  __profileAPI(
      " - %s( light=%s, pname=%s, pParams=(0x%x, 0x%x, 0x%x, 0x%x) )\n",
      __FUNCTION__, LightToString(light), LightParamToString(pname), pParams[0],
      pParams[1], pParams[2], pParams[3]);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLightParameter<fixed16>(
        light, pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glLineWidthx(GLfixed width) {
  __profileAPI(" - %s( width=%d )\n", __FUNCTION__, width);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLineWidth(static_cast<floatf>(fixed16::make(width)));
  }
}

GL_API void GL_APIENTRY glLoadIdentity() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->loadIdentity();
  }
}

GL_API void GL_APIENTRY glLoadMatrixx(const GLfixed *pM) {
  __profileAPI(" - %s( pM=(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, "
               "0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x) )\n",
               __FUNCTION__, pM[0], pM[1], pM[2], pM[3], pM[4], pM[5], pM[6],
               pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13], pM[14],
               pM[15]);

  if (nullptr == pM)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->loadMatrix(*reinterpret_cast<const MATRIX44 *>(pM));
#else
    pContext->loadMatrix(MATRIX44(static_cast<floatf>(fixed16::make(pM[0])),
                                  static_cast<floatf>(fixed16::make(pM[1])),
                                  static_cast<floatf>(fixed16::make(pM[2])),
                                  static_cast<floatf>(fixed16::make(pM[3])),
                                  static_cast<floatf>(fixed16::make(pM[4])),
                                  static_cast<floatf>(fixed16::make(pM[5])),
                                  static_cast<floatf>(fixed16::make(pM[6])),
                                  static_cast<floatf>(fixed16::make(pM[7])),
                                  static_cast<floatf>(fixed16::make(pM[8])),
                                  static_cast<floatf>(fixed16::make(pM[9])),
                                  static_cast<floatf>(fixed16::make(pM[10])),
                                  static_cast<floatf>(fixed16::make(pM[11])),
                                  static_cast<floatf>(fixed16::make(pM[12])),
                                  static_cast<floatf>(fixed16::make(pM[13])),
                                  static_cast<floatf>(fixed16::make(pM[14])),
                                  static_cast<floatf>(fixed16::make(pM[15]))));
#endif
  }
}

GL_API void GL_APIENTRY glLogicOp(GLenum opcode) {
  __profileAPI(" - %s( opcode=%s )\n", __FUNCTION__, LogicOpToString(opcode));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setLogicOp(opcode);
  }
}

GL_API void GL_APIENTRY glMaterialx(GLenum face, GLenum pname, GLfixed param) {
  __profileAPI(" - %s( face=%s, pname=%s, param=%d )\n", __FUNCTION__,
               MaterialFaceToString(face), MaterialParamToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMaterial<fixed16>(face, pname,
                                   reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glMaterialxv(GLenum face, GLenum pname,
                                     const GLfixed *pParams) {
  __profileAPI(" - %s( face=%s, pname=%s, pParams=(0x%x, 0x%x, 0x%x, 0x%x) )\n",
               __FUNCTION__, MaterialFaceToString(face),
               MaterialParamToString(pname), pParams[0], pParams[1], pParams[2],
               pParams[3]);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMaterial<fixed16>(face, pname,
                                   reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glMatrixMode(GLenum mode) {
  __profileAPI(" - %s( mode=%s )\n", __FUNCTION__, MatrixModeToString(mode));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMatrixMode(mode);
  }

  return;
}

GL_API void GL_APIENTRY glMultMatrixx(const GLfixed *pM) {
  __profileAPI(" - %s( pM=(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, "
               "0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x) )\n",
               __FUNCTION__, pM[0], pM[1], pM[2], pM[3], pM[4], pM[5], pM[6],
               pM[7], pM[8], pM[9], pM[10], pM[11], pM[12], pM[13], pM[14],
               pM[15]);

  if (nullptr == pM)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
#ifdef COCOGL_PIXEDPOINT
    pContext->multiply(*reinterpret_cast<const MATRIX44 *>(pM));
#else
    pContext->multiply(MATRIX44(static_cast<floatf>(fixed16::make(pM[0])),
                                static_cast<floatf>(fixed16::make(pM[1])),
                                static_cast<floatf>(fixed16::make(pM[2])),
                                static_cast<floatf>(fixed16::make(pM[3])),
                                static_cast<floatf>(fixed16::make(pM[4])),
                                static_cast<floatf>(fixed16::make(pM[5])),
                                static_cast<floatf>(fixed16::make(pM[6])),
                                static_cast<floatf>(fixed16::make(pM[7])),
                                static_cast<floatf>(fixed16::make(pM[8])),
                                static_cast<floatf>(fixed16::make(pM[9])),
                                static_cast<floatf>(fixed16::make(pM[10])),
                                static_cast<floatf>(fixed16::make(pM[11])),
                                static_cast<floatf>(fixed16::make(pM[12])),
                                static_cast<floatf>(fixed16::make(pM[13])),
                                static_cast<floatf>(fixed16::make(pM[14])),
                                static_cast<floatf>(fixed16::make(pM[15]))));
#endif
  }
}

GL_API void GL_APIENTRY glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t,
                                          GLfixed r, GLfixed q) {
  __profileAPI(" - %s( target=%s, s=%d, t=%d, r=%d, q=%d )\n", __FUNCTION__,
               TextureToString(target), s, t, r, q);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setMultiTexCoord(target, static_cast<floatf>(fixed16::make(s)),
                               static_cast<floatf>(fixed16::make(t)),
                               static_cast<floatf>(fixed16::make(r)),
                               static_cast<floatf>(fixed16::make(q)));
  }
}

GL_API void GL_APIENTRY glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz) {
  __profileAPI(" - %s( nx=%d, ny=%d, nz=%d )\n", __FUNCTION__, nx, ny, nz);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setNormal(static_cast<floatf>(fixed16::make(nx)),
                        static_cast<floatf>(fixed16::make(ny)),
                        static_cast<floatf>(fixed16::make(nz)));
  }
}

GL_API void GL_APIENTRY glNormalPointer(GLenum type, GLsizei stride,
                                        const GLvoid *pPointer) {
  __profileAPI(" - %s( type=%s, stride=%d, pPointer=0x%p )\n", __FUNCTION__,
               TypeToString(type), stride, pPointer);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setNormalPointer(type, stride, pPointer);
  }
}

GL_API void GL_APIENTRY glOrthox(GLfixed left, GLfixed right, GLfixed bottom,
                                 GLfixed top, GLfixed zNear, GLfixed zFar) {
  __profileAPI(
      " - %s( left=%d, right=%d, bottom=%d, top=%d, zNear=%d, zFar=%d )\n",
      __FUNCTION__, left, right, bottom, top, zNear, zFar);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->ortho(static_cast<floatf>(fixed16::make(left)),
                    static_cast<floatf>(fixed16::make(right)),
                    static_cast<floatf>(fixed16::make(bottom)),
                    static_cast<floatf>(fixed16::make(top)),
                    static_cast<floatf>(fixed16::make(zNear)),
                    static_cast<floatf>(fixed16::make(zFar)));
  }
}

GL_API void GL_APIENTRY glPixelStorei(GLenum pname, GLint param) {
  __profileAPI(" - %s( pname=%s, param=%d )\n", __FUNCTION__,
               PixelStoreToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPixelStorei(pname, param);
  }
}

GL_API void GL_APIENTRY glPointParameterx(GLenum pname, GLfixed param) {
  __profileAPI(" - %s( pname=%s, param=%d )\n", __FUNCTION__,
               PointParameterToString(pname), param);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointParameter<fixed16>(
        pname, reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glPointParameterxv(GLenum pname,
                                           const GLfixed *pParams) {
  __profileAPI(" - %s( pname=%s, pParams=0x%p )\n", __FUNCTION__,
               PointParameterToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointParameter<fixed16>(
        pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glPointSizex(GLfixed size) {
  __profileAPI(" - %s( size=%d )\n", __FUNCTION__, size);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointSize(static_cast<floatf>(fixed16::make(size)));
  }
}

GL_API void GL_APIENTRY glPolygonOffsetx(GLfixed factor, GLfixed units) {
  __profileAPI(" - %s( factor=%d, units=%d )\n", __FUNCTION__, factor, units);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPolygonOffset(static_cast<floatf>(fixed16::make(factor)),
                               static_cast<floatf>(fixed16::make(units)));
  }
}

GL_API void GL_APIENTRY glPopMatrix() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->popMatrix();
  }
}

GL_API void GL_APIENTRY glPushMatrix() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->pushMatrix();
  }
}

GL_API void GL_APIENTRY glReadPixels(GLint x, GLint y, GLsizei width,
                                     GLsizei height, GLenum format, GLenum type,
                                     GLvoid *pPixels) {
  __profileAPI(" - %s( x=%d, y=%d, width=%d, height=%d, format=%s, "
               "type=%s, pPixels=0x%p )\n",
               __FUNCTION__, x, y, width, height, FormatToString(format),
               TypeToString(type), pPixels);

  if (nullptr == pPixels)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->readPixels(x, y, width, height, format, type, pPixels);
  }
}

GL_API void GL_APIENTRY glRotatex(GLfixed angle, GLfixed x, GLfixed y,
                                  GLfixed z) {
  __profileAPI(" - %s( angle=%d, x=%d, y=%d, z=%d )\n", __FUNCTION__, angle, x,
               y, z);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->rotate(static_cast<floatf>(fixed16::make(angle)),
                     static_cast<floatf>(fixed16::make(x)),
                     static_cast<floatf>(fixed16::make(y)),
                     static_cast<floatf>(fixed16::make(z)));
  }
}

GL_API void GL_APIENTRY glSampleCoverage(GLclampf value, GLboolean invert) {
  __profileAPI(" - %s( value=%f, invert=%d )\n", __FUNCTION__, value, invert);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setSampleCoverage(floatf(value), invert);
  }
}

GL_API void GL_APIENTRY glSampleCoveragex(GLclampx value, GLboolean invert) {
  __profileAPI(" - %s( value=%f, invert=%d )\n", __FUNCTION__, value, invert);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setSampleCoverage(static_cast<floatf>(fixed16::make(value)),
                                invert);
  }
}

GL_API void GL_APIENTRY glScalex(GLfixed x, GLfixed y, GLfixed z) {
  __profileAPI(" - %s( x=%d, y=%d, z=%d )\n", __FUNCTION__, x, y, z);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->scale(static_cast<floatf>(fixed16::make(x)),
                    static_cast<floatf>(fixed16::make(y)),
                    static_cast<floatf>(fixed16::make(z)));
  }
}

GL_API void GL_APIENTRY glScissor(GLint x, GLint y, GLsizei width,
                                  GLsizei height) {
  __profileAPI(" - %s( x=%d, y=%d, width=%d, height=%d )\n", __FUNCTION__, x, y,
               width, height);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setScissor(x, y, width, height);
  }
}

GL_API void GL_APIENTRY glShadeModel(GLenum mode) {
  __profileAPI(" - %s( mode=%s )\n", __FUNCTION__, ShadeModelToString(mode));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setShadeModel(mode);
  }
}

GL_API void GL_APIENTRY glStencilFunc(GLenum func, GLint ref, GLuint mask) {
  __profileAPI(" - %s( func=%s, ref=%d, mask=0x%x )\n", __FUNCTION__,
               FuncToString(func), ref, mask);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setStencilFunc(func, ref, mask);
  }
}

GL_API void GL_APIENTRY glStencilMask(GLuint mask) {
  __profileAPI(" - %s( mask=0x%x )\n", __FUNCTION__, mask);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setStencilMask(mask);
  }
}

GL_API void GL_APIENTRY glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
  __profileAPI(" - %s( fail=%s, zfail=%s, zpass=%s )\n", __FUNCTION__,
               StencilOpToString(fail), StencilOpToString(zfail),
               StencilOpToString(zpass));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setStencilOp(fail, zfail, zpass);
  }
}

GL_API void GL_APIENTRY glTexCoordPointer(GLint size, GLenum type,
                                          GLsizei stride,
                                          const GLvoid *pPointer) {
  __profileAPI(" - %s( size=%d, type=%s, stride=%d, pPointer=0x%p )\n",
               __FUNCTION__, size, TypeToString(type), stride, pPointer);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexCoordPointer(size, type, stride, pPointer);
  }
}

GL_API void GL_APIENTRY glTexEnvi(GLenum env, GLenum pname, GLint param) {
  __profileAPI(" - %s( env=%s, pname=%s, param=%s )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname),
               TexEnvValueToString(param));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexEnv<int>(env, pname, &param);
  }
}

GL_API void GL_APIENTRY glTexEnvx(GLenum env, GLenum pname, GLfixed param) {
  __profileAPI(" - %s( env=%s, pname=%s, param=%s )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname),
               TexEnvValueToString(param));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexEnv<fixed16>(env, pname,
                                 reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glTexEnviv(GLenum env, GLenum pname,
                                   const GLint *pParams) {
  __profileAPI(" - %s( env=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexEnv<int>(env, pname, pParams);
  }
}

GL_API void GL_APIENTRY glTexEnvxv(GLenum env, GLenum pname,
                                   const GLfixed *pParams) {
  __profileAPI(" - %s( env=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TexEnvToString(env), TexEnvParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexEnv<fixed16>(env, pname,
                                 reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glTexImage2D(GLenum target, GLint level,
                                     GLint internalformat, GLsizei width,
                                     GLsizei height, GLint border,
                                     GLenum format, GLenum type,
                                     const GLvoid *pPixels) {
  __profileAPI(" - %s( target=%s, level=%d, internalformat=%s, width=%d, "
               "height=%d, border=%d, format=%s, type=%s, pPixels=0x%p )\n",
               __FUNCTION__, TextureTypeToString(target), level,
               FormatToString(internalformat), width, height, border,
               FormatToString(format), TypeToString(type), pPixels);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexImage2D(target, level, internalformat, width, height,
                            border, format, type, pPixels);
  }
}

GL_API void GL_APIENTRY glTexParameteri(GLenum target, GLenum pname,
                                        GLint param) {
  __profileAPI(" - %s( target=%s, pname=%s, param=%s )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname),
               TexParamValueToString(param));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexParameter<int>(target, pname, &param);
  }
}

GL_API void GL_APIENTRY glTexParameterx(GLenum target, GLenum pname,
                                        GLfixed param) {
  __profileAPI(" - %s( target=%s, pname=%s, param=%s )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname),
               TexParamValueToString(param));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexParameter<fixed16>(
        target, pname, reinterpret_cast<const fixed16 *>(&param));
  }
}

GL_API void GL_APIENTRY glTexParameteriv(GLenum target, GLenum pname,
                                         const GLint *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexParameter<int>(target, pname, pParams);
  }
}

GL_API void GL_APIENTRY glTexParameterxv(GLenum target, GLenum pname,
                                         const GLfixed *pParams) {
  __profileAPI(" - %s( target=%s, pname=%s, pParams=0x%p )\n", __FUNCTION__,
               TextureTypeToString(target), TexParamToString(pname), pParams);

  if (nullptr == pParams)
    return;

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexParameter<fixed16>(
        target, pname, reinterpret_cast<const fixed16 *>(pParams));
  }
}

GL_API void GL_APIENTRY glTexSubImage2D(GLenum target, GLint level,
                                        GLint xoffset, GLint yoffset,
                                        GLsizei width, GLsizei height,
                                        GLenum format, GLenum type,
                                        const GLvoid *pPixels) {
  __profileAPI(" - %s( target=%s, level=%d, xoffset=%d, yoffset=%d, "
               "width=%d, height=%d, format=%s, type=%s, pPixels=0x%p )\n",
               __FUNCTION__, TextureTypeToString(target), level, xoffset,
               yoffset, width, height, FormatToString(format),
               TypeToString(type), pPixels);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setTexSubImage2D(target, level, xoffset, yoffset, width, height,
                               format, type, pPixels);
  }
}

GL_API void GL_APIENTRY glTranslatex(GLfixed x, GLfixed y, GLfixed z) {
  __profileAPI(" - %s( x=%d, y=%d, z=%d )\n", __FUNCTION__, x, y, z);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->translate(static_cast<floatf>(fixed16::make(x)),
                        static_cast<floatf>(fixed16::make(y)),
                        static_cast<floatf>(fixed16::make(z)));
  }
}

GL_API void GL_APIENTRY glVertexPointer(GLint size, GLenum type, GLsizei stride,
                                        const GLvoid *pPointer) {
  __profileAPI(" - %s( size=%d, type=%s, stride=%d, pPointer=0x%p )\n",
               __FUNCTION__, size, TypeToString(type), stride, pPointer);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setVertexPointer(size, type, stride, pPointer);
  }
}

GL_API void GL_APIENTRY glViewport(GLint x, GLint y, GLsizei width,
                                   GLsizei height) {
  __profileAPI(" - %s( x=%d, y=%d, width=%d, height=%d )\n", __FUNCTION__, x, y,
               width, height);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setViewport(x, y, width, height);
  }
}

GL_API void GL_APIENTRY glPointSizePointerOES(GLenum type, GLsizei stride,
                                              const GLvoid *pPointer) {
  __profileAPI(" - %s( type=%s, stride=%d, pPointer=0x%p )\n", __FUNCTION__,
               TypeToString(type), stride, pPointer);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    pContext->setPointSizePointerOES(type, stride, pPointer);
  }
}

GL_API GLbitfield GL_APIENTRY glQueryMatrixxOES(GLfixed mantissa[16],
                                                GLint exponent[16]) {
  __profileAPI(" - %s( mantissa=0x%p, exponent=0x%p )\n", __FUNCTION__,
               mantissa, exponent);

  auto pContext = g_driver.getCurrentContext();
  if (nullptr == pContext)
    return GL_INVALID_OPERATION;

  return pContext->queryMatrix<fixed16>(reinterpret_cast<fixed16 *>(mantissa),
                                        exponent);
}
