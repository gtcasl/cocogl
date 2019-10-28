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


class CBlitTable {
public:
  typedef GLenum (*PFN_COPY)(const GLSurfaceDesc &dstDesc, unsigned dstOffsetX,
                             unsigned dstOffsetY, unsigned copyWidth,
                             unsigned copyHeight, const GLSurfaceDesc &srcDesc,
                             unsigned srcOffsetX, unsigned srcOffsetY);

  CBlitTable() {
    for (unsigned s = 0; s < FORMAT_COLOR_SIZE_; ++s) {
      for (unsigned d = 0; d < FORMAT_COLOR_SIZE_; ++d) {
        m_copyFuncs[s][d] = TCopyNC;
      }
    }

    for (unsigned s = 0; s < FORMAT_COLOR_SIZE_; ++s) {
      switch (s) {
      case FORMAT_A8:
      case FORMAT_L8:
        m_copyFuncs[s][s] = TCopyFast<uint8_t>;
        break;

      case FORMAT_A8L8:
        m_copyFuncs[FORMAT_A8L8][FORMAT_A8] = TCopy<FORMAT_A8L8, FORMAT_A8>;
        m_copyFuncs[FORMAT_A8L8][FORMAT_A8L8] = TCopyFast<uint16_t>;
        break;

      case FORMAT_R5G6B5:
        m_copyFuncs[FORMAT_R5G6B5][FORMAT_L8] = TCopy<FORMAT_R5G6B5, FORMAT_L8>;
        m_copyFuncs[FORMAT_R5G6B5][FORMAT_R5G6B5] = TCopyFast<uint16_t>;
        m_copyFuncs[FORMAT_R5G6B5][FORMAT_R8G8B8] =
            TCopy<FORMAT_R5G6B5, FORMAT_R8G8B8>;
        m_copyFuncs[FORMAT_R5G6B5][FORMAT_B8G8R8] =
            TCopy<FORMAT_R5G6B5, FORMAT_B8G8R8>;
        m_copyFuncs[FORMAT_R5G6B5][FORMAT_A8B8G8R8] =
            TCopy<FORMAT_R5G6B5, FORMAT_A8B8G8R8>;
        m_copyFuncs[FORMAT_R5G6B5][FORMAT_A8R8G8B8] =
            TCopy<FORMAT_R5G6B5, FORMAT_A8R8G8B8>;
        break;

      case FORMAT_A1R5G5B5:
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_A8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_A8>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_L8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_L8>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_A8L8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_A8L8>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_R8G8B8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_R8G8B8>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_A8R8G8B8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_A8R8G8B8>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_R5G5B5A1] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_R5G5B5A1>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_R4G4B4A4] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_R4G4B4A4>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_B8G8R8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_B8G8R8>;
        m_copyFuncs[FORMAT_A1R5G5B5][FORMAT_A8B8G8R8] =
            TCopy<FORMAT_A1R5G5B5, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_A4R4G4B4:
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_A8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_A8>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_L8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_L8>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_A8L8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_A8L8>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_R8G8B8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_R8G8B8>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_A8R8G8B8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_A8R8G8B8>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_R5G5B5A1] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_R5G5B5A1>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_R4G4B4A4] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_R4G4B4A4>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_B8G8R8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_B8G8R8>;
        m_copyFuncs[FORMAT_A4R4G4B4][FORMAT_A8B8G8R8] =
            TCopy<FORMAT_A4R4G4B4, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_R8G8B8:
        m_copyFuncs[FORMAT_R8G8B8][FORMAT_L8] = TCopy<FORMAT_R8G8B8, FORMAT_L8>;
        m_copyFuncs[FORMAT_R8G8B8][FORMAT_R5G6B5] =
            TCopy<FORMAT_R8G8B8, FORMAT_R5G6B5>;
        m_copyFuncs[FORMAT_R8G8B8][FORMAT_R8G8B8] = TCopyFast<uint24>;
        m_copyFuncs[FORMAT_R8G8B8][FORMAT_B8G8R8] =
            TCopy<FORMAT_R8G8B8, FORMAT_B8G8R8>;
        m_copyFuncs[FORMAT_R8G8B8][FORMAT_A8B8G8R8] =
            TCopy<FORMAT_R8G8B8, FORMAT_A8B8G8R8>;
        m_copyFuncs[FORMAT_R8G8B8][FORMAT_A8R8G8B8] =
            TCopy<FORMAT_R8G8B8, FORMAT_A8R8G8B8>;
        break;

      case FORMAT_A8R8G8B8:
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_A8] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_A8>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_L8] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_L8>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_A8L8] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_A8L8>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_R5G6B5] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_R5G6B5>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_R8G8B8] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_R8G8B8>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_A8R8G8B8] = TCopyFast<unsigned>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_R5G5B5A1] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_R5G5B5A1>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_R4G4B4A4] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_R4G4B4A4>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_B8G8R8] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_B8G8R8>;
        m_copyFuncs[FORMAT_A8R8G8B8][FORMAT_A8B8G8R8] =
            TCopy<FORMAT_A8R8G8B8, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_R5G5B5A1:
        m_copyFuncs[FORMAT_R5G5B5A1][FORMAT_A8] =
            TCopy<FORMAT_R5G5B5A1, FORMAT_A8>;
        m_copyFuncs[FORMAT_R5G5B5A1][FORMAT_L8] =
            TCopy<FORMAT_R5G5B5A1, FORMAT_L8>;
        m_copyFuncs[FORMAT_R5G5B5A1][FORMAT_A8L8] =
            TCopy<FORMAT_R5G5B5A1, FORMAT_A8L8>;
        m_copyFuncs[FORMAT_R5G5B5A1][FORMAT_RGB] =
            TCopy<FORMAT_R5G5B5A1, FORMAT_RGB>;
        m_copyFuncs[FORMAT_R5G5B5A1][FORMAT_ARGB] =
            TCopy<FORMAT_R5G5B5A1, FORMAT_ARGB>;
        break;

      case FORMAT_R4G4B4A4:
        m_copyFuncs[FORMAT_R4G4B4A4][FORMAT_A8] =
            TCopy<FORMAT_R4G4B4A4, FORMAT_A8>;
        m_copyFuncs[FORMAT_R4G4B4A4][FORMAT_L8] =
            TCopy<FORMAT_R4G4B4A4, FORMAT_L8>;
        m_copyFuncs[FORMAT_R4G4B4A4][FORMAT_A8L8] =
            TCopy<FORMAT_R4G4B4A4, FORMAT_A8L8>;
        m_copyFuncs[FORMAT_R4G4B4A4][FORMAT_RGB] =
            TCopy<FORMAT_R4G4B4A4, FORMAT_RGB>;
        m_copyFuncs[FORMAT_R4G4B4A4][FORMAT_ARGB] =
            TCopy<FORMAT_R4G4B4A4, FORMAT_ARGB>;
        break;

      case FORMAT_B8G8R8:
        m_copyFuncs[FORMAT_B8G8R8][FORMAT_L8] = TCopy<FORMAT_B8G8R8, FORMAT_L8>;
        m_copyFuncs[FORMAT_B8G8R8][FORMAT_RGB] =
            TCopy<FORMAT_B8G8R8, FORMAT_RGB>;
        break;

      case FORMAT_A8B8G8R8:
        m_copyFuncs[FORMAT_A8B8G8R8][FORMAT_A8] =
            TCopy<FORMAT_A8B8G8R8, FORMAT_A8>;
        m_copyFuncs[FORMAT_A8B8G8R8][FORMAT_L8] =
            TCopy<FORMAT_A8B8G8R8, FORMAT_L8>;
        m_copyFuncs[FORMAT_A8B8G8R8][FORMAT_A8L8] =
            TCopy<FORMAT_A8B8G8R8, FORMAT_A8L8>;
        m_copyFuncs[FORMAT_A8B8G8R8][FORMAT_RGB] =
            TCopy<FORMAT_A8B8G8R8, FORMAT_RGB>;
        m_copyFuncs[FORMAT_A8B8G8R8][FORMAT_ARGB] =
            TCopy<FORMAT_A8B8G8R8, FORMAT_ARGB>;
        break;
      }
    }
  }

  PFN_COPY Get(unsigned srcFormat, unsigned dstFormat) const {
    ASSERT(srcFormat < FORMAT_COLOR_SIZE_);
    ASSERT(dstFormat < FORMAT_COLOR_SIZE_);
    return m_copyFuncs[srcFormat][dstFormat];
  }

