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

#define MAX_TEXTURES 2
#define MAX_TEXTURES_MASK 2
#define MAX_TEXTURE_LEVELS 12
#define MAX_TEXTURE_SIZE (1 << MAX_TEXTURE_LEVELS)
#define MAX_CLIPPLANES 6
#define CLIPPLANES_MASK 63
#define MAX_LIGHTS 8
#define LIGHTS_MASK 255
#define MAX_REGISTERS 13
#define MAX_TILE_SIZE 256000000
#define MAX_BLOCK_SIZE 16

#define __GL_NO_DATA 0xffffffff

#define IMPLEMENTATION_COLOR_READ_TYPE_OES GL_UNSIGNED_SHORT_5_6_5
#define IMPLEMENTATION_COLOR_READ_FORMAT_OES GL_RGB

#define fixedRX fixed20
#define fixedRF float20
#define fixedW fixed12
#define fixedDDA fixed16

#ifdef COCOGL_PIXEDPOINT
#define floatRW Fixed<28, int64_t>
#define floatQ Fixed<30, int64_t>
#else
#define floatRW float
#define floatQ float
#endif

#define EGL_CONFIG_VENDOR "Blaise Tine"
#define EGL_CONFIG_VERSION "OpenGL ES-CM 1.1"
#define EGL_CONFIG_RENDERER "Software"
#define EGL_CONFIG_EXTENSIONS                                                  \
  "GL_OES_fixed_point "                                                        \
  "GL_OES_single_precision "                                                   \
  "GL_OES_read_format "

#define __enum_bitfield_begin enum { __offset0 = 0,

#define __enum_bitfield_end                                                    \
  }                                                                            \
  ;

#define __enum_bitfield(pos, pos1, prefix, size)                               \
  prefix##_SHIFT = __offset##pos, prefix##_SIZE = size,                        \
  prefix##_MASK = ((uint32_t)((1 << size) - 1) << prefix##_SHIFT),             \
  __offset##pos1 = (prefix##_SHIFT + size)

#define __get_bitfield(mask, prefix)                                           \
  ((uint32_t)(mask & prefix##_MASK) >> prefix##_SHIFT)

#define __set_bitfield(mask, prefix, value)                                    \
    mask = (((uint32_t)(value) << (prefix##_SHIFT)) | (mask & ~(prefix##_MASK))))

enum eHANDLETYPE {
  HANDLE_NONE = 0,
  HANDLE_DISPLAY = 1,
  HANDLE_CONFIG = 2,
  HANDLE_CONTEXT = 3,
  HANDLE_SURFACE = 4,
  HANDLE_BUFFER = 5,
  HANDLE_TEXTURE = 6,
};

struct VECTOR2B {
  enum { DIM = 2 };

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      GLbyte x;
      GLbyte y;
    };
    struct {
      GLbyte m[DIM];
    };
  };
  DISABLE_WARNING_POP

  VECTOR2B(const VECTOR2B &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
  }
};

struct VECTOR2S {
  enum { DIM = 2 };

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      GLshort x;
      GLshort y;
    };
    struct {
      GLshort m[DIM];
    };
  };
  DISABLE_WARNING_POP

  VECTOR2S(const VECTOR2S &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
  }
};

struct VECTOR3B {
  enum { DIM = 3 };

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      GLbyte x;
      GLbyte y;
      GLbyte z;
    };
    struct {
      GLbyte m[DIM];
    };
  };
  DISABLE_WARNING_POP

  VECTOR3B(const VECTOR3B &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
  }
};

struct VECTOR3S {
  enum { DIM = 3 };

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      GLshort x;
      GLshort y;
      GLshort z;
    };
    struct {
      GLshort m[DIM];
    };
  };
  DISABLE_WARNING_POP

  VECTOR3S(const VECTOR3S &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
  }
};

struct VECTOR4B {
  enum { DIM = 4 };

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      GLbyte x;
      GLbyte y;
      GLbyte z;
      GLbyte w;
    };
    struct {
      GLbyte m[DIM];
    };
  };
  DISABLE_WARNING_POP

  VECTOR4B(const VECTOR4B &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;
  }
};

struct VECTOR4S {
  enum { DIM = 4 };

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      GLshort x;
      GLshort y;
      GLshort z;
      GLshort w;
    };
    struct {
      GLshort m[DIM];
    };
  };
  DISABLE_WARNING_POP

  VECTOR4S(const VECTOR4S &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;
  }
};

