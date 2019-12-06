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

class BlitTable {
public:
  typedef GLenum (*PfnCopy)(const GLSurfaceDesc &dstDesc, uint32_t dstOffsetX,
                            uint32_t dstOffsetY, uint32_t copyWidth,
                            uint32_t copyHeight, const GLSurfaceDesc &srcDesc,
                            uint32_t srcOffsetX, uint32_t srcOffsetY);

  BlitTable() {
    for (uint32_t s = 0; s < FORMAT_COLOR_SIZE_; ++s) {
      for (uint32_t d = 0; d < FORMAT_COLOR_SIZE_; ++d) {
        copyFuncs_[s][d] = CopyNC;
      }
    }

    for (uint32_t s = 0; s < FORMAT_COLOR_SIZE_; ++s) {
      switch (s) {
      case FORMAT_A8:
      case FORMAT_L8:
        copyFuncs_[s][s] = CopyFast<uint8_t>;
        break;

      case FORMAT_A8L8:
        copyFuncs_[FORMAT_A8L8][FORMAT_A8] = Copy<FORMAT_A8L8, FORMAT_A8>;
        copyFuncs_[FORMAT_A8L8][FORMAT_A8L8] = CopyFast<uint16_t>;
        break;

      case FORMAT_R5G6B5:
        copyFuncs_[FORMAT_R5G6B5][FORMAT_L8] = Copy<FORMAT_R5G6B5, FORMAT_L8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_R5G6B5] = CopyFast<uint16_t>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_R8G8B8] =
            Copy<FORMAT_R5G6B5, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_B8G8R8] =
            Copy<FORMAT_R5G6B5, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_A8B8G8R8] =
            Copy<FORMAT_R5G6B5, FORMAT_A8B8G8R8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_A8R8G8B8] =
            Copy<FORMAT_R5G6B5, FORMAT_A8R8G8B8>;
        break;

      case FORMAT_A1R5G5B5:
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_A8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_L8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_L8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8L8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_R8G8B8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8R8G8B8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_A8R8G8B8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_R5G5B5A1] =
            Copy<FORMAT_A1R5G5B5, FORMAT_R5G5B5A1>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_R4G4B4A4] =
            Copy<FORMAT_A1R5G5B5, FORMAT_R4G4B4A4>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_B8G8R8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8B8G8R8] =
            Copy<FORMAT_A1R5G5B5, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_A4R4G4B4:
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_A8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_L8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_L8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8L8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_R8G8B8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8R8G8B8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_A8R8G8B8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_R5G5B5A1] =
            Copy<FORMAT_A4R4G4B4, FORMAT_R5G5B5A1>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_R4G4B4A4] =
            Copy<FORMAT_A4R4G4B4, FORMAT_R4G4B4A4>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_B8G8R8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8B8G8R8] =
            Copy<FORMAT_A4R4G4B4, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_R8G8B8:
        copyFuncs_[FORMAT_R8G8B8][FORMAT_L8] = Copy<FORMAT_R8G8B8, FORMAT_L8>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_R5G6B5] =
            Copy<FORMAT_R8G8B8, FORMAT_R5G6B5>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_R8G8B8] = CopyFast<uint24_t>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_B8G8R8] =
            Copy<FORMAT_R8G8B8, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_A8B8G8R8] =
            Copy<FORMAT_R8G8B8, FORMAT_A8B8G8R8>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_A8R8G8B8] =
            Copy<FORMAT_R8G8B8, FORMAT_A8R8G8B8>;
        break;

      case FORMAT_A8R8G8B8:
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8] =
            Copy<FORMAT_A8R8G8B8, FORMAT_A8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_L8] =
            Copy<FORMAT_A8R8G8B8, FORMAT_L8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8L8] =
            Copy<FORMAT_A8R8G8B8, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R5G6B5] =
            Copy<FORMAT_A8R8G8B8, FORMAT_R5G6B5>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R8G8B8] =
            Copy<FORMAT_A8R8G8B8, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8R8G8B8] = CopyFast<uint32_t>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R5G5B5A1] =
            Copy<FORMAT_A8R8G8B8, FORMAT_R5G5B5A1>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R4G4B4A4] =
            Copy<FORMAT_A8R8G8B8, FORMAT_R4G4B4A4>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_B8G8R8] =
            Copy<FORMAT_A8R8G8B8, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8B8G8R8] =
            Copy<FORMAT_A8R8G8B8, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_R5G5B5A1:
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_A8] =
            Copy<FORMAT_R5G5B5A1, FORMAT_A8>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_L8] =
            Copy<FORMAT_R5G5B5A1, FORMAT_L8>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_A8L8] =
            Copy<FORMAT_R5G5B5A1, FORMAT_A8L8>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_RGB] =
            Copy<FORMAT_R5G5B5A1, FORMAT_RGB>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_ARGB] =
            Copy<FORMAT_R5G5B5A1, FORMAT_ARGB>;
        break;

      case FORMAT_R4G4B4A4:
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_A8] =
            Copy<FORMAT_R4G4B4A4, FORMAT_A8>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_L8] =
            Copy<FORMAT_R4G4B4A4, FORMAT_L8>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_A8L8] =
            Copy<FORMAT_R4G4B4A4, FORMAT_A8L8>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_RGB] =
            Copy<FORMAT_R4G4B4A4, FORMAT_RGB>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_ARGB] =
            Copy<FORMAT_R4G4B4A4, FORMAT_ARGB>;
        break;

      case FORMAT_B8G8R8:
        copyFuncs_[FORMAT_B8G8R8][FORMAT_L8] = Copy<FORMAT_B8G8R8, FORMAT_L8>;
        copyFuncs_[FORMAT_B8G8R8][FORMAT_RGB] = Copy<FORMAT_B8G8R8, FORMAT_RGB>;
        break;

      case FORMAT_A8B8G8R8:
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_A8] =
            Copy<FORMAT_A8B8G8R8, FORMAT_A8>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_L8] =
            Copy<FORMAT_A8B8G8R8, FORMAT_L8>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_A8L8] =
            Copy<FORMAT_A8B8G8R8, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_RGB] =
            Copy<FORMAT_A8B8G8R8, FORMAT_RGB>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_ARGB] =
            Copy<FORMAT_A8B8G8R8, FORMAT_ARGB>;
        break;
      }
    }
  }

  PfnCopy get(uint32_t srcFormat, uint32_t dstFormat) const {
    assert(srcFormat < FORMAT_COLOR_SIZE_);
    assert(dstFormat < FORMAT_COLOR_SIZE_);
    return copyFuncs_[srcFormat][dstFormat];
  }