private:
  template <ePixelFormat SrcFormat, ePixelFormat DstFormat>
  static GLenum TCopy(const GLSurfaceDesc &dstDesc, unsigned dstOffsetX,
                      unsigned dstOffsetY, unsigned copyWidth,
                      unsigned copyHeight, const GLSurfaceDesc &srcDesc,
                      unsigned srcOffsetX, unsigned srcOffsetY) {
    const unsigned srcBPP = TFormatInfo<SrcFormat>::CBSIZE;
    const unsigned dstBPP = TFormatInfo<DstFormat>::CBSIZE;
    const unsigned srcNextLine = srcDesc.Pitch;
    const unsigned dstNextLine = dstDesc.Pitch;

    const uint8_t *pbSrc =
        srcDesc.pBits + srcOffsetX * srcBPP + srcOffsetY * srcDesc.Pitch;
    uint8_t *pbDst =
        dstDesc.pBits + dstOffsetX * dstBPP + dstOffsetY * dstDesc.Pitch;

    Color4 tmp;

    while (copyHeight--) {
      const typename TFormatInfo<SrcFormat>::TYPE *pSrc =
          reinterpret_cast<const typename TFormatInfo<SrcFormat>::TYPE *>(pbSrc);

      for (typename TFormatInfo<DstFormat>::TYPE *
               pDst = reinterpret_cast<typename TFormatInfo<DstFormat>::TYPE *>(pbDst),
              *const pEnd = pDst + copyWidth;
           pDst != pEnd; ++pDst, ++pSrc) {
        Format::TConvertFrom<SrcFormat, true>(&tmp, pSrc);
        Format::TConvertTo<DstFormat>(pDst, tmp);
      }

      pbSrc += srcNextLine;
      pbDst += dstNextLine;
    }

    return GL_NO_ERROR;
  }

  template <class Type>
  static GLenum TCopyFast(const GLSurfaceDesc &dstDesc, unsigned dstOffsetX,
                          unsigned dstOffsetY, unsigned copyWidth,
                          unsigned copyHeight, const GLSurfaceDesc &srcDesc,
                          unsigned srcOffsetX, unsigned srcOffsetY) {
    const unsigned nBPP = sizeof(Type);
    const unsigned srcNextLine = srcDesc.Pitch;
    const unsigned dstNextLine = dstDesc.Pitch;

    const uint8_t *pbSrc =
        srcDesc.pBits + srcOffsetX * nBPP + srcOffsetY * srcDesc.Pitch;
    uint8_t *pbDst =
        dstDesc.pBits + dstOffsetX * nBPP + dstOffsetY * dstDesc.Pitch;

    while (copyHeight--) {
      const Type *pSrc = reinterpret_cast<const Type *>(pbSrc);
      for (Type *pDst = reinterpret_cast<Type *>(pbDst),
                *const pEnd = pDst + copyWidth;
           pDst != pEnd; ++pDst, ++pSrc) {
        *pDst = *pSrc;
      }

      pbSrc += srcNextLine;
      pbDst += dstNextLine;
    }

    return GL_NO_ERROR;
  }

  static GLenum TCopyNC(const GLSurfaceDesc & /*dstDesc*/,
                        unsigned /*dstOffsetX*/, unsigned /*dstOffsetY*/,
                        unsigned /*copyWidth*/, unsigned /*copyHeight*/,
                        const GLSurfaceDesc & /*srcDesc*/,
                        unsigned /*srcOffsetX*/, unsigned /*srcOffsetY*/
                        ) {
    __glLogError(_T("CSurface2D::Copy() failed, the source and destination ")
                 _T("formats are not compatible."));
    return GL_INVALID_OPERATION;
  }

  PFN_COPY m_copyFuncs[FORMAT_COLOR_SIZE_][FORMAT_COLOR_SIZE_];
};