struct TEXCOORD2 {
  enum { DIM = 2 };
  float20 m[DIM];

  TEXCOORD2() {}

  TEXCOORD2(const TEXCOORD2 &rhs) {
    this->m[0] = rhs.m[0];
    this->m[1] = rhs.m[1];
  }

  auto &u() const { return m[0]; }
  auto &u() { return m[0]; }
  auto &v() const { return m[1]; }
  auto &v() { return m[1]; }
};

enum eVERTEXDATA {
  VERTEXDATA_FLAGS,
  VERTEXDATA_WORLDPOS,
  VERTEXDATA_EYEPOS = VERTEXDATA_WORLDPOS,
  VERTEXDATA_CLIPPOS,
  VERTEXDATA_SCREENPOS,
  VERTEXDATA_POINTSIZE,
  VERTEXDATA_FRONTCOLOR,
  VERTEXDATA_COLOR0 = VERTEXDATA_FRONTCOLOR,
  VERTEXDATA_BACKCOLOR,
  VERTEXDATA_COLOR1 = VERTEXDATA_BACKCOLOR,
  VERTEXDATA_TEXCOORD0,
  VERTEXDATA_TEXCOORD1,
  VERTEXDATA_FOG,
  VERTEXDATA_SIZE,
};

enum eVertexFormat {
  VERTEX_UNKNOWN,
  VERTEX_BYTE,
  VERTEX_BYTE2,
  VERTEX_BYTE3,
  VERTEX_BYTE4,
  VERTEX_SHORT,
  VERTEX_SHORT2,
  VERTEX_SHORT3,
  VERTEX_SHORT4,
  VERTEX_FIXED,
  VERTEX_FIXED2,
  VERTEX_FIXED3,
  VERTEX_FIXED4,
  VERTEX_FLOAT,
  VERTEX_FLOAT2,
  VERTEX_FLOAT3,
  VERTEX_FLOAT4,
  VERTEX_RGBA_,
  VERTEX_RGBA__,
  VERTEX_RGBA___,
  VERTEX_RGBA,
};

struct Viewport {
  int Left;
  int Top;
  int Width;
  int Height;
};

struct DepthRange {
  floatf fNear;
  floatf fFar;
};

struct SampleCoverage {
  floatf fValue;
  bool bInvert;
};

struct PolygonOffset {
  floatf fFactor;
  floatf fUnits;
};

struct ScreenXform {
  fixed4 fMinX;
  int iScaleX;

  fixed4 fMinY;
  int iScaleY;

  float20 fMinZ;
  floatf fScaleZ;
};

enum eFogMode {
  FogLinear,
  FogExp,
  FogExp2,
};

struct Fog {
  enum {
    FACTORS_FIRST = GL_FOG_DENSITY,
    FACTORS_LAST = GL_FOG_END,
    FACTORS_SIZE = (FACTORS_LAST - FACTORS_FIRST) + 1,
  };

  eFogMode Mode;

  void setFactor(GLenum pname, floatf fValue) {
    assert(pname >= FACTORS_FIRST && pname <= FACTORS_LAST);
    factors_[pname - FACTORS_FIRST] = fValue;
  }

  floatf getFactor(GLenum pname) const {
    assert(pname >= FACTORS_FIRST && pname <= FACTORS_LAST);
    return factors_[pname - FACTORS_FIRST];
  }

private:
  floatf factors_[FACTORS_SIZE];

public:
  fixedRF fRatio;
};

struct PointParams {
  enum {
    PARAMS_FIRST = GL_POINT_SIZE_MIN,
    PARAMS_LAST = GL_POINT_FADE_THRESHOLD_SIZE,
    PARAMS_SIZE = (PARAMS_LAST - PARAMS_FIRST) + 1,
  };

  VECTOR3 vAttenuation;

  floatf get(GLenum pname) {
    assert(pname >= PARAMS_FIRST && pname <= PARAMS_LAST);
    return params_[pname - PARAMS_FIRST];
  }

  void set(GLenum pname, floatf fValue) {
    assert(pname >= PARAMS_FIRST && pname <= PARAMS_LAST);
    params_[pname - PARAMS_FIRST] = fValue;
  }

private:
  floatf params_[PARAMS_SIZE];
};

enum eHint {
  HINT_DONTCARE,
  HINT_FASTEST,
  HINT_NICEST,
};