private:
  template <ePixelFormat SrcFormat, ePixelFormat DstFormat>
  static GLenum Copy(const GLSurfaceDesc &dstDesc, uint32_t dstOffsetX,
                     uint32_t dstOffsetY, uint32_t copyWidth,
                     uint32_t copyHeight, const GLSurfaceDesc &srcDesc,
                     uint32_t srcOffsetX, uint32_t srcOffsetY) {
    uint32_t srcBPP = TFormatInfo<SrcFormat>::CBSIZE;
    uint32_t dstBPP = TFormatInfo<DstFormat>::CBSIZE;
    int32_t srcNextLine = srcDesc.Pitch;
    int32_t dstNextLine = dstDesc.Pitch;

    const uint8_t *pbSrc =
        srcDesc.pBits + srcOffsetX * srcBPP + srcOffsetY * srcDesc.Pitch;
    uint8_t *pbDst =
        dstDesc.pBits + dstOffsetX * dstBPP + dstOffsetY * dstDesc.Pitch;

    Color4 tmp;

    while (copyHeight--) {
      auto pSrc =
          reinterpret_cast<const typename TFormatInfo<SrcFormat>::TYPE *>(
              pbSrc);
      for (auto *
               pDst = reinterpret_cast<typename TFormatInfo<DstFormat>::TYPE *>(
                  pbDst),
              *const pEnd = pDst + copyWidth;
           pDst != pEnd; ++pDst, ++pSrc) {
        Format::ConvertFrom<SrcFormat, true>(&tmp, pSrc);
        Format::ConvertTo<DstFormat>(pDst, tmp);
      }

      pbSrc += srcNextLine;
      pbDst += dstNextLine;
    }

    return GL_NO_ERROR;
  }

  template <typename Type>
  static GLenum CopyFast(const GLSurfaceDesc &dstDesc, uint32_t dstOffsetX,
                         uint32_t dstOffsetY, uint32_t copyWidth,
                         uint32_t copyHeight, const GLSurfaceDesc &srcDesc,
                         uint32_t srcOffsetX, uint32_t srcOffsetY) {
    uint32_t nBPP = sizeof(Type);
    int32_t srcNextLine = srcDesc.Pitch;
    int32_t dstNextLine = dstDesc.Pitch;

    const uint8_t *pbSrc =
        srcDesc.pBits + srcOffsetX * nBPP + srcOffsetY * srcDesc.Pitch;
    uint8_t *pbDst =
        dstDesc.pBits + dstOffsetX * nBPP + dstOffsetY * dstDesc.Pitch;

    while (copyHeight--) {
      auto pSrc = reinterpret_cast<const Type *>(pbSrc);
      for (auto *pDst = reinterpret_cast<Type *>(pbDst), *const pEnd =
                                                             pDst + copyWidth;
           pDst != pEnd; ++pDst, ++pSrc) {
        *pDst = *pSrc;
      }

      pbSrc += srcNextLine;
      pbDst += dstNextLine;
    }

    return GL_NO_ERROR;
  }

  static GLenum CopyNC(const GLSurfaceDesc & /*dstDesc*/,
                       uint32_t /*dstOffsetX*/, uint32_t /*dstOffsetY*/,
                       uint32_t /*copyWidth*/, uint32_t /*copyHeight*/,
                       const GLSurfaceDesc & /*srcDesc*/,
                       uint32_t /*srcOffsetX*/, uint32_t /*srcOffsetY*/
  ) {
    __glLogError("Copy() failed, the source and destination "
                 "formats are not compatible.");
    return GL_INVALID_OPERATION;
  }

  PfnCopy copyFuncs_[FORMAT_COLOR_SIZE_][FORMAT_COLOR_SIZE_];
};