GLenum CopyBuffers(const GLSurfaceDesc &dstDesc, unsigned dstOffsetX,
                   unsigned dstOffsetY, unsigned copyWidth, unsigned copyHeight,
                   const GLSurfaceDesc &srcDesc, unsigned srcOffsetX,
                   unsigned srcOffsetY) {
  static const CBlitTable s_blitTable;

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

  return s_blitTable.Get(srcDesc.Format, dstDesc.Format)(
      dstDesc, dstOffsetX, dstOffsetY, copyWidth, copyHeight, srcDesc,
      srcOffsetX, srcOffsetY);
}

//////////////////////////////////////////////////////////////////////////////


unsigned GLSizeOf(GLenum type) {
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
    return (eVertexFormat)(VERTEX_BYTE + size - 1);

  case GL_UNSIGNED_BYTE:
    return VERTEX_RGBA;

  case GL_SHORT:
    return (eVertexFormat)(VERTEX_SHORT + size - 1);

  case GL_FLOAT:
    return (eVertexFormat)(VERTEX_FLOAT + size - 1);

  case GL_FIXED:
    return (eVertexFormat)(VERTEX_FIXED + size - 1);
  }
}


unsigned VertexDataSize(eVertexFormat format) {
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


GLenum ToPixelFormat(ePixelFormat *pOut, unsigned *pBPP, GLenum format,
                     GLenum type) {
  ASSERT(pOut && pBPP);

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
    __glLogError(_T("ToPixelFormat() failed, invalid type parameter: %d.\r\n"),
                 type);
    return GL_INVALID_ENUM;
  }

  return GL_NO_ERROR;
}