struct HINTS {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Value;
    struct {
      uint32_t Perspective : 2;
      uint32_t PointSmooth : 2;
      uint32_t LineSmooth : 2;
      uint32_t Fog : 2;
      uint32_t GenerateMipmap : 2;
    };
  };
  DISABLE_WARNING_POP

  HINTS() : Value(0) { static_assert(sizeof(HINTS) == sizeof(Value)); }
};

enum eTexFilter {
  FILTER_NONE,
  FILTER_NEAREST,
  FILTER_LINEAR,
  FILTER_NEAREST_MIPMAP_NEAREST,
  FILTER_LINEAR_MIPMAP_NEAREST,
  FILTER_NEAREST_MIPMAP_LINEAR,
  FILTER_LINEAR_MIPMAP_LINEAR,
  FILTER_SIZE_,
};

enum eTexAddress {
  ADDRESS_WRAP,
  ADDRESS_CLAMP,
  ADDRESS_SIZE_,
};

enum eEnvMode {
  ENVMODE_ADD,
  ENVMODE_BLEND,
  ENVMODE_REPLACE,
  ENVMODE_MODULATE,
  ENVMODE_DECAL,
  ENVMODE_SIZE_,
};

enum ecompare {
  COMPARE_NEVER,
  COMPARE_LESS,
  COMPARE_EQUAL,
  COMPARE_LEQUAL,
  COMPARE_GREATER,
  COMPARE_NOTEQUAL,
  COMPARE_GEQUAL,
  COMPARE_ALWAYS,
  COMPARE_SIZE_,
};

enum eStencilOp {
  STENCIL_KEEP,
  STENCIL_REPLACE,
  STENCIL_INCR,
  STENCIL_DECR,
  STENCIL_ZERO,
  STENCIL_INVERT,
  STENCIL_SIZE_,
};

enum eBlendOp {
  BLEND_ZERO,
  BLEND_ONE,
  BLEND_SRC_COLOR,
  BLEND_ONE_MINUS_SRC_COLOR,
  BLEND_SRC_ALPHA,
  BLEND_ONE_MINUS_SRC_ALPHA,
  BLEND_DST_ALPHA,
  BLEND_ONE_MINUS_DST_ALPHA,
  BLEND_DST_COLOR,
  BLEND_ONE_MINUS_DST_COLOR,
  BLEND_SRC_ALPHA_SATURATE,
  BLEND_SIZE_,
};

enum eLogicOp {
  LOGICOP_CLEAR,
  LOGICOP_AND,
  LOGICOP_AND_REVERSE,
  LOGICOP_COPY,
  LOGICOP_AND_INVERTED,
  LOGICOP_NOOP,
  LOGICOP_XOR,
  LOGICOP_OR,
  LOGICOP_NOR,
  LOGICOP_EQUIV,
  LOGICOP_INVERT,
  LOGICOP_OR_REVERSE,
  LOGICOP_COPY_INVERTED,
  LOGICOP_OR_INVERTED,
  LOGICOP_NAND,
  LOGICOP_SET,
  LOGICOP_SIZE_,
};

enum eCullFace {
  CULL_BACK,
  CULL_FRONT,
  CULL_FRONT_AND_BACK,
};

enum eCullMode {
  CULL_CCW,
  CULL_CW,
};

struct CullStates {
  bool bCullFaceEnabled;
  bool bTwoSidedLighting;
  uint8_t FrontFace;
  uint8_t CullFace;
};

enum eShadeModel {
  SHADE_FLAT,
  SHADE_SMOOTH,
};

struct TexParams {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Value;
    struct {
      uint32_t MinFilter : 3;
      uint32_t MagFilter : 3;
      uint32_t AddressU : 2;
      uint32_t AddressV : 2;
    };
  };
  DISABLE_WARNING_POP
  TexParams() : Value(0) { static_assert(sizeof(TexParams) == sizeof(Value)); }

  TexParams(uint32_t value) { this->Value = value; }

  bool operator==(const TexParams &rhs) const {
    return (this->Value == rhs.Value);
  }
};