GLenum CopyBuffers(const GLSurfaceDesc &dstDesc, int32_t dstOffsetX,
                   int32_t dstOffsetY, int32_t copyWidth, int32_t copyHeight,
                   const GLSurfaceDesc &srcDesc, int32_t srcOffsetX,
                   int32_t srcOffsetY) {
  static const BlitTable s_blitTable;

  if ((srcOffsetX >= srcDesc.Width) || (srcOffsetY >= srcDesc.Height) ||
      (dstOffsetX >= dstDesc.Width) || (dstOffsetY >= dstDesc.Height)) {
    return GL_NO_ERROR;
  }

  if (copyWidth > dstDesc.Width) {
    copyWidth = dstDesc.Width;
  }

  if (copyWidth > srcDesc.Width) {
    copyWidth = srcDesc.Width;
  }

  if (copyHeight > dstDesc.Height) {
    copyHeight = dstDesc.Height;
  }

  if (copyHeight > srcDesc.Height) {
    copyHeight = srcDesc.Height;
  }

  return s_blitTable.get(srcDesc.Format, dstDesc.Format)(
      dstDesc, dstOffsetX, dstOffsetY, copyWidth, copyHeight, srcDesc,
      srcOffsetX, srcOffsetY);
}

//////////////////////////////////////////////////////////////////////////////

uint32_t GLSizeOf(GLenum type) {
  switch (type) {
  default:
    __no_default;
  case GL_BYTE:
    return sizeof(char);

  case GL_UNSIGNED_BYTE:
    return sizeof(char);

  case GL_SHORT:
    return sizeof(short);

  case GL_UNSIGNED_SHORT:
    return sizeof(short);

  case GL_FLOAT:
    return sizeof(float);

  case GL_FIXED:
    return sizeof(int);
  }
}

eVertexFormat ToVertexFormat(GLenum type, GLint size) {
  switch (type) {
  default:
    __no_default;
  case GL_BYTE:
    return static_cast<eVertexFormat>(VERTEX_BYTE + size - 1);

  case GL_UNSIGNED_BYTE:
    return VERTEX_RGBA;

  case GL_SHORT:
    return static_cast<eVertexFormat>(VERTEX_SHORT + size - 1);

  case GL_FLOAT:
    return static_cast<eVertexFormat>(VERTEX_FLOAT + size - 1);

  case GL_FIXED:
    return static_cast<eVertexFormat>(VERTEX_FIXED + size - 1);
  }
}