GLenum ToPixelFormat(ePixelFormat *pOut, GLint format) {
  ASSERT(pOut);

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
    __glLogError(
        _T("ToPixelFormat() failed, invalid format parameter: %d.\r\n"),
        format);
    return GL_INVALID_VALUE;
  }

  return GL_NO_ERROR;
}


eCompare Reverse(eCompare compare) {
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


unsigned CompareFuncFromEnum(GLenum func) { return (func - GL_NEVER); }


GLenum EnumFromCompareFunc(unsigned func) { return (GL_NEVER + func); }


unsigned StencilOpFromEnum(GLenum op) {
  if (GL_ZERO == op) {
    return STENCIL_ZERO;
  } else if (GL_INVERT == op) {
    return STENCIL_INVERT;
  } else {
    return (op - GL_KEEP);
  }
}


GLenum EnumFromStencilOp(unsigned op) {
  if (STENCIL_ZERO == op) {
    return GL_ZERO;
  } else if (STENCIL_INVERT == op) {
    return GL_INVERT;
  } else {
    return (GL_KEEP + op);
  }
}


unsigned BlendFuncFromEnum(GLenum func) {
  switch (func) {
  case GL_ZERO:
  case GL_ONE:
    return BLEND_ZERO + (func - GL_ZERO);

  default:
    return BLEND_SRC_COLOR + (func - GL_SRC_COLOR);
  }
}


GLenum EnumFromBlendFunc(unsigned func) {
  switch (func) {
  case BLEND_ZERO:
  case BLEND_ONE:
    return (GL_ZERO + func);

  default:
    return GL_SRC_ALPHA + (func - BLEND_SRC_ALPHA);
  }
}


unsigned LogicOpFromEnum(GLenum op) { return (op - GL_CLEAR); }


GLenum EnumFromLogicOp(unsigned op) { return (GL_CLEAR + op); }


unsigned FogModeFromEnum(GLenum mode) {
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


GLenum EnumFromFogMode(unsigned mode) {
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


unsigned ShadeModelFromEnum(GLenum mode) {
  switch (mode) {
  case GL_FLAT:
    return SHADE_FLAT;

  case GL_SMOOTH:
  default:
    return SHADE_SMOOTH;
    break;
  }
}


GLenum EnumFromShadeModel(unsigned mode) {
  switch (mode) {
  case SHADE_FLAT:
    return GL_FLAT;

  case SHADE_SMOOTH:
  default:
    return GL_SMOOTH;
    break;
  }
}


unsigned CullFaceFromEnum(GLenum mode) {
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


GLenum EnumFromCullFace(unsigned mode) {
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


unsigned FrontFaceFromEnum(GLenum mode) {
  switch (mode) {
  case GL_CW:
    return CULL_CW;

  case GL_CCW:
  default:
    return CULL_CCW;
  }
}


GLenum EnumFromFrontFace(unsigned mode) {
  switch (mode) {
  case CULL_CW:
    return GL_CW;

  case CULL_CCW:
  default:
    return GL_CCW;
  }
}


unsigned HintFromEnum(GLenum mode) { return (mode - GL_DONT_CARE); }


GLenum EnumFromHint(unsigned mode) { return (GL_DONT_CARE + mode); }


unsigned TexFilterFromEnum(GLenum param) {
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


GLenum EnumFromTexFilter(unsigned param) {
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


unsigned TexAddressFromEnum(GLenum param) {
  switch (param) {
  case GL_CLAMP_TO_EDGE:
    return ADDRESS_CLAMP;

  case GL_REPEAT:
  default:
    return ADDRESS_WRAP;
  }
}


GLenum EnumFromTexAddress(unsigned param) {
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


GLenum EnumFromTexEnv(unsigned param) {
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