struct DirtyFlags {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Value;
    struct {
      uint32_t ModelViewInvT44 : 1;
      uint32_t ModelViewInvT33 : 1;
      uint32_t ProjectionInvT : 1;
      uint32_t ModelViewProj : 1;
      uint32_t Lights : 1;
      uint32_t ScreenXform : 1;
      uint32_t ScissorRECT : 1;
      uint32_t ScaledAmbient : 1;
      uint32_t FogRatio : 1;
      uint32_t ClipPlanesCS : MAX_CLIPPLANES;
      uint32_t DefaultColor : 1;
      uint32_t NormalizeNormal : 1;
      uint32_t ClearColor : 1;
      uint32_t ClearDepth : 1;
      uint32_t ColorWriteMask : 1;
      uint32_t FogColor : 1;
    };
  };
  DISABLE_WARNING_POP
  DirtyFlags() : Value(0) {
    static_assert(sizeof(DirtyFlags) == sizeof(Value));
  }
};

struct DirtyLights {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Value;
    struct {
      uint8_t States[4];
    };
    struct {
      uint8_t Ambient;
      uint8_t Diffuse;
      uint8_t Specular;
      uint8_t Position;
    };
  };
  DISABLE_WARNING_POP
  DirtyLights() : Value(0) {
    static_assert(sizeof(DirtyLights) == sizeof(Value));
  }
};

struct RDVECTOR {
  fixed4 x;
  fixed4 y;
  float20 z;
  floatRW rhw;

  RDVECTOR() {}
};

enum eClipFlags {
  CLIP_LEFT,
  CLIP_RIGHT,
  CLIP_BOTTOM,
  CLIP_TOP,
  CLIP_FRONT,
  CLIP_BACK,
  CLIP_PLANE0,
  CLIP_PLANE1,
  CLIP_PLANE2,
  CLIP_PLANE3,
  CLIP_PLANE4,
  CLIP_PLANE5,
  CLIP_PLANES,
  CLIPPING_MASK = (1 << CLIP_PLANES) - 1,
};

enum eTextureUnit {
  TEXTURE_UNIT0 = 1 << 0,
  TEXTURE_UNIT1 = 1 << 1,
  TEXTURE_UNIT11 = TEXTURE_UNIT0 | TEXTURE_UNIT1
};

struct VertexStates {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Value;
    struct {
      uint32_t Position : 1;
      uint32_t Normal : 1;
      uint32_t Color : 1;
      uint32_t TexCoords : MAX_TEXTURES;
      uint32_t PointSize : 1;
    };
  };
  DISABLE_WARNING_POP

  VertexStates() : Value(0) {
    static_assert(sizeof(VertexStates) == sizeof(Value));
  }
};

struct GLCAPS {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Flags1;
    struct {
      uint32_t DepthTest : 1;
      uint32_t ShadeModel : 1;
      uint32_t Lighting : 1;
      uint32_t CullFace : 1;
      uint32_t AlphaTest : 1;
      uint32_t Blend : 1;
      uint32_t Fog : 1;
      uint32_t StencilTest : 1;
      uint32_t ScissorTest : 1;
      uint32_t ColorMaterial : 1;
      uint32_t Normalize : 1;
      uint32_t RescaleNormal : 1;
      uint32_t PolygonOffsetFill : 1;
      uint32_t TwoSidedLighting : 1;
      uint32_t LogicOp : 1;
      uint32_t PointSmooth : 1;
      uint32_t PointSprite : 1;
      uint32_t LineSmooth : 1;
      uint32_t MultiSample : 1;
      uint32_t SampleAlphaToCoverage : 1;
      uint32_t SampleAlphaToOne : 1;
      uint32_t SampleCoverage : 1;
      uint32_t Dither : 1;
    };
  };

  union {
    uint32_t Flags2;
    struct {
      uint8_t Texture2D;
      uint8_t ClipPlanes;
      uint8_t Lights;
    };
  };
  DISABLE_WARNING_POP

  GLCAPS() : Flags1(0), Flags2(0) {
    static_assert(sizeof(GLCAPS) == sizeof(Flags1) + sizeof(Flags2));
  }
};

struct RASTERSTATES {
  __enum_bitfield_begin __enum_bitfield(0, 1, DEPTHFUNC, 3),
      __enum_bitfield(1, 2, BLENDSRC, 4), __enum_bitfield(2, 3, BLENDDST, 4),
      __enum_bitfield(3, 4, ALPHAFUNC, 3),
      __enum_bitfield(4, 5, STENCILFUNC, 3),
      __enum_bitfield(5, 6, STENCILFAIL, 3),
      __enum_bitfield(6, 7, STENCILZFAIL, 3),
      __enum_bitfield(7, 8, STENCILPASS, 3),
      __enum_bitfield(8, 9, LOGICFUNC, 4),
      __enum_bitfield_end DISABLE_WARNING_PUSH DISABLE_WARNING_ANONYMOUS_STRUCT
      union {
    uint32_t Value;
    struct {
      uint32_t DepthFunc : 3;
      uint32_t BlendSrc : 4;
      uint32_t BlendDst : 4;
      uint32_t AlphaFunc : 3;
      uint32_t StencilFunc : 3;
      uint32_t StencilFail : 3;
      uint32_t StencilZFail : 3;
      uint32_t StencilZPass : 3;
      uint32_t LogicFunc : 4;
    };
  };
  DISABLE_WARNING_POP

  RASTERSTATES() : Value(0) {
    static_assert(sizeof(RASTERSTATES) == sizeof(Value));
  }

  RASTERSTATES(uint32_t value) : Value(value) {
    static_assert(sizeof(RASTERSTATES) == sizeof(Value));
  }

  bool operator==(const RASTERSTATES &rhs) const {
    return (this->Value == rhs.Value);
  }
};

struct RASTERFLAGS {
  __enum_bitfield_begin __enum_bitfield(0, 1, DEPTHTEST, 1),
      __enum_bitfield(1, 2, DEPTHWRITE, 1), __enum_bitfield(2, 3, COLOR, 1),
      __enum_bitfield(3, 4, PERSPECTIVE, 1),
      __enum_bitfield(4, 5, NUMTEXTURES, MAX_TEXTURES_MASK),
      __enum_bitfield(5, 6, TEXTUREMIPS, MAX_TEXTURES),
      __enum_bitfield(6, 7, BLEND, 1), __enum_bitfield(7, 8, ALPHATEST, 1),
      __enum_bitfield(8, 9, FOG, 1), __enum_bitfield(9, 10, STENCILTEST, 1),
      __enum_bitfield(10, 11, STENCILWRITE, 1),
      __enum_bitfield(11, 12, LOGICOP, 1),
      __enum_bitfield(12, 13, COLORWRITEMASK, 1),
      __enum_bitfield(13, 14, COLORFORMAT, 4),
      __enum_bitfield(14, 15, DEPTHSTENCILFORMAT, 4),
      __enum_bitfield(15, 16, INTERPOLATEDEPTH, 1),
      __enum_bitfield(16, 17, INTERPOLATECOLOR, 1),
      __enum_bitfield(17, 18, INTERPOLATEALPHA, 1),
      __enum_bitfield(18, 19, INTERPOLATEMIPS, MAX_TEXTURES),
      __enum_bitfield(19, 20, INTERPOLATEFOG, 1),
      __enum_bitfield_end

      DISABLE_WARNING_PUSH DISABLE_WARNING_ANONYMOUS_STRUCT union {
    uint32_t Value;
    struct {
      uint32_t DepthTest : 1;
      uint32_t DepthWrite : 1;
      uint32_t Color : 1;
      uint32_t Perspective : 1;
      uint32_t NumTextures : MAX_TEXTURES_MASK;
      uint32_t TextureMips : MAX_TEXTURES;
      uint32_t Blend : 1;
      uint32_t AlphaTest : 1;
      uint32_t Fog : 1;
      uint32_t StencilTest : 1;
      uint32_t StencilWrite : 1;
      uint32_t LogicOp : 1;
      uint32_t ColorWriteMask : 1;
      uint32_t ColorFormat : 4;
      uint32_t DepthStencilFormat : 4;
      uint32_t InterpolateDepth : 1;
      uint32_t InterpolateColor : 1;
      uint32_t InterpolateAlpha : 1;
      uint32_t InterpolateMips : MAX_TEXTURES;
      uint32_t InterpolateFog : 1;
    };
  };
  DISABLE_WARNING_POP

  RASTERFLAGS() : Value(0) {
    static_assert(sizeof(RASTERFLAGS) == sizeof(Value));
  }

  RASTERFLAGS(uint32_t value) : Value(value) {
    static_assert(sizeof(RASTERFLAGS) == sizeof(Value));
  }

  bool operator==(const RASTERFLAGS &rhs) const {
    return (this->Value == rhs.Value);
  }

  void DebugPrint() const {
    DbgPrintf(3, "Z%d C%d A%d P%d T%d M%d B%d A%d F%d",
              this->DepthTest + this->InterpolateDepth,
              this->Color + this->InterpolateColor,
              this->Color + this->InterpolateAlpha, this->Perspective,
              this->NumTextures,
              (this->TextureMips << 2) + this->InterpolateMips, this->Blend,
              this->AlphaTest, this->Fog + this->InterpolateFog);
  }
};

struct TEXTURESTATES {
  __enum_bitfield_begin __enum_bitfield(0, 1, MIPFILTER, 2),
      __enum_bitfield(1, 2, MINFILTER, 2), __enum_bitfield(2, 3, MAGFILTER, 2),
      __enum_bitfield(3, 4, ADDRESSS, 2), __enum_bitfield(4, 5, ADDRESST, 2),
      __enum_bitfield(5, 6, ENVMODE, 8), __enum_bitfield(6, 7, FORMAT, 8),
      __enum_bitfield_end DISABLE_WARNING_PUSH DISABLE_WARNING_ANONYMOUS_STRUCT
      union {
    uint32_t Value;
    struct {
      uint32_t MipFilter : 2;
      uint32_t MinFilter : 2;
      uint32_t MagFilter : 2;
      uint32_t AddressU : 2;
      uint32_t AddressV : 2;
      uint32_t EnvMode : 8;
      uint32_t Format : 8;
    };
  };
  DISABLE_WARNING_POP

  TEXTURESTATES() : Value(0) {
    static_assert(sizeof(TEXTURESTATES) == sizeof(Value));
  }

  TEXTURESTATES(uint32_t value) { this->Value = value; }

  bool operator==(const TEXTURESTATES &rhs) const {
    return (this->Value == rhs.Value);
  }
};

struct RASTERID {
  RASTERFLAGS Flags;
  RASTERSTATES States;

  TEXTURESTATES Textures[MAX_TEXTURES];

  RASTERID() {}

  RASTERID(uint32_t flags, uint32_t states, uint32_t t0, uint32_t t1)
      : Flags(flags), States(states), Textures{t0, t1} {}

  bool operator==(const RASTERID &rhs) const {
    return (this->Flags == rhs.Flags) && (this->States == rhs.States) &&
           (this->Textures[0] == rhs.Textures[0]) &&
           (this->Textures[1] == rhs.Textures[1]);
  }

  operator uint32_t() const { return this->Flags.Value; }
};

struct TNLFLAGS {
  __enum_bitfield_begin __enum_bitfield(0, 1, USERCLIPPLANES, 1),
      __enum_bitfield(1, 2, EYESPACE, 1), __enum_bitfield(2, 3, EYESPACEZ, 1),
      __enum_bitfield(3, 4, COLOR, 1), __enum_bitfield(4, 5, NORMALIZE, 1),
      __enum_bitfield(5, 6, TEXCOORDS, MAX_TEXTURES_MASK),
      __enum_bitfield(6, 7, FOG, 1), __enum_bitfield(7, 8, POINTSIZE, 1),
      __enum_bitfield(8, 9, POINTATTENUATE, 1),
      __enum_bitfield_end DISABLE_WARNING_PUSH DISABLE_WARNING_ANONYMOUS_STRUCT
      union {
    uint32_t Value;
    struct {
      uint32_t UserClipPlanes : 1;
      uint32_t EyeSpace : 1;
      uint32_t EyeSpaceZ : 1;
      uint32_t Color : 1;
      uint32_t Normalize : 1;
      uint32_t TexCoords : MAX_TEXTURES_MASK;
      uint32_t Fog : 1;
      uint32_t PointSize : 1;
      uint32_t PointSizeQAttn : 1;
    };
  };
  DISABLE_WARNING_POP

  TNLFLAGS() : Value(0) { static_assert(sizeof(TNLFLAGS) == sizeof(Value)); }
};

struct LIGHTFLAGS {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint32_t Value;
    struct {
      uint32_t DirectionalLight : 1;
      uint32_t Attenuation : 1;
      uint32_t SpotLight : 1;
    };
  };
  DISABLE_WARNING_POP

  LIGHTFLAGS() : Value(0) {
    static_assert(sizeof(LIGHTFLAGS) == sizeof(Value));
  }
};

struct ProfileCounter {
  uint32_t Invocations;
  uint32_t DrawnPixels;
  float RenderTime;

  ProfileCounter() : Invocations(0), DrawnPixels(0), RenderTime(0.0f) {}

  bool operator<(const ProfileCounter &rhs) const {
    return (this->RenderTime < rhs.RenderTime);
  }
};