uint32_t VertexDataSize(eVertexFormat format) {
  switch (format) {
  default:
    __no_default;
  case VERTEX_BYTE:
  case VERTEX_BYTE2:
  case VERTEX_BYTE3:
  case VERTEX_BYTE4:
    return sizeof(char) * (format - VERTEX_BYTE + 1);

  case VERTEX_SHORT:
  case VERTEX_SHORT2:
  case VERTEX_SHORT3:
  case VERTEX_SHORT4:
    return sizeof(short) * (format - VERTEX_SHORT + 1);

  case VERTEX_FIXED:
  case VERTEX_FIXED2:
  case VERTEX_FIXED3:
  case VERTEX_FIXED4:
    return sizeof(int) * (format - VERTEX_FIXED + 1);

  case VERTEX_FLOAT:
  case VERTEX_FLOAT2:
  case VERTEX_FLOAT3:
  case VERTEX_FLOAT4:
    return sizeof(float) * (format - VERTEX_FLOAT + 1);

  case VERTEX_RGBA:
    return sizeof(int);
  }
}

GLenum DecodeDataType(eVertexFormat format) {
  switch (format) {
  default:
    __no_default;
  case VERTEX_BYTE:
  case VERTEX_BYTE2:
  case VERTEX_BYTE3:
  case VERTEX_BYTE4:
    return GL_BYTE;

  case VERTEX_SHORT:
  case VERTEX_SHORT2:
  case VERTEX_SHORT3:
  case VERTEX_SHORT4:
    return GL_SHORT;

  case VERTEX_FIXED:
  case VERTEX_FIXED2:
  case VERTEX_FIXED3:
  case VERTEX_FIXED4:
    return GL_FIXED;

  case VERTEX_FLOAT:
  case VERTEX_FLOAT2:
  case VERTEX_FLOAT3:
  case VERTEX_FLOAT4:
    return GL_FLOAT;

  case VERTEX_RGBA:
    return GL_UNSIGNED_BYTE;
  }
}

GLint DecodeDataSize(eVertexFormat format) {
  switch (format) {
  default:
    __no_default;
  case VERTEX_BYTE:
  case VERTEX_BYTE2:
  case VERTEX_BYTE3:
  case VERTEX_BYTE4:
    return (format - VERTEX_BYTE + 1);

  case VERTEX_SHORT:
  case VERTEX_SHORT2:
  case VERTEX_SHORT3:
  case VERTEX_SHORT4:
    return (format - VERTEX_SHORT + 1);

  case VERTEX_FIXED:
  case VERTEX_FIXED2:
  case VERTEX_FIXED3:
  case VERTEX_FIXED4:
    return (format - VERTEX_FIXED + 1);

  case VERTEX_FLOAT:
  case VERTEX_FLOAT2:
  case VERTEX_FLOAT3:
  case VERTEX_FLOAT4:
    return (format - VERTEX_FLOAT + 1);

  case VERTEX_RGBA:
    return 4;
  }
}

GLenum GLERROR_FROM_HRESULT(HRESULT hr) {
  if (SUCCEEDED(hr)) {
    return GL_NO_ERROR;
  } else if (E_INVALIDARG == hr) {
    return GL_INVALID_VALUE;
  } else if (E_OUTOFMEMORY == hr) {
    return GL_OUT_OF_MEMORY;
  }

  return GL_INVALID_OPERATION;
}

GLenum ToPixelFormat(ePixelFormat *pOut, uint32_t *pBPP, GLenum format,
                     GLenum type) {
  assert(pOut && pBPP);

  switch (type) {
  case GL_UNSIGNED_BYTE:
    if (GL_ALPHA == format) {
      *pOut = FORMAT_A8;
      *pBPP = 1;
    } else if (GL_LUMINANCE == format) {
      *pOut = FORMAT_L8;
      *pBPP = 1;
    } else if (GL_LUMINANCE_ALPHA == format) {
      *pOut = FORMAT_A8L8;
      *pBPP = 2;
    } else if (GL_RGB == format) {
      *pOut = FORMAT_B8G8R8;
      *pBPP = 3;
    } else if (GL_RGBA == format) {
      *pOut = FORMAT_A8B8G8R8;
      *pBPP = 4;
    } else {
      return GL_INVALID_OPERATION;
    }

    break;

  case GL_UNSIGNED_SHORT_5_6_5:
    if (GL_RGB == format) {
      *pOut = FORMAT_R5G6B5;
      *pBPP = 2;
    } else {
      return GL_INVALID_OPERATION;
    }

    break;

  case GL_UNSIGNED_SHORT_4_4_4_4:
    if (GL_RGBA == format) {
      *pOut = FORMAT_R4G4B4A4;
      *pBPP = 2;
    } else {
      return GL_INVALID_OPERATION;
    }

    break;

  case GL_UNSIGNED_SHORT_5_5_5_1:
    if (GL_RGBA == format) {
      *pOut = FORMAT_R5G5B5A1;
      *pBPP = 2;
    } else {
      return GL_INVALID_OPERATION;
    }

    break;

  default:
    __glLogError("ToPixelFormat() failed, invalid type parameter: %d.\r\n",
                 type);
    return GL_INVALID_ENUM;
  }

  return GL_NO_ERROR;
}

GLenum ToPixelFormat(ePixelFormat *pOut, GLint format) {
  assert(pOut);

  switch (format) {
  case GL_ALPHA:
    *pOut = FORMAT_A8;
    break;

  case GL_RGB:
    *pOut = FORMAT_RGB;
    break;

  case GL_RGBA:
    *pOut = FORMAT_ARGB;
    break;

  case GL_LUMINANCE:
    *pOut = FORMAT_L8;
    break;

  case GL_LUMINANCE_ALPHA:
    *pOut = FORMAT_A8L8;
    break;

  default:
    __glLogError("ToPixelFormat() failed, invalid format parameter: %d.\r\n",
                 format);
    return GL_INVALID_VALUE;
  }

  return GL_NO_ERROR;
}

ecompare Reverse(ecompare compare) {
  switch (compare) {
  default:
    __no_default;

  case COMPARE_NEVER:
    return COMPARE_ALWAYS;

  case COMPARE_LESS:
    return COMPARE_GEQUAL;

  case COMPARE_EQUAL:
    return COMPARE_NOTEQUAL;

  case COMPARE_LEQUAL:
    return COMPARE_GREATER;

  case COMPARE_GREATER:
    return COMPARE_LEQUAL;

  case COMPARE_NOTEQUAL:
    return COMPARE_EQUAL;

  case COMPARE_GEQUAL:
    return COMPARE_LESS;

  case COMPARE_ALWAYS:
    return COMPARE_NEVER;
  }
}

uint32_t compareFuncFromEnum(GLenum func) { return (func - GL_NEVER); }

GLenum EnumFromcompareFunc(uint32_t func) { return (GL_NEVER + func); }

uint32_t StencilOpFromEnum(GLenum op) {
  if (GL_ZERO == op) {
    return STENCIL_ZERO;
  } else if (GL_INVERT == op) {
    return STENCIL_INVERT;
  } else {
    return (op - GL_KEEP);
  }
}

GLenum EnumFromStencilOp(uint32_t op) {
  if (STENCIL_ZERO == op) {
    return GL_ZERO;
  } else if (STENCIL_INVERT == op) {
    return GL_INVERT;
  } else {
    return (GL_KEEP + op);
  }
}

uint32_t BlendFuncFromEnum(GLenum func) {
  switch (func) {
  case GL_ZERO:
  case GL_ONE:
    return BLEND_ZERO + (func - GL_ZERO);

  default:
    return BLEND_SRC_COLOR + (func - GL_SRC_COLOR);
  }
}

GLenum EnumFromBlendFunc(uint32_t func) {
  switch (func) {
  case BLEND_ZERO:
  case BLEND_ONE:
    return (GL_ZERO + func);

  default:
    return GL_SRC_ALPHA + (func - BLEND_SRC_ALPHA);
  }
}

uint32_t LogicOpFromEnum(GLenum op) { return (op - GL_CLEAR); }

GLenum EnumFromLogicOp(uint32_t op) { return (GL_CLEAR + op); }

uint32_t FogModeFromEnum(GLenum mode) {
  switch (mode) {
  case GL_LINEAR:
    return FogLinear;

  case GL_EXP:
    return FogExp;

  case GL_EXP2:
  default:
    return FogExp2;
  }
}

GLenum EnumFromFogMode(uint32_t mode) {
  switch (mode) {
  case FogLinear:
    return GL_LINEAR;

  case FogExp:
    return GL_EXP;

  case FogExp2:
  default:
    return GL_EXP2;
  }
}

uint32_t ShadeModelFromEnum(GLenum mode) {
  switch (mode) {
  case GL_FLAT:
    return SHADE_FLAT;

  case GL_SMOOTH:
  default:
    return SHADE_SMOOTH;
    break;
  }
}

GLenum EnumFromShadeModel(uint32_t mode) {
  switch (mode) {
  case SHADE_FLAT:
    return GL_FLAT;

  case SHADE_SMOOTH:
  default:
    return GL_SMOOTH;
    break;
  }
}

uint32_t CullFaceFromEnum(GLenum mode) {
  switch (mode) {
  case GL_FRONT:
    return CULL_FRONT;

  case GL_BACK:
    return CULL_BACK;

  case GL_FRONT_AND_BACK:
  default:
    return CULL_FRONT_AND_BACK;
  }
}

GLenum EnumFromCullFace(uint32_t mode) {
  switch (mode) {
  case CULL_FRONT:
    return GL_FRONT;

  case CULL_BACK:
    return GL_BACK;

  case CULL_FRONT_AND_BACK:
  default:
    return GL_FRONT_AND_BACK;
  }
}

uint32_t FrontFaceFromEnum(GLenum mode) {
  switch (mode) {
  case GL_CW:
    return CULL_CW;

  case GL_CCW:
  default:
    return CULL_CCW;
  }
}

GLenum EnumFromFrontFace(uint32_t mode) {
  switch (mode) {
  case CULL_CW:
    return GL_CW;

  case CULL_CCW:
  default:
    return GL_CCW;
  }
}

uint32_t HintFromEnum(GLenum mode) { return (mode - GL_DONT_CARE); }

GLenum EnumFromHint(uint32_t mode) { return (GL_DONT_CARE + mode); }

uint32_t TexFilterFromEnum(GLenum param) {
  switch (param) {
  case GL_NEAREST:
    return FILTER_NEAREST;

  case GL_LINEAR:
    return FILTER_LINEAR;

  case GL_NEAREST_MIPMAP_NEAREST:
    return FILTER_NEAREST_MIPMAP_NEAREST;

  case GL_LINEAR_MIPMAP_NEAREST:
    return FILTER_LINEAR_MIPMAP_NEAREST;

  case GL_NEAREST_MIPMAP_LINEAR:
    return FILTER_NEAREST_MIPMAP_LINEAR;

  case GL_LINEAR_MIPMAP_LINEAR:
  default:
    return FILTER_LINEAR_MIPMAP_LINEAR;
  }
}

GLenum EnumFromTexFilter(uint32_t param) {
  switch (param) {
  case FILTER_NEAREST:
    return GL_NEAREST;

  case FILTER_LINEAR:
    return GL_LINEAR;

  case FILTER_NEAREST_MIPMAP_NEAREST:
    return GL_NEAREST_MIPMAP_NEAREST;

  case FILTER_LINEAR_MIPMAP_NEAREST:
    return GL_LINEAR_MIPMAP_NEAREST;

  case FILTER_NEAREST_MIPMAP_LINEAR:
    return GL_NEAREST_MIPMAP_LINEAR;

  case FILTER_LINEAR_MIPMAP_LINEAR:
  default:
    return GL_LINEAR_MIPMAP_LINEAR;
  }
}

uint32_t TexAddressFromEnum(GLenum param) {
  switch (param) {
  case GL_CLAMP_TO_EDGE:
    return ADDRESS_CLAMP;

  case GL_REPEAT:
  default:
    return ADDRESS_WRAP;
  }
}

GLenum EnumFromTexAddress(uint32_t param) {
  switch (param) {
  case ADDRESS_CLAMP:
    return GL_CLAMP_TO_EDGE;

  case ADDRESS_WRAP:
  default:
    return GL_REPEAT;
  }
}

eEnvMode TexEnvFromEnum(GLenum param) {
  switch (param) {
  case GL_ADD:
    return ENVMODE_ADD;

  case GL_BLEND:
    return ENVMODE_BLEND;

  case GL_REPLACE:
    return ENVMODE_REPLACE;

  case GL_MODULATE:
    return ENVMODE_MODULATE;

  case GL_DECAL:
  default:
    return ENVMODE_DECAL;
  }
}

GLenum EnumFromTexEnv(uint32_t param) {
  switch (param) {
  case ENVMODE_ADD:
    return GL_ADD;

  case ENVMODE_BLEND:
    return GL_BLEND;

  case ENVMODE_REPLACE:
    return GL_REPLACE;

  case ENVMODE_MODULATE:
    return GL_MODULATE;

  case ENVMODE_DECAL:
  default:
    return GL_DECAL;
  }
}