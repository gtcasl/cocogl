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

template <bool DepthTest, unsigned DepthFunc, bool DepthWrite,
          unsigned StencilFunc, unsigned StencilPass, unsigned StencilFail,
          unsigned StencilZFail, bool StencilWrite>
bool TStencilTest(unsigned depthValue, unsigned stencilRef,
                  unsigned stencilMask, unsigned stencilWriteMask,
                  void *pDSBuffer) {
  ASSERT(pDSBuffer);

  const unsigned depthStencilValue = *reinterpret_cast<uint32_t *>(pDSBuffer);
  const unsigned stencilValue = depthStencilValue >> 16;
  const unsigned _depthValue = depthStencilValue & 0xffff;
  const unsigned _stencilValue = stencilValue & stencilMask;
  const unsigned _stencilRef = stencilRef & stencilMask;

  unsigned stencilResult;
  unsigned writeMask = 0;

  if
    constexpr(StencilWrite) { writeMask = stencilWriteMask << 16; }

  bool bStencilTest = TCompare<StencilFunc>(_stencilRef, _stencilValue);
  if (bStencilTest) {
    if (TCompare<(DepthTest ? DepthFunc
                            : static_cast<uint32_t>(COMPARE_ALWAYS))>(
            depthValue, _depthValue)) {
      stencilResult = TStencilOp<StencilPass>(stencilValue, stencilRef);

      if
        constexpr(DepthTest && DepthWrite) { writeMask |= 0xffff; }
    } else {
      stencilResult = TStencilOp<StencilZFail>(stencilValue, stencilRef);
      bStencilTest = false;
    }
  } else {
    stencilResult = TStencilOp<StencilFail>(stencilValue, stencilRef);
  }

  if
    constexpr(DepthWrite || StencilWrite) {
      const unsigned value = ((stencilResult << 16) | depthValue);
      *reinterpret_cast<uint32_t *>(pDSBuffer) =
          (depthStencilValue & ~writeMask) | (value & writeMask);
    }

  return bStencilTest;
}

//////////////////////////////////////////////////////////////////////////////

template <unsigned MipFilter, unsigned MinFilter, unsigned MagFilter,
          unsigned Format, unsigned AddressU, unsigned AddressV>
inline unsigned TGetSamplerColor(const SurfaceDesc &surface, fixedRX fU,
                                 fixedRX fV) {
  return TGetMinFilterN<MagFilter, Format, AddressU, AddressV>(surface, fU, fV);
}

template <unsigned MipFilter, unsigned MinFilter, unsigned MagFilter,
          unsigned Format, unsigned AddressU, unsigned AddressV>
unsigned TGetSamplerColor(const Sampler &sampler, fixedRX fU, fixedRX fV,
                          fixedRX fM) {
  return TGetMipFilterN<MipFilter, MinFilter, MagFilter, Format, AddressU,
                        AddressV>(sampler, fU, fV, fM);
}

//////////////////////////////////////////////////////////////////////////////

template <unsigned EnvMode, unsigned Format, bool NativeColor>
void TGetTexEnvColor(Color4 *pInOut, unsigned texture, ColorARGB cEnvColor) {
  typedef TFormatSize<TFormatInfo<Format>> FormatSize;

  if
    constexpr(NativeColor) {
      if
        constexpr(EnvMode == ENVMODE_REPLACE) {
          __unreferenced(cEnvColor);

          if
            constexpr(FormatSize::RGB) { pInOut->b = texture; }
          if
            constexpr(FormatSize::ALPHA) {
              Color4 cTexture;
              Format::TConvertFromNative<Format, false>(texture, &cTexture);
              pInOut->a = cTexture.a;
            }
        }
    }
  else {
    Color4 cTexture;
    Format::TConvertFromNative<Format, false>(&cTexture, texture);

    if
      constexpr(FormatSize::RGB) {
        if
          constexpr(FormatSize::ALPHA) {
            TGetTexEnvColorARGB<EnvMode>(pInOut, cTexture, cEnvColor);
          }
        else {
          TGetTexEnvColorRGB<EnvMode>(pInOut, cTexture, cEnvColor);
        }
      }
    else {
      if
        constexpr(FormatSize::ALPHA) {
          TGetTexEnvColorA<EnvMode>(pInOut, cTexture, cEnvColor);
        }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////

template <unsigned Format, bool NativeColor>
inline void TCalcFog(Color4 *pInOut, ColorARGB cFogColor, fixedRX fFactor) {

  const int factor = fFactor.GetRaw();

  if
    constexpr(NativeColor) {
      Color4 color;
      Format::TConvertFromNative<Format, false>(&color, pInOut->b);
      color.r =
          cFogColor.r + ((factor * (pInOut->r - cFogColor.r)) >> fixedRX::FRAC);
      color.g =
          cFogColor.g + ((factor * (pInOut->g - cFogColor.g)) >> fixedRX::FRAC);
      color.b =
          cFogColor.b + ((factor * (pInOut->b - cFogColor.b)) >> fixedRX::FRAC);
      pInOut->b = Format::TConvertToNative<Format>(color);
    }
  else {
    pInOut->r =
        cFogColor.r + ((factor * (pInOut->r - cFogColor.r)) >> fixedRX::FRAC);
    pInOut->g =
        cFogColor.g + ((factor * (pInOut->g - cFogColor.g)) >> fixedRX::FRAC);
    pInOut->b =
        cFogColor.b + ((factor * (pInOut->b - cFogColor.b)) >> fixedRX::FRAC);
  }
}

//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// <summary>
// </summary>
//----------------------------------------------------------------------------
template <unsigned AlphaFunc>
bool TAlphaTest(unsigned alphaValue, unsigned alphaRef) {
  return TCompare<AlphaFunc>(alphaValue, alphaRef);
}

//////////////////////////////////////////////////////////////////////////////

template <unsigned Format, unsigned BlendSrc, unsigned BlendDst,
          bool NativeColor>
class TBlend {
public:
  inline static unsigned Execute(const Color4 &cColor, unsigned dstColor);
};

template <unsigned BlendSrc, unsigned BlendDst, bool NativeColor>
class TBlend<FORMAT_R5G6B5, BlendSrc, BlendDst, NativeColor> {
private:
  enum {
    ZeroBlendSrc = (BLEND_ZERO == BlendSrc) && (BLEND_SRC_COLOR != BlendDst) &&
                   (BLEND_ONE_MINUS_SRC_COLOR != BlendDst) &&
                   (BLEND_DST_COLOR != BlendDst) &&
                   (BLEND_ONE_MINUS_DST_COLOR != BlendDst),
    ZeroBlendDst = (BLEND_ZERO == BlendDst) && (BLEND_SRC_COLOR != BlendSrc) &&
                   (BLEND_ONE_MINUS_SRC_COLOR != BlendSrc) &&
                   (BLEND_DST_COLOR != BlendSrc) &&
                   (BLEND_ONE_MINUS_DST_COLOR != BlendSrc),
    AlphaBlendSrc = (BLEND_SRC_ALPHA == BlendSrc) &&
                    (BLEND_ONE_MINUS_SRC_ALPHA == BlendDst),
    AlphaBlendDst = (BLEND_ONE_MINUS_SRC_ALPHA == BlendSrc) &&
                    (BLEND_SRC_ALPHA == BlendDst),
    NativeBlend =
        ZeroBlendSrc || ZeroBlendDst || AlphaBlendSrc || AlphaBlendDst,
  };

  template <unsigned BlendOp>
  inline static unsigned GetBlendOp(unsigned inColor, unsigned srcColor,
                                    unsigned srcAlpha, unsigned dstColor) {

    if
      constexpr(BlendOp == BLEND_ZERO) {
        __unreferenced(inColor);
        __unreferenced(srcColor);
        __unreferenced(srcAlpha);
        __unreferenced(dstColor);
        return 0;
      }

    if
      constexpr(BlendOp == BLEND_ONE) {
        __unreferenced(srcColor);
        __unreferenced(srcAlpha);
        __unreferenced(dstColor);
        return inColor;
      }

    if
      constexpr(BlendOp == BLEND_SRC_ALPHA) {
        __unreferenced(srcColor);
        __unreferenced(dstColor);
        const unsigned alpha =
            srcAlpha >> (8 - TFormatInfo<FORMAT_R5G6B5>::LERP);
        const TColorNative<FORMAT_R5G6B5> c0(inColor);
        return c0.Mul(alpha);
      }

    if
      constexpr(BlendOp == BLEND_ONE_MINUS_SRC_ALPHA) {
        __unreferenced(srcColor);
        __unreferenced(dstColor);
        const unsigned alpha =
            (0xff - srcAlpha) >> (8 - TFormatInfo<FORMAT_R5G6B5>::LERP);
        const TColorNative<FORMAT_R5G6B5> c0(inColor);
        return c0.Mul(alpha);
      }
  }

public:
  inline static unsigned Execute(const Color4 &cColor, unsigned dstColor) {
    if
      constexpr(NativeBlend) {
        unsigned srcColor;

        if
          constexpr(NativeColor) { srcColor = cColor.b; }
        else {
          srcColor = Format::TConvertToNative<FORMAT_R5G6B5>(cColor);
        }

        if
          constexpr(ZeroBlendSrc) {
            return GetBlendOp<BlendDst>(dstColor, srcColor, cColor.a, dstColor);
          }

        if
          constexpr(ZeroBlendDst) {
            return GetBlendOp<BlendSrc>(srcColor, srcColor, cColor.a, dstColor);
          }

        if
          constexpr(AlphaBlendSrc) {
            const unsigned alpha =
                cColor.a >> (8 - TFormatInfo<FORMAT_R5G6B5>::LERP);
            const TColorNative<FORMAT_R5G6B5> c0(dstColor);
            const TColorNative<FORMAT_R5G6B5> c1(srcColor);
            return c0.Lerp(c1, alpha);
          }

        if
          constexpr(AlphaBlendDst) {
            const unsigned alpha =
                cColor.a >> (8 - TFormatInfo<FORMAT_R5G6B5>::LERP);
            const TColorNative<FORMAT_R5G6B5> c0(srcColor);
            const TColorNative<FORMAT_R5G6B5> c1(dstColor);
            return c0.Lerp(c1, alpha);
          }
      }
    else {
      Color4 cSrcColor;
      Color4 cDstColor;

      if
        constexpr(NativeColor) {
          Format::TConvertFromNative<FORMAT_R5G6B5, false>(&cSrcColor,
                                                           cColor.b);
          cSrcColor.a = cColor.a;
        }
      else {
        cSrcColor = cColor;
      }

      Format::TConvertFromNative<FORMAT_R5G6B5, true>(&cDstColor, dstColor);

      TGetBlendCoeff<BlendSrc>(&cSrcColor, cSrcColor, cDstColor);
      TGetBlendCoeff<BlendDst>(&cDstColor, cSrcColor, cDstColor);

      cSrcColor.r = Math::Add8(cSrcColor.r, cDstColor.r);
      cSrcColor.g = Math::Add8(cSrcColor.g, cDstColor.g);
      cSrcColor.b = Math::Add8(cSrcColor.b, cDstColor.b);
      cSrcColor.a = Math::Add8(cSrcColor.a, cDstColor.a);

      return Format::TConvertToNative<FORMAT_R5G6B5>(cSrcColor);
    }
  }
};

//////////////////////////////////////////////////////////////////////////////

template <unsigned ColorFormat, unsigned LogicFunc, bool ColorWriteMask>
void TWriteColor(unsigned color, unsigned dstColor, unsigned writeMask,
                 uint8_t *pCB) {
  typedef TFormatInfo<ColorFormat> FormatInfo;

  unsigned result = TLogicOp<LogicFunc>(color, dstColor);

  if
    constexpr(ColorWriteMask) {
      result = (result & writeMask) | (dstColor & ~writeMask);
    }
  else {
    __unreferenced(writeMask);
    __unreferenced(dstColor);
  }

  *reinterpret_cast<typename FormatInfo::TYPE *>(pCB) =
      static_cast<typename FormatInfo::TYPE>(result);
}

template <unsigned Flags, unsigned States, unsigned Texture0, unsigned Texture1>
class TOptimizedScanlineA {
public:
  enum {
    ColorFormat = __get_bitfield(Flags, RASTERFLAGS::COLORFORMAT),
    DepthStencilFormat = __get_bitfield(Flags, RASTERFLAGS::DEPTHSTENCILFORMAT),
    DepthTest = __get_bitfield(Flags, RASTERFLAGS::DEPTHTEST),
    DepthWrite = __get_bitfield(Flags, RASTERFLAGS::DEPTHWRITE),
    StencilTest = __get_bitfield(Flags, RASTERFLAGS::STENCILTEST),
    StencilWrite = __get_bitfield(Flags, RASTERFLAGS::STENCILWRITE),
    Color = __get_bitfield(Flags, RASTERFLAGS::COLOR),
    Fog = __get_bitfield(Flags, RASTERFLAGS::FOG),
    NumTextures = __get_bitfield(Flags, RASTERFLAGS::NUMTEXTURES),
    TextureMips = __get_bitfield(Flags, RASTERFLAGS::TEXTUREMIPS),
    AlphaTest = __get_bitfield(Flags, RASTERFLAGS::ALPHATEST),
    Blend = __get_bitfield(Flags, RASTERFLAGS::BLEND),
    LogicOp = __get_bitfield(Flags, RASTERFLAGS::LOGICOP),
    ColorWriteMask = __get_bitfield(Flags, RASTERFLAGS::COLORWRITEMASK),
    InterpolateDepth = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEDEPTH),
    InterpolateColor = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATECOLOR),
    InterpolateAlpha = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEALPHA),
    InterpolateFog = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEFOG),
    InterpolateMips = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEMIPS),

    DepthFunc = __get_bitfield(States, RASTERSTATES::DEPTHFUNC),
    StencilFunc = __get_bitfield(States, RASTERSTATES::STENCILFUNC),
    StencilPass = __get_bitfield(States, RASTERSTATES::STENCILPASS),
    StencilFail = __get_bitfield(States, RASTERSTATES::STENCILFAIL),
    StencilZFail = __get_bitfield(States, RASTERSTATES::STENCILZFAIL),
    AlphaFunc = __get_bitfield(States, RASTERSTATES::ALPHAFUNC),
    BlendSrc = __get_bitfield(States, RASTERSTATES::BLENDSRC),
    BlendDst = __get_bitfield(States, RASTERSTATES::BLENDDST),
    LogicFunc = __get_bitfield(States, RASTERSTATES::LOGICFUNC),

    Tex0_Format = __get_bitfield(Texture0, TEXTURESTATES::FORMAT),
    Tex0_AddressS = __get_bitfield(Texture0, TEXTURESTATES::ADDRESSS),
    Tex0_AddressT = __get_bitfield(Texture0, TEXTURESTATES::ADDRESST),
    Tex0_MipFilter = __get_bitfield(Texture0, TEXTURESTATES::MIPFILTER),
    Tex0_MinFilter = __get_bitfield(Texture0, TEXTURESTATES::MINFILTER),
    Tex0_MagFilter = __get_bitfield(Texture0, TEXTURESTATES::MAGFILTER),
    Tex0_EnvMode = __get_bitfield(Texture0, TEXTURESTATES::ENVMODE),

    Tex1_Format = __get_bitfield(Texture1, TEXTURESTATES::FORMAT),
    Tex1_AddressS = __get_bitfield(Texture1, TEXTURESTATES::ADDRESSS),
    Tex1_AddressT = __get_bitfield(Texture1, TEXTURESTATES::ADDRESST),
    Tex1_MipFilter = __get_bitfield(Texture1, TEXTURESTATES::MIPFILTER),
    Tex1_MinFilter = __get_bitfield(Texture1, TEXTURESTATES::MINFILTER),
    Tex1_MagFilter = __get_bitfield(Texture1, TEXTURESTATES::MAGFILTER),
    Tex1_EnvMode = __get_bitfield(Texture1, TEXTURESTATES::ENVMODE),

    ColorStride = TFormatInfo<ColorFormat>::CBSIZE,
    DepthStencilStride = TFormatInfo<DepthStencilFormat>::CBSIZE,

    Mip0 = ((TextureMips >> 0) & 0x1),
    Mip1 = ((TextureMips >> 1) & 0x1),

    DEPTH_SIZE = DepthTest ? 1 : 0,
    COLOR_SIZE = Color ? 4 : 0,
    TEX0_SIZE = Texture0 ? 2 : 0,
    TEX1_SIZE = Texture1 ? 2 : 0,
    MIP0_SIZE = Mip0 ? 1 : 0,
    MIP1_SIZE = Mip1 ? 1 : 0,
    FOG_SIZE = Fog ? 1 : 0,

    REG_DEPTH = 0,
    REG_COLOR = REG_DEPTH + DEPTH_SIZE,
    REG_TEX0 = REG_COLOR + COLOR_SIZE,
    REG_TEX1 = REG_TEX0 + TEX0_SIZE,
    REG_MIP0 = REG_TEX1 + TEX1_SIZE,
    REG_MIP1 = REG_MIP0 + MIP0_SIZE,
    REG_FOG = REG_MIP1 + MIP1_SIZE,
    REG_SIZE = REG_FOG + FOG_SIZE,

    NativeColor = (((0 == NumTextures) && (0 == InterpolateColor) &&
                    (0 == InterpolateAlpha)) ||
                   ((1 == NumTextures) && (ENVMODE_REPLACE == Tex0_EnvMode) &&
                    (ColorFormat == Tex0_Format)))
                      ? 1
                      : 0,
  };

  inline static void Execute(const RasterData &rasterData, int y, int lx,
                             int rx) {
    const Sampler *sampler0, *sampler1;
    ColorARGB cEnvColor0, cEnvColor1;
    SurfaceDesc surface0, surface1;

    if
      constexpr(Texture0) {
        sampler0 = &rasterData.Samplers[0];
        cEnvColor0 = sampler0->cEnvColor_MaxMipLevel;

        if
          constexpr(!Mip0) { surface0 = sampler0->pMipLevels[0]; }

        if
          constexpr(Texture1) {
            sampler1 = &rasterData.Samplers[1];
            cEnvColor1 = sampler1->cEnvColor_MaxMipLevel;

            if
              constexpr(!Mip1) { surface1 = sampler1->pMipLevels[0]; }
          }
      }

    uint8_t alphaRef;
    if
      constexpr(AlphaTest) { alphaRef = rasterData.AlphaRef; }

    uint16_t stencilRef;
    uint16_t stencilMask;
    uint16_t stencilWriteMask;

    if
      constexpr(StencilTest) {
        stencilRef = rasterData.StencilRef;
        stencilMask = rasterData.StencilMask;
        stencilWriteMask = rasterData.StencilWriteMask;
      }

    ColorARGB cFogColor;

    if
      constexpr(Fog) {
        if
          constexpr(NativeColor) {
            cFogColor =
                Format::TConvertToNative<ColorFormat>(rasterData.cFogColor);
          }
        else {
          cFogColor = rasterData.cFogColor;
        }
      }

    const fixed4 fOffsetX = fixed4(lx) - rasterData.fRefX;
    const fixed4 fOffsetY = fixed4(y) - rasterData.fRefY;

    fixedRX fZ, fZdA;

    if
      constexpr(DepthTest) {
        fZ = rasterData.Registers[REG_DEPTH].m[2];

        if
          constexpr(InterpolateDepth) {
            fZdA = rasterData.Registers[REG_DEPTH].m[0];
            fixedRX fZdB = rasterData.Registers[REG_DEPTH].m[1];
            fZ += fZdA * fOffsetX + fZdB * fOffsetY;
          }
      }

    fixedRX fB, fG, fR, fA;
    fixedRX fBdA, fGdA, fRdA, fAdA;
    Color4 cNativeColor;

    if
      constexpr(Color) {
        fB = rasterData.Registers[REG_COLOR + 0].m[2];
        fG = rasterData.Registers[REG_COLOR + 1].m[2];
        fR = rasterData.Registers[REG_COLOR + 2].m[2];
        fA = rasterData.Registers[REG_COLOR + 3].m[2];

        if
          constexpr(InterpolateColor) {
            fBdA = rasterData.Registers[REG_COLOR + 0].m[0];
            fixedRX fBdB = rasterData.Registers[REG_COLOR + 0].m[1];
            fB += fBdA * fOffsetX + fBdB * fOffsetY;

            fGdA = rasterData.Registers[REG_COLOR + 1].m[0];
            fixedRX fGdB = rasterData.Registers[REG_COLOR + 1].m[1];
            fG += fGdA * fOffsetX + fGdB * fOffsetY;

            fRdA = rasterData.Registers[REG_COLOR + 2].m[0];
            fixedRX fRdB = rasterData.Registers[REG_COLOR + 2].m[1];
            fR += fRdA * fOffsetX + fRdB * fOffsetY;
          }

        if
          constexpr(InterpolateAlpha) {
            fAdA = rasterData.Registers[REG_COLOR + 3].m[0];
            fixedRX fAdB = rasterData.Registers[REG_COLOR + 3].m[1];
            fA += fAdA * fOffsetX + fAdB * fOffsetY;
          }

        if
          constexpr(NativeColor) {
            ToColor4(&cNativeColor, fA, fR, fG, fB);
            cNativeColor.b =
                Format::TConvertToNative<ColorFormat>(cNativeColor);
          }
      }

    fixedRX fU0, fV0, fU1, fV1;
    fixedRX fM0, fM1;
    fixedRX fU0dA, fV0dA, fU1dA, fV1dA;

    if
      constexpr(Texture0) {
        fU0dA = rasterData.Registers[REG_TEX0 + 0].m[0];
        fixedRX fU0dB = rasterData.Registers[REG_TEX0 + 0].m[1];
        fixedRX fU0dC = rasterData.Registers[REG_TEX0 + 0].m[2];
        fU0 = fU0dA * fOffsetX + fU0dB * fOffsetY + fU0dC;

        fV0dA = rasterData.Registers[REG_TEX0 + 1].m[0];
        fixedRX fV0dB = rasterData.Registers[REG_TEX0 + 1].m[1];
        fixedRX fV0dC = rasterData.Registers[REG_TEX0 + 1].m[2];
        fV0 = fV0dA * fOffsetX + fV0dB * fOffsetY + fV0dC;

        if
          constexpr(Mip0) { fM0 = rasterData.Registers[REG_MIP0 + 0].m[2]; }

        if
          constexpr(Texture1) {
            fU1dA = rasterData.Registers[REG_TEX1 + 0].m[0];
            fixedRX fU1dB = rasterData.Registers[REG_TEX1 + 0].m[1];
            fixedRX fU1dC = rasterData.Registers[REG_TEX1 + 0].m[2];
            fU1 = fU1dA * fOffsetX + fU1dB * fOffsetY + fU1dC;

            fV1dA = rasterData.Registers[REG_TEX1 + 1].m[0];
            fixedRX fV1dB = rasterData.Registers[REG_TEX1 + 1].m[1];
            fixedRX fV1dC = rasterData.Registers[REG_TEX1 + 1].m[2];
            fV1 = fV1dA * fOffsetX + fV1dB * fOffsetY + fV1dC;

            if
              constexpr(Mip1) {
                fM1 = rasterData.Registers[REG_MIP1 + 0].m[2];
              }
          }
      }

    fixedRX fFog, fFogdA;

    if
      constexpr(Fog) {
        fFog = rasterData.Registers[REG_FOG + 0].m[2];

        if
          constexpr(InterpolateFog) {
            fFogdA = rasterData.Registers[REG_FOG].m[0];
            fixedRX fFogdB = rasterData.Registers[REG_FOG].m[1];
            fFog += fFogdA * fOffsetX + fFogdB * fOffsetY;
          }
      }

    uint8_t *pDS;

    if
      constexpr(DepthTest || StencilTest) {
        uint8_t *const pDepthStencilBits = rasterData.pDepthStencilBits;
        const unsigned depthStencilPitch = rasterData.DepthStencilPitch;
        pDS =
            lx * DepthStencilStride + y * depthStencilPitch + pDepthStencilBits;
      }

    const unsigned colorWriteMask = rasterData.ColorWriteMask;
    uint8_t *const pColorBits = rasterData.pColorBits;
    const unsigned colorPitch = rasterData.ColorPitch;
    uint8_t *pCB = lx * ColorStride + y * colorPitch + pColorBits;
    uint8_t *const pCBEnd = pCB + (rx - lx) * ColorStride;

    Color4 cColor(0xff, 0xff, 0xff, 0xff);

    do {
      for (;;) {
        unsigned depthValue = 0;

        if
          constexpr(DepthTest || StencilTest) {

            if
              constexpr(DepthTest) {
                depthValue = static_cast<fixed16>(fZ).GetRaw() & 0xFFFF;
              }

            if
              constexpr(StencilTest) {
                if
                  constexpr(!AlphaTest) {
                    // Execute the depth stencil test
                    if (!TStencilTest<DepthTest, DepthFunc, DepthWrite,
                                      StencilFunc, StencilPass, StencilFail,
                                      StencilZFail, StencilWrite>(
                            depthValue, stencilRef, stencilMask,
                            stencilWriteMask, pDS)) {
                      break;
                    }
                  }
              }
            else {
              // Early out depth test
              if (!TCompare<DepthFunc>(
                      depthValue, *reinterpret_cast<const uint16_t *>(pDS))) {
                break;
              }

              if
                constexpr(DepthTest && DepthWrite) {
                  if
                    constexpr(!AlphaTest) {
                      // Apply DepthWrite
                      *reinterpret_cast<uint16_t *>(pDS) =
                          static_cast<uint16_t>(depthValue);
                    }
                }
            }
          }

        if
          constexpr(Color) {
            if
              constexpr(NativeColor) { cColor = cNativeColor; }
            else {
              ToColor4(&cColor, fA, fR, fG, fB);
            }
          }

        if
          constexpr(Texture0) {
            unsigned texture;

            if
              constexpr(Mip0) {
                texture =
                    TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                     Tex0_MagFilter, Tex0_Format, Tex0_AddressS,
                                     Tex0_AddressT>(sampler0, fU0, fV0, fM0);
              }
            else {
              texture =
                  TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                   Tex0_MagFilter, Tex0_Format, Tex0_AddressS,
                                   Tex0_AddressT>(surface0, fU0, fV0);
            }

            TGetTexEnvColor<Tex0_EnvMode, Tex0_Format, NativeColor>(
                &cColor, texture, cEnvColor0);

            if
              constexpr(Texture1) {
                if
                  constexpr(Mip1) {
                    texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                               Tex1_MagFilter, Tex1_Format,
                                               Tex1_AddressS, Tex1_AddressT>(
                        sampler1, fU1, fV1, fM1);
                  }
                else {
                  texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                             Tex1_MagFilter, Tex1_Format,
                                             Tex1_AddressS, Tex1_AddressT>(
                      surface1, fU1, fV1);
                }

                TGetTexEnvColor<Tex1_EnvMode, Tex1_Format, NativeColor>(
                    &cColor, texture, cEnvColor1);
              }
          }

        if
          constexpr(AlphaTest) {
            // Execute alpha test
            if (!TAlphaTest<AlphaFunc>(cColor.a, alphaRef)) {
              break;
            }

            if
              constexpr(DepthTest || StencilTest) {
                if
                  constexpr(StencilTest) {
                    // Execute the depth stencil test
                    if (!TStencilTest<DepthTest, DepthFunc, DepthWrite,
                                      StencilFunc, StencilPass, StencilFail,
                                      StencilZFail, StencilWrite>(
                            depthValue, stencilRef, stencilMask,
                            stencilWriteMask, pDS)) {
                      break;
                    }
                  }
                else {
                  if
                    constexpr(DepthTest && DepthWrite) {
                      *reinterpret_cast<uint16_t *>(pDS) =
                          static_cast<uint16_t>(depthValue);
                    }
                }
              }
          }

        if
          constexpr(Fog) {
            // Execute fog blend
            TCalcFog<ColorFormat, NativeColor>(&cColor, cFogColor, fFog);
          }

        unsigned color;

        const unsigned dstColor =
            *reinterpret_cast<typename TFormatInfo<ColorFormat>::TYPE *>(pCB);

        if
          constexpr(Blend) {
            // Execute pixel blend
            color =
                TBlend<ColorFormat, BlendSrc, BlendDst, NativeColor>::Execute(
                    cColor, dstColor);
          }
        else {
          if
            constexpr(NativeColor) { color = cColor.b; }
          else {
            // Convert color to native
            color = Format::TConvertToNative<ColorFormat>(cColor);
          }
        }

        // Write output color
        TWriteColor<ColorFormat,
                    (LogicOp ? LogicFunc : static_cast<uint32_t>(LOGICOP_COPY)),
                    ColorWriteMask>(color, dstColor, colorWriteMask, pCB);
        break;
      }

      if
        constexpr(DepthTest || StencilTest) { pDS += DepthStencilStride; }

      if
        constexpr(InterpolateDepth) { fZ += fZdA; }

      if
        constexpr(InterpolateColor) {
          fB += fBdA;
          fG += fGdA;
          fR += fRdA;
        }

      if
        constexpr(InterpolateAlpha) { fA += fAdA; }

      if
        constexpr(Texture0) {
          fU0 += fU0dA;
          fV0 += fV0dA;
        }

      if
        constexpr(Texture1) {
          fU1 += fU1dA;
          fV1 += fV1dA;
        }

      if
        constexpr(InterpolateFog) { fFog += fFogdA; }

      pCB += ColorStride;
    } while (pCB < pCBEnd);
  }
};

template <unsigned Flags, unsigned States, unsigned Texture0, unsigned Texture1>
class TOptimizedScanlineP {
public:
  enum {
    ColorFormat = __get_bitfield(Flags, RASTERFLAGS::COLORFORMAT),
    DepthStencilFormat = __get_bitfield(Flags, RASTERFLAGS::DEPTHSTENCILFORMAT),
    DepthTest = __get_bitfield(Flags, RASTERFLAGS::DEPTHTEST),
    DepthWrite = __get_bitfield(Flags, RASTERFLAGS::DEPTHWRITE),
    StencilTest = __get_bitfield(Flags, RASTERFLAGS::STENCILTEST),
    StencilWrite = __get_bitfield(Flags, RASTERFLAGS::STENCILWRITE),
    Color = __get_bitfield(Flags, RASTERFLAGS::COLOR),
    Fog = __get_bitfield(Flags, RASTERFLAGS::FOG),
    NumTextures = __get_bitfield(Flags, RASTERFLAGS::NUMTEXTURES),
    TextureMips = __get_bitfield(Flags, RASTERFLAGS::TEXTUREMIPS),
    AlphaTest = __get_bitfield(Flags, RASTERFLAGS::ALPHATEST),
    Blend = __get_bitfield(Flags, RASTERFLAGS::BLEND),
    LogicOp = __get_bitfield(Flags, RASTERFLAGS::LOGICOP),
    ColorWriteMask = __get_bitfield(Flags, RASTERFLAGS::COLORWRITEMASK),
    InterpolateDepth = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEDEPTH),
    InterpolateColor = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATECOLOR),
    InterpolateAlpha = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEALPHA),
    InterpolateFog = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEFOG),
    InterpolateMips = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEMIPS),

    DepthFunc = __get_bitfield(States, RASTERSTATES::DEPTHFUNC),
    StencilFunc = __get_bitfield(States, RASTERSTATES::STENCILFUNC),
    StencilPass = __get_bitfield(States, RASTERSTATES::STENCILPASS),
    StencilFail = __get_bitfield(States, RASTERSTATES::STENCILFAIL),
    StencilZFail = __get_bitfield(States, RASTERSTATES::STENCILZFAIL),
    AlphaFunc = __get_bitfield(States, RASTERSTATES::ALPHAFUNC),
    BlendSrc = __get_bitfield(States, RASTERSTATES::BLENDSRC),
    BlendDst = __get_bitfield(States, RASTERSTATES::BLENDDST),
    LogicFunc = __get_bitfield(States, RASTERSTATES::LOGICFUNC),

    Tex0_Format = __get_bitfield(Texture0, TEXTURESTATES::FORMAT),
    Tex0_AddressS = __get_bitfield(Texture0, TEXTURESTATES::ADDRESSS),
    Tex0_AddressT = __get_bitfield(Texture0, TEXTURESTATES::ADDRESST),
    Tex0_MipFilter = __get_bitfield(Texture0, TEXTURESTATES::MIPFILTER),
    Tex0_MinFilter = __get_bitfield(Texture0, TEXTURESTATES::MINFILTER),
    Tex0_MagFilter = __get_bitfield(Texture0, TEXTURESTATES::MAGFILTER),
    Tex0_EnvMode = __get_bitfield(Texture0, TEXTURESTATES::ENVMODE),

    Tex1_Format = __get_bitfield(Texture1, TEXTURESTATES::FORMAT),
    Tex1_AddressS = __get_bitfield(Texture1, TEXTURESTATES::ADDRESSS),
    Tex1_AddressT = __get_bitfield(Texture1, TEXTURESTATES::ADDRESST),
    Tex1_MipFilter = __get_bitfield(Texture1, TEXTURESTATES::MIPFILTER),
    Tex1_MinFilter = __get_bitfield(Texture1, TEXTURESTATES::MINFILTER),
    Tex1_MagFilter = __get_bitfield(Texture1, TEXTURESTATES::MAGFILTER),
    Tex1_EnvMode = __get_bitfield(Texture1, TEXTURESTATES::ENVMODE),

    ColorStride = TFormatInfo<ColorFormat>::CBSIZE,
    DepthStencilStride = TFormatInfo<DepthStencilFormat>::CBSIZE,

    Mip0 = ((TextureMips >> 0) & 0x1),
    Mip1 = ((TextureMips >> 1) & 0x1),

    DEPTH_SIZE = DepthTest ? 1 : 0,
    COLOR_SIZE = Color ? 4 : 0,
    RHW_SIZE = 1,
    TEX0_SIZE = Texture0 ? 2 : 0,
    TEX1_SIZE = Texture1 ? 2 : 0,
    MIP0_SIZE = Mip0 ? 1 : 0,
    MIP1_SIZE = Mip1 ? 1 : 0,
    FOG_SIZE = Fog ? 1 : 0,

    REG_DEPTH = 0,
    REG_COLOR = REG_DEPTH + DEPTH_SIZE,
    REG_RHW = REG_COLOR + COLOR_SIZE,
    REG_TEX0 = REG_RHW + RHW_SIZE,
    REG_TEX1 = REG_TEX0 + TEX0_SIZE,
    REG_MIP0 = REG_TEX1 + TEX1_SIZE,
    REG_MIP1 = REG_MIP0 + MIP0_SIZE,
    REG_FOG = REG_MIP1 + MIP1_SIZE,
    REG_SIZE = REG_FOG + FOG_SIZE,

    NativeColor = (((0 == NumTextures) && (0 == InterpolateColor) &&
                    (0 == InterpolateAlpha)) ||
                   ((1 == NumTextures) && (ENVMODE_REPLACE == Tex0_EnvMode) &&
                    (ColorFormat == Tex0_Format)))
                      ? 1
                      : 0,
  };

  inline static void Execute(const RasterData &rasterData, int y, int lx,
                             int rx) {
    const Sampler *sampler0, *sampler1;
    ColorARGB cEnvColor0, cEnvColor1;
    SurfaceDesc surface0, surface1;

    if
      constexpr(Texture0) {
        sampler0 = &rasterData.Samplers[0];
        cEnvColor0 = sampler0->cEnvColor_MaxMipLevel;

        if
          constexpr(!Mip0) { surface0 = sampler0->pMipLevels[0]; }

        if
          constexpr(Texture1) {
            sampler1 = &rasterData.Samplers[1];
            cEnvColor1 = sampler1->cEnvColor_MaxMipLevel;

            if
              constexpr(!Mip1) { surface1 = sampler1->pMipLevels[0]; }
          }
      }

    uint8_t alphaRef;

    if
      constexpr(AlphaTest) { alphaRef = rasterData.AlphaRef; }

    uint16_t stencilRef;
    uint16_t stencilMask;
    uint16_t stencilWriteMask;

    if
      constexpr(StencilTest) {
        stencilRef = rasterData.StencilRef;
        stencilMask = rasterData.StencilMask;
        stencilWriteMask = rasterData.StencilWriteMask;
      }

    ColorARGB cFogColor;

    if
      constexpr(Fog) {
        if
          constexpr(NativeColor) {
            cFogColor =
                Format::TConvertToNative<ColorFormat>(rasterData.cFogColor);
          }
        else {
          cFogColor = rasterData.cFogColor;
        }
      }

    const fixed4 fOffsetX = fixed4(lx) - rasterData.fRefX;
    const fixed4 fOffsetY = fixed4(y) - rasterData.fRefY;

    fixedRX fZ, fZdA;

    if
      constexpr(DepthTest) {
        fZ = rasterData.Registers[REG_DEPTH].m[2];

        if
          constexpr(InterpolateDepth) {
            fZdA = rasterData.Registers[REG_DEPTH].m[0];
            fixedRX fZdB = rasterData.Registers[REG_DEPTH].m[1];
            fZ += fZdA * fOffsetX + fZdB * fOffsetY;
          }
      }

    fixedRX fB, fG, fR, fA;
    fixedRX fBdA, fGdA, fRdA, fAdA;
    Color4 cNativeColor;

    if
      constexpr(Color) {
        fB = rasterData.Registers[REG_COLOR + 0].m[2];
        fG = rasterData.Registers[REG_COLOR + 1].m[2];
        fR = rasterData.Registers[REG_COLOR + 2].m[2];
        fA = rasterData.Registers[REG_COLOR + 3].m[2];

        if
          constexpr(InterpolateColor) {
            fBdA = rasterData.Registers[REG_COLOR + 0].m[0];
            fixedRX fBdB = rasterData.Registers[REG_COLOR + 0].m[1];
            fB += fBdA * fOffsetX + fBdB * fOffsetY;

            fGdA = rasterData.Registers[REG_COLOR + 1].m[0];
            fixedRX fGdB = rasterData.Registers[REG_COLOR + 1].m[1];
            fG += fGdA * fOffsetX + fGdB * fOffsetY;

            fRdA = rasterData.Registers[REG_COLOR + 2].m[0];
            fixedRX fRdB = rasterData.Registers[REG_COLOR + 2].m[1];
            fR += fRdA * fOffsetX + fRdB * fOffsetY;
          }

        if
          constexpr(InterpolateAlpha) {
            fAdA = rasterData.Registers[REG_COLOR + 3].m[0];
            fixedRX fAdB = rasterData.Registers[REG_COLOR + 3].m[1];
            fA += fAdA * fOffsetX + fAdB * fOffsetY;
          }

        if
          constexpr(NativeColor) {
            ToColor4(&cNativeColor, fA, fR, fG, fB);
            cNativeColor.b =
                Format::TConvertToNative<ColorFormat>(cNativeColor);
          }
      }

    fixedRX fRhw, fRhwdA;
    fixedRX fU0OverW, fV0OverW, fU1OverW, fV1OverW;
    fixedRX fU0OverWdA, fV0OverWdA, fU1OverWdA, fV1OverWdA;
    fixedRX fM0OverW2, fM1OverW2;
    fixedRX fM0OverW2dA, fM1OverW2dA;

    if
      constexpr(Texture0) {
        fRhwdA = rasterData.Registers[REG_RHW].m[0];
        fixedRX fRhwdB = rasterData.Registers[REG_RHW].m[1];
        fixedRX fRhwdC = rasterData.Registers[REG_RHW].m[2];
        fRhw = fRhwdA * fOffsetX + fRhwdB * fOffsetY + fRhwdC;

        fU0OverWdA = rasterData.Registers[REG_TEX0 + 0].m[0];
        fixedRX fU0OverWdB = rasterData.Registers[REG_TEX0 + 0].m[1];
        fixedRX fU0OverWdC = rasterData.Registers[REG_TEX0 + 0].m[2];
        fU0OverW = fU0OverWdA * fOffsetX + fU0OverWdB * fOffsetY + fU0OverWdC;

        fV0OverWdA = rasterData.Registers[REG_TEX0 + 1].m[0];
        fixedRX fV0OverWdB = rasterData.Registers[REG_TEX0 + 1].m[1];
        fixedRX fV0OverWdC = rasterData.Registers[REG_TEX0 + 1].m[2];
        fV0OverW = fV0OverWdA * fOffsetX + fV0OverWdB * fOffsetY + fV0OverWdC;

        if
          constexpr(Mip0) {
            fM0OverW2 = rasterData.Registers[REG_MIP0].m[2];

            if
              constexpr(InterpolateMips & 0x1) {
                fM0OverW2dA = rasterData.Registers[REG_MIP0].m[0];
                fixedRX fM0OverW2dB = rasterData.Registers[REG_MIP0].m[1];
                fM0OverW2 += fM0OverW2dA * fOffsetX + fM0OverW2dB * fOffsetY;
              }
          }

        if
          constexpr(Texture1) {
            fixedRX fU1OverWdA = rasterData.Registers[REG_TEX1 + 0].m[0];
            fixedRX fU1OverWdB = rasterData.Registers[REG_TEX1 + 0].m[1];
            fixedRX fU1OverWdC = rasterData.Registers[REG_TEX1 + 0].m[2];
            fU1OverW =
                fU1OverWdA * fOffsetX + fU1OverWdB * fOffsetY + fU1OverWdC;

            fixedRX fV1OverWdA = rasterData.Registers[REG_TEX1 + 1].m[0];
            fixedRX fV1OverWdB = rasterData.Registers[REG_TEX1 + 1].m[1];
            fixedRX fV1OverWdC = rasterData.Registers[REG_TEX1 + 1].m[2];
            fV1OverW =
                fV1OverWdA * fOffsetX + fV1OverWdB * fOffsetY + fV1OverWdC;

            if
              constexpr(Mip1) {
                fM1OverW2 = rasterData.Registers[REG_MIP1].m[2];

                if
                  constexpr(InterpolateMips & 0x2) {
                    fM1OverW2dA = rasterData.Registers[REG_MIP1].m[0];
                    fixedRX fM1OverW2dB = rasterData.Registers[REG_MIP1].m[1];
                    fM1OverW2 +=
                        fM1OverW2dA * fOffsetX + fM1OverW2dB * fOffsetY;
                  }
              }
          }
      }

    fixedRX fFog, fFogdA;

    if
      constexpr(Fog) {
        fFog = rasterData.Registers[REG_FOG + 0].m[2];

        if
          constexpr(InterpolateFog) {
            fFogdA = rasterData.Registers[REG_FOG].m[0];
            fixedRX fFogdB = rasterData.Registers[REG_FOG].m[1];
            fFog += fFogdA * fOffsetX + fFogdB * fOffsetY;
          }
      }

    const fixedW fW = Math::TInv<fixedW>(fRhw);
    fixedW fW2;

    if
      constexpr(Mip0 || Mip1) { fW2 = fW * fW; }

    fixedRX fU0, fV0, fU1, fV1;
    fixedRX fM0, fM1;

    if
      constexpr(Texture0) {
        fU0 = fU0OverW * fW;
        fV0 = fV0OverW * fW;

        if
          constexpr(Mip0) {
            if
              constexpr(InterpolateMips & 0x1) { fM0 = fM0OverW2 * fW2; }
            else {
              fM0 = fM0OverW2;
            }
          }
      }

    if
      constexpr(Texture1) {
        fU1 = fU1OverW * fW;
        fV1 = fV1OverW * fW;

        if
          constexpr(Mip1) {
            if
              constexpr(InterpolateMips & 0x2) { fM1 = fM1OverW2 * fW2; }
            else {
              fM1 = fM1OverW2;
            }
          }
      }

    uint8_t *pDS;

    if
      constexpr(DepthTest || StencilTest) {
        uint8_t *const pDepthStencilBits = rasterData.pDepthStencilBits;
        const unsigned depthStencilPitch = rasterData.DepthStencilPitch;
        pDS =
            lx * DepthStencilStride + y * depthStencilPitch + pDepthStencilBits;
      }

    const unsigned colorWriteMask = rasterData.ColorWriteMask;
    uint8_t *const pColorBits = rasterData.pColorBits;
    const unsigned colorPitch = rasterData.ColorPitch;
    uint8_t *pCB = lx * ColorStride + y * colorPitch + pColorBits;
    int width = rx - lx;

    Color4 cColor(0xff, 0xff, 0xff, 0xff);

    do {
      const unsigned int blockWidth1 = Math::TMin(width, MAX_BLOCK_SIZE);
      const unsigned int log2width = Math::iLog2(blockWidth1);
      const unsigned int blockWidth = 1 << log2width;
      width -= blockWidth;
      uint8_t *const pCBEnd = pCB + blockWidth * ColorStride;

      fixedRX fdUr0, fdVr0, fdMr0;
      fixedRX fdUr1, fdVr1, fdMr1;
      fixedW fWr2;

      if (log2width) {
        fRhw += fRhwdA << log2width;
        const fixedW fWr = Math::TInv<fixedW>(fRhw);

        if
          constexpr(InterpolateMips) { fWr2 = fWr * fWr; }

        if
          constexpr(Texture0) {
            fU0OverW += fU0OverWdA << log2width;
            fV0OverW += fV0OverWdA << log2width;
            fdUr0 = (fU0OverW * fWr - fU0) >> log2width;
            fdVr0 = (fV0OverW * fWr - fV0) >> log2width;

            if
              constexpr(InterpolateMips & 0x1) {
                fM0OverW2 += fM0OverW2dA << log2width;
                fdMr0 = (fM0OverW2 * fWr2 - fM0) >> log2width;
              }
          }

        if
          constexpr(Texture1) {
            fU1OverW += fU1OverWdA << log2width;
            fV1OverW += fV1OverWdA << log2width;
            fdUr1 = (fU1OverW * fWr - fU1) >> log2width;
            fdVr1 = (fV1OverW * fWr - fV1) >> log2width;

            if
              constexpr(InterpolateMips & 0x2) {
                fM1OverW2 += fM1OverW2dA << log2width;
                fdMr1 = (fM1OverW2 * fWr2 - fM1) >> log2width;
              }
          }
      }

      do {
        for (;;) {
          unsigned depthValue = 0;

          if
            constexpr(DepthTest || StencilTest) {

              if
                constexpr(DepthTest) {
                  depthValue = static_cast<fixed16>(fZ).GetRaw() & 0xFFFF;
                }

              if
                constexpr(StencilTest) {
                  if
                    constexpr(!AlphaTest) {
                      // Execute the depth stencil test
                      if (!TStencilTest<DepthTest, DepthFunc, DepthWrite,
                                        StencilFunc, StencilPass, StencilFail,
                                        StencilZFail, StencilWrite>(
                              depthValue, stencilRef, stencilMask,
                              stencilWriteMask, pDS)) {
                        break;
                      }
                    }
                }
              else {
                // Early out depth test
                if (!TCompare<DepthFunc>(
                        depthValue, *reinterpret_cast<const uint16_t *>(pDS))) {
                  break;
                }

                if
                  constexpr(DepthTest && DepthWrite) {
                    if
                      constexpr(!AlphaTest) {
                        // Apply DepthWrite
                        *reinterpret_cast<uint16_t *>(pDS) =
                            static_cast<uint16_t>(depthValue);
                      }
                  }
              }
            }

          if
            constexpr(Color) {
              if
                constexpr(NativeColor) { cColor = cNativeColor; }
              else {
                ToColor4(&cColor, fA, fR, fG, fB);
              }
            }

          // Execute texture operations
          if
            constexpr(Texture0) {
              unsigned texture;

              if
                constexpr(Mip0) {
                  texture = TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                             Tex0_MagFilter, Tex0_Format,
                                             Tex0_AddressS, Tex0_AddressT>(
                      sampler0, fU0, fV0, fM0);
                }
              else {
                texture =
                    TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                     Tex0_MagFilter, Tex0_Format, Tex0_AddressS,
                                     Tex0_AddressT>(surface0, fU0, fV0);
              }

              TGetTexEnvColor<Tex0_EnvMode, Tex0_Format, NativeColor>(
                  &cColor, texture, cEnvColor0);

              if
                constexpr(Texture1) {
                  if
                    constexpr(Mip1) {
                      texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                                 Tex1_MagFilter, Tex1_Format,
                                                 Tex1_AddressS, Tex1_AddressT>(
                          sampler1, fU1, fV1, fM1);
                    }
                  else {
                    texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                               Tex1_MagFilter, Tex1_Format,
                                               Tex1_AddressS, Tex1_AddressT>(
                        surface1, fU1, fV1);
                  }

                  TGetTexEnvColor<Tex1_EnvMode, Tex1_Format, NativeColor>(
                      &cColor, texture, cEnvColor1);
                }
            }

          if
            constexpr(AlphaTest) {
              // Execute alpha test
              if (!TAlphaTest<AlphaFunc>(cColor.a, alphaRef)) {
                break;
              }

              if
                constexpr(DepthTest || StencilTest) {
                  if
                    constexpr(StencilTest) {
                      // Execute the depth stencil test
                      if (!TStencilTest<DepthTest, DepthFunc, DepthWrite,
                                        StencilFunc, StencilPass, StencilFail,
                                        StencilZFail, StencilWrite>(
                              depthValue, stencilRef, stencilMask,
                              stencilWriteMask, pDS)) {
                        break;
                      }
                    }
                  else {
                    if
                      constexpr(DepthTest && DepthWrite) {
                        *reinterpret_cast<uint16_t *>(pDS) =
                            static_cast<uint16_t>(depthValue);
                      }
                  }
                }
            }

          if
            constexpr(Fog) {
              // Execute fog blend
              TCalcFog<ColorFormat, NativeColor>(&cColor, cFogColor, fFog);
            }

          unsigned color;

          const unsigned dstColor =
              *reinterpret_cast<typename TFormatInfo<ColorFormat>::TYPE *>(pCB);

          if
            constexpr(Blend) {
              // Execute pixel blend
              color =
                  TBlend<ColorFormat, BlendSrc, BlendDst, NativeColor>::Execute(
                      cColor, dstColor);
            }
          else {
            if
              constexpr(NativeColor) { color = cColor.b; }
            else {
              // Convert color to native
              color = Format::TConvertToNative<ColorFormat>(cColor);
            }
          }

          // Write output color
          TWriteColor<ColorFormat, LogicOp ? LogicFunc : LOGICOP_COPY,
                      ColorWriteMask>(color, dstColor, colorWriteMask, pCB);
          break;
        }

        if
          constexpr(DepthTest || StencilTest) { pDS += DepthStencilStride; }

        if
          constexpr(InterpolateDepth) { fZ += fZdA; }

        if
          constexpr(InterpolateColor) {
            fB += fBdA;
            fG += fGdA;
            fR += fRdA;
          }

        if
          constexpr(InterpolateAlpha) { fA += fAdA; }

        if
          constexpr(Texture0) {
            fU0 += fdUr0;
            fV0 += fdVr0;

            if
              constexpr(InterpolateMips & 0x1) { fM0 += fdMr0; }
          }

        if
          constexpr(Texture1) {
            fU1 += fdUr1;
            fV1 += fdVr1;

            if
              constexpr(InterpolateMips & 0x2) { fM1 += fdMr1; }
          }

        if
          constexpr(InterpolateFog) { fFog += fFogdA; }

        pCB += ColorStride;
      } while (pCB < pCBEnd);
    } while (width);
  }
};

template <unsigned Flags, unsigned States, unsigned Texture0, unsigned Texture1>
class TOptimizedScanlineAZ {
public:
  enum {
    ColorFormat = __get_bitfield(Flags, RASTERFLAGS::COLORFORMAT),
    DepthStencilFormat = __get_bitfield(Flags, RASTERFLAGS::DEPTHSTENCILFORMAT),
    DepthTest = __get_bitfield(Flags, RASTERFLAGS::DEPTHTEST),
    DepthWrite = __get_bitfield(Flags, RASTERFLAGS::DEPTHWRITE),
    StencilTest = __get_bitfield(Flags, RASTERFLAGS::STENCILTEST),
    StencilWrite = __get_bitfield(Flags, RASTERFLAGS::STENCILWRITE),
    Color = __get_bitfield(Flags, RASTERFLAGS::COLOR),
    Fog = __get_bitfield(Flags, RASTERFLAGS::FOG),
    NumTextures = __get_bitfield(Flags, RASTERFLAGS::NUMTEXTURES),
    TextureMips = __get_bitfield(Flags, RASTERFLAGS::TEXTUREMIPS),
    AlphaTest = __get_bitfield(Flags, RASTERFLAGS::ALPHATEST),
    Blend = __get_bitfield(Flags, RASTERFLAGS::BLEND),
    LogicOp = __get_bitfield(Flags, RASTERFLAGS::LOGICOP),
    ColorWriteMask = __get_bitfield(Flags, RASTERFLAGS::COLORWRITEMASK),
    InterpolateDepth = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEDEPTH),
    InterpolateColor = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATECOLOR),
    InterpolateAlpha = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEALPHA),
    InterpolateFog = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEFOG),
    InterpolateMips = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEMIPS),

    DepthFunc = __get_bitfield(States, RASTERSTATES::DEPTHFUNC),
    StencilFunc = __get_bitfield(States, RASTERSTATES::STENCILFUNC),
    StencilPass = __get_bitfield(States, RASTERSTATES::STENCILPASS),
    StencilFail = __get_bitfield(States, RASTERSTATES::STENCILFAIL),
    StencilZFail = __get_bitfield(States, RASTERSTATES::STENCILZFAIL),
    AlphaFunc = __get_bitfield(States, RASTERSTATES::ALPHAFUNC),
    BlendSrc = __get_bitfield(States, RASTERSTATES::BLENDSRC),
    BlendDst = __get_bitfield(States, RASTERSTATES::BLENDDST),
    LogicFunc = __get_bitfield(States, RASTERSTATES::LOGICFUNC),

    Tex0_Format = __get_bitfield(Texture0, TEXTURESTATES::FORMAT),
    Tex0_AddressS = __get_bitfield(Texture0, TEXTURESTATES::ADDRESSS),
    Tex0_AddressT = __get_bitfield(Texture0, TEXTURESTATES::ADDRESST),
    Tex0_MipFilter = __get_bitfield(Texture0, TEXTURESTATES::MIPFILTER),
    Tex0_MinFilter = __get_bitfield(Texture0, TEXTURESTATES::MINFILTER),
    Tex0_MagFilter = __get_bitfield(Texture0, TEXTURESTATES::MAGFILTER),
    Tex0_EnvMode = __get_bitfield(Texture0, TEXTURESTATES::ENVMODE),

    Tex1_Format = __get_bitfield(Texture1, TEXTURESTATES::FORMAT),
    Tex1_AddressS = __get_bitfield(Texture1, TEXTURESTATES::ADDRESSS),
    Tex1_AddressT = __get_bitfield(Texture1, TEXTURESTATES::ADDRESST),
    Tex1_MipFilter = __get_bitfield(Texture1, TEXTURESTATES::MIPFILTER),
    Tex1_MinFilter = __get_bitfield(Texture1, TEXTURESTATES::MINFILTER),
    Tex1_MagFilter = __get_bitfield(Texture1, TEXTURESTATES::MAGFILTER),
    Tex1_EnvMode = __get_bitfield(Texture1, TEXTURESTATES::ENVMODE),

    ColorStride = TFormatInfo<ColorFormat>::CBSIZE,
    DepthStencilStride = TFormatInfo<DepthStencilFormat>::CBSIZE,

    Mip0 = ((TextureMips >> 0) & 0x1),
    Mip1 = ((TextureMips >> 1) & 0x1),

    DEPTH_SIZE = DepthTest ? 1 : 0,
    COLOR_SIZE = Color ? 4 : 0,
    TEX0_SIZE = Texture0 ? 2 : 0,
    TEX1_SIZE = Texture1 ? 2 : 0,
    MIP0_SIZE = Mip0 ? 1 : 0,
    MIP1_SIZE = Mip1 ? 1 : 0,
    FOG_SIZE = Fog ? 1 : 0,

    REG_DEPTH = 0,
    REG_COLOR = REG_DEPTH + DEPTH_SIZE,
    REG_TEX0 = REG_COLOR + COLOR_SIZE,
    REG_TEX1 = REG_TEX0 + TEX0_SIZE,
    REG_MIP0 = REG_TEX1 + TEX1_SIZE,
    REG_MIP1 = REG_MIP0 + MIP0_SIZE,
    REG_FOG = REG_MIP1 + MIP1_SIZE,
    REG_SIZE = REG_FOG + FOG_SIZE,

    NativeColor = (((0 == NumTextures) && (0 == InterpolateColor) &&
                    (0 == InterpolateAlpha)) ||
                   ((1 == NumTextures) && (ENVMODE_REPLACE == Tex0_EnvMode) &&
                    (ColorFormat == Tex0_Format)))
                      ? 1
                      : 0,
  };

  inline static void Execute(const RasterData &rasterData, int y, int lx,
                             int rx) {
    const Sampler *sampler0, *sampler1;
    ColorARGB cEnvColor0, cEnvColor1;
    SurfaceDesc surface0, surface1;

    if
      constexpr(Texture0) {
        sampler0 = &rasterData.Samplers[0];
        cEnvColor0 = sampler0->cEnvColor_MaxMipLevel;

        if
          constexpr(!Mip0) { surface0 = sampler0->pMipLevels[0]; }

        if
          constexpr(Texture1) {
            sampler1 = &rasterData.Samplers[1];
            cEnvColor1 = sampler1->cEnvColor_MaxMipLevel;

            if
              constexpr(!Mip1) { surface1 = sampler1->pMipLevels[0]; }
          }
      }

    uint16_t stencilRef;
    uint16_t stencilMask;
    uint16_t stencilWriteMask;

    if
      constexpr(StencilTest && StencilWrite) {
        stencilRef = rasterData.StencilRef;
        stencilMask = rasterData.StencilMask;
        stencilWriteMask = rasterData.StencilWriteMask;
      }

    ColorARGB cFogColor;

    if
      constexpr(Fog) {
        if
          constexpr(NativeColor) {
            cFogColor =
                Format::TConvertToNative<ColorFormat>(rasterData.cFogColor);
          }
        else {
          cFogColor = rasterData.cFogColor;
        }
      }

    const fixed4 fOffsetX = fixed4(lx) - rasterData.fRefX;
    const fixed4 fOffsetY = fixed4(y) - rasterData.fRefY;

    fixedRX fZ, fZdA;

    if
      constexpr(DepthTest) {
        fZ = rasterData.Registers[REG_DEPTH].m[2];

        if
          constexpr(InterpolateDepth) {
            fZdA = rasterData.Registers[REG_DEPTH].m[0];
            fixedRX fZdB = rasterData.Registers[REG_DEPTH].m[1];
            fZ += fZdA * fOffsetX + fZdB * fOffsetY;
          }
      }

    fixedRX fB, fG, fR, fA;
    fixedRX fBdA, fGdA, fRdA, fAdA;
    Color4 cNativeColor;

    if
      constexpr(Color) {
        fB = rasterData.Registers[REG_COLOR + 0].m[2];
        fG = rasterData.Registers[REG_COLOR + 1].m[2];
        fR = rasterData.Registers[REG_COLOR + 2].m[2];
        fA = rasterData.Registers[REG_COLOR + 3].m[2];

        if
          constexpr(InterpolateColor) {
            fBdA = rasterData.Registers[REG_COLOR + 0].m[0];
            fixedRX fBdB = rasterData.Registers[REG_COLOR + 0].m[1];
            fB += fBdA * fOffsetX + fBdB * fOffsetY;

            fGdA = rasterData.Registers[REG_COLOR + 1].m[0];
            fixedRX fGdB = rasterData.Registers[REG_COLOR + 1].m[1];
            fG += fGdA * fOffsetX + fGdB * fOffsetY;

            fRdA = rasterData.Registers[REG_COLOR + 2].m[0];
            fixedRX fRdB = rasterData.Registers[REG_COLOR + 2].m[1];
            fR += fRdA * fOffsetX + fRdB * fOffsetY;
          }

        if
          constexpr(InterpolateAlpha) {
            fAdA = rasterData.Registers[REG_COLOR + 3].m[0];
            fixedRX fAdB = rasterData.Registers[REG_COLOR + 3].m[1];
            fA += fAdA * fOffsetX + fAdB * fOffsetY;
          }

        if
          constexpr(NativeColor) {
            ToColor4(&cNativeColor, fA, fR, fG, fB);
            cNativeColor.b =
                Format::TConvertToNative<ColorFormat>(cNativeColor);
          }
      }

    fixedRX fU0, fV0, fU1, fV1;
    fixedRX fM0, fM1;
    fixedRX fU0dA, fV0dA, fU1dA, fV1dA;

    if
      constexpr(Texture0) {
        fU0dA = rasterData.Registers[REG_TEX0 + 0].m[0];
        fixedRX fU0dB = rasterData.Registers[REG_TEX0 + 0].m[1];
        fixedRX fU0dC = rasterData.Registers[REG_TEX0 + 0].m[2];
        fU0 = fU0dA * fOffsetX + fU0dB * fOffsetY + fU0dC;

        fV0dA = rasterData.Registers[REG_TEX0 + 1].m[0];
        fixedRX fV0dB = rasterData.Registers[REG_TEX0 + 1].m[1];
        fixedRX fV0dC = rasterData.Registers[REG_TEX0 + 1].m[2];
        fV0 = fV0dA * fOffsetX + fV0dB * fOffsetY + fV0dC;

        if
          constexpr(Mip0) { fM0 = rasterData.Registers[REG_MIP0 + 0].m[2]; }

        if
          constexpr(Texture1) {
            fU1dA = rasterData.Registers[REG_TEX1 + 0].m[0];
            fixedRX fU1dB = rasterData.Registers[REG_TEX1 + 0].m[1];
            fixedRX fU1dC = rasterData.Registers[REG_TEX1 + 0].m[2];
            fU1 = fU1dA * fOffsetX + fU1dB * fOffsetY + fU1dC;

            fV1dA = rasterData.Registers[REG_TEX1 + 1].m[0];
            fixedRX fV1dB = rasterData.Registers[REG_TEX1 + 1].m[1];
            fixedRX fV1dC = rasterData.Registers[REG_TEX1 + 1].m[2];
            fV1 = fV1dA * fOffsetX + fV1dB * fOffsetY + fV1dC;

            if
              constexpr(Mip1) {
                fM1 = rasterData.Registers[REG_MIP1 + 0].m[2];
              }
          }
      }

    fixedRX fFog, fFogdA;

    if
      constexpr(Fog) {
        fFog = rasterData.Registers[REG_FOG + 0].m[2];

        if
          constexpr(InterpolateFog) {
            fFogdA = rasterData.Registers[REG_FOG].m[0];
            fixedRX fFogdB = rasterData.Registers[REG_FOG].m[1];
            fFog += fFogdA * fOffsetX + fFogdB * fOffsetY;
          }
      }

    const unsigned colorWriteMask = rasterData.ColorWriteMask;
    uint8_t *const pColorBits = rasterData.pColorBits;
    const unsigned colorPitch = rasterData.ColorPitch;
    uint8_t *pCB = lx * ColorStride + y * colorPitch + pColorBits;

    uint8_t *const pDepthStencilBits = rasterData.pDepthStencilBits;
    const unsigned depthStencilPitch = rasterData.DepthStencilPitch;
    uint8_t *pDS =
        lx * DepthStencilStride + y * depthStencilPitch + pDepthStencilBits;

    Color4 cColor(0xff, 0xff, 0xff, 0xff);

    int count = rx - lx;

    do {
      int start = count;

      do {
        unsigned depthValue = 0;

        if
          constexpr(DepthTest) {
            depthValue = static_cast<fixed16>(fZ).GetRaw() & 0xFFFF;
          }

        if
          constexpr(StencilTest) {
            // Execute the depth stencil test
            if (TStencilTest<DepthTest, DepthFunc, false, StencilFunc,
                             STENCIL_KEEP, StencilFail, StencilZFail,
                             StencilWrite>(depthValue, stencilRef, stencilMask,
                                           stencilWriteMask, pDS)) {
              break;
            }
          }
        else {
          // Early out depth test
          if (TCompare<DepthFunc>(depthValue,
                                  *reinterpret_cast<const uint16_t *>(pDS))) {
            break;
          }
        }

        pDS += DepthStencilStride;

        if
          constexpr(InterpolateDepth) { fZ += fZdA; }

        --count;
      } while (count);

      if (0 == count) {
        break;
      }

      const int offset = start - count;
      start = count;

      do {
        unsigned depthValue = 0;

        if
          constexpr(DepthTest) {
            depthValue = static_cast<fixed16>(fZ).GetRaw() & 0xFFFF;
          }

        if
          constexpr(StencilTest) {
            // Execute the depth stencil test
            if (!TStencilTest<DepthTest, DepthFunc, DepthWrite, StencilFunc,
                              StencilPass, STENCIL_KEEP, STENCIL_KEEP,
                              StencilWrite>(depthValue, stencilRef, stencilMask,
                                            stencilWriteMask, pDS)) {
              break;
            }
          }
        else {
          // Early out depth test
          if (!TCompare<DepthFunc>(depthValue,
                                   *reinterpret_cast<const uint16_t *>(pDS))) {
            break;
          }

          if
            constexpr(DepthTest && DepthWrite) {
              // Apply DepthWrite
              *reinterpret_cast<uint16_t *>(pDS) =
                  static_cast<uint16_t>(depthValue);
            }
        }

        pDS += DepthStencilStride;

        if
          constexpr(InterpolateDepth) { fZ += fZdA; }

        --count;
      } while (count);

      int width = start - count;

      //--

      pCB += ColorStride * offset;
      uint8_t *const pCBEnd = pCB + width * ColorStride;

      if
        constexpr(InterpolateColor) {
          fB += fBdA * offset;
          fG += fGdA * offset;
          fR += fRdA * offset;
        }

      if
        constexpr(InterpolateAlpha) { fA += fAdA * offset; }

      if
        constexpr(Texture0) {
          fU0 += fU0dA * offset;
          fV0 += fV0dA * offset;
        }

      if
        constexpr(Texture1) {
          fU1 += fU1dA * offset;
          fV1 += fV1dA * offset;
        }

      if
        constexpr(InterpolateFog) { fFog += fFogdA * offset; }

      do {
        if
          constexpr(Color) {
            if
              constexpr(NativeColor) { cColor = cNativeColor; }
            else {
              ToColor4(&cColor, fA, fR, fG, fB);
            }
          }

        if
          constexpr(Texture0) {
            unsigned texture;

            if
              constexpr(Mip0) {
                texture =
                    TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                     Tex0_MagFilter, Tex0_Format, Tex0_AddressS,
                                     Tex0_AddressT>(*sampler0, fU0, fV0, fM0);
              }
            else {
              texture =
                  TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                   Tex0_MagFilter, Tex0_Format, Tex0_AddressS,
                                   Tex0_AddressT>(surface0, fU0, fV0);
            }

            TGetTexEnvColor<Tex0_EnvMode, Tex0_Format, NativeColor>(
                &cColor, texture, cEnvColor0);

            if
              constexpr(Texture1) {
                if
                  constexpr(Mip1) {
                    texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                               Tex1_MagFilter, Tex1_Format,
                                               Tex1_AddressS, Tex1_AddressT>(
                        *sampler1, fU1, fV1, fM1);
                  }
                else {
                  texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                             Tex1_MagFilter, Tex1_Format,
                                             Tex1_AddressS, Tex1_AddressT>(
                      surface1, fU1, fV1);
                }

                TGetTexEnvColor<Tex1_EnvMode, Tex1_Format, NativeColor>(
                    &cColor, texture, cEnvColor1);
              }
          }

        if
          constexpr(Fog) {
            // Execute fog blend
            TCalcFog<ColorFormat, NativeColor>(&cColor, cFogColor, fFog);
          }

        unsigned color;

        const unsigned dstColor =
            *reinterpret_cast<typename TFormatInfo<ColorFormat>::TYPE *>(pCB);

        if
          constexpr(Blend) {
            // Execute pixel blend
            color =
                TBlend<ColorFormat, BlendSrc, BlendDst, NativeColor>::Execute(
                    cColor, dstColor);
          }
        else {
          if
            constexpr(NativeColor) { color = cColor.b; }
          else {
            // Convert color to native
            color = Format::TConvertToNative<ColorFormat>(cColor);
          }
        }

        // Write output color
        TWriteColor<ColorFormat, LogicOp ? LogicFunc : LOGICOP_COPY,
                    ColorWriteMask>(color, dstColor, colorWriteMask, pCB);

        if
          constexpr(InterpolateColor) {
            fB += fBdA;
            fG += fGdA;
            fR += fRdA;
          }

        if
          constexpr(InterpolateAlpha) { fA += fAdA; }

        if
          constexpr(Texture0) {
            fU0 += fU0dA;
            fV0 += fV0dA;
          }

        if
          constexpr(Texture1) {
            fU1 += fU1dA;
            fV1 += fV1dA;
          }

        if
          constexpr(InterpolateFog) { fFog += fFogdA; }

        pCB += ColorStride;
      } while (pCB < pCBEnd);
    } while (count);
  }
};

template <unsigned Flags, unsigned States, unsigned Texture0, unsigned Texture1>
class TOptimizedScanlinePZ {
public:
  enum {
    ColorFormat = __get_bitfield(Flags, RASTERFLAGS::COLORFORMAT),
    DepthStencilFormat = __get_bitfield(Flags, RASTERFLAGS::DEPTHSTENCILFORMAT),
    DepthTest = __get_bitfield(Flags, RASTERFLAGS::DEPTHTEST),
    DepthWrite = __get_bitfield(Flags, RASTERFLAGS::DEPTHWRITE),
    StencilTest = __get_bitfield(Flags, RASTERFLAGS::STENCILTEST),
    StencilWrite = __get_bitfield(Flags, RASTERFLAGS::STENCILWRITE),
    Color = __get_bitfield(Flags, RASTERFLAGS::COLOR),
    Fog = __get_bitfield(Flags, RASTERFLAGS::FOG),
    NumTextures = __get_bitfield(Flags, RASTERFLAGS::NUMTEXTURES),
    TextureMips = __get_bitfield(Flags, RASTERFLAGS::TEXTUREMIPS),
    AlphaTest = __get_bitfield(Flags, RASTERFLAGS::ALPHATEST),
    Blend = __get_bitfield(Flags, RASTERFLAGS::BLEND),
    LogicOp = __get_bitfield(Flags, RASTERFLAGS::LOGICOP),
    ColorWriteMask = __get_bitfield(Flags, RASTERFLAGS::COLORWRITEMASK),
    InterpolateDepth = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEDEPTH),
    InterpolateColor = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATECOLOR),
    InterpolateAlpha = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEALPHA),
    InterpolateFog = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEFOG),
    InterpolateMips = __get_bitfield(Flags, RASTERFLAGS::INTERPOLATEMIPS),

    DepthFunc = __get_bitfield(States, RASTERSTATES::DEPTHFUNC),
    StencilFunc = __get_bitfield(States, RASTERSTATES::STENCILFUNC),
    StencilPass = __get_bitfield(States, RASTERSTATES::STENCILPASS),
    StencilFail = __get_bitfield(States, RASTERSTATES::STENCILFAIL),
    StencilZFail = __get_bitfield(States, RASTERSTATES::STENCILZFAIL),
    AlphaFunc = __get_bitfield(States, RASTERSTATES::ALPHAFUNC),
    BlendSrc = __get_bitfield(States, RASTERSTATES::BLENDSRC),
    BlendDst = __get_bitfield(States, RASTERSTATES::BLENDDST),
    LogicFunc = __get_bitfield(States, RASTERSTATES::LOGICFUNC),

    Tex0_Format = __get_bitfield(Texture0, TEXTURESTATES::FORMAT),
    Tex0_AddressS = __get_bitfield(Texture0, TEXTURESTATES::ADDRESSS),
    Tex0_AddressT = __get_bitfield(Texture0, TEXTURESTATES::ADDRESST),
    Tex0_MipFilter = __get_bitfield(Texture0, TEXTURESTATES::MIPFILTER),
    Tex0_MinFilter = __get_bitfield(Texture0, TEXTURESTATES::MINFILTER),
    Tex0_MagFilter = __get_bitfield(Texture0, TEXTURESTATES::MAGFILTER),
    Tex0_EnvMode = __get_bitfield(Texture0, TEXTURESTATES::ENVMODE),

    Tex1_Format = __get_bitfield(Texture1, TEXTURESTATES::FORMAT),
    Tex1_AddressS = __get_bitfield(Texture1, TEXTURESTATES::ADDRESSS),
    Tex1_AddressT = __get_bitfield(Texture1, TEXTURESTATES::ADDRESST),
    Tex1_MipFilter = __get_bitfield(Texture1, TEXTURESTATES::MIPFILTER),
    Tex1_MinFilter = __get_bitfield(Texture1, TEXTURESTATES::MINFILTER),
    Tex1_MagFilter = __get_bitfield(Texture1, TEXTURESTATES::MAGFILTER),
    Tex1_EnvMode = __get_bitfield(Texture1, TEXTURESTATES::ENVMODE),

    ColorStride = TFormatInfo<ColorFormat>::CBSIZE,
    DepthStencilStride = TFormatInfo<DepthStencilFormat>::CBSIZE,

    Mip0 = ((TextureMips >> 0) & 0x1),
    Mip1 = ((TextureMips >> 1) & 0x1),

    DEPTH_SIZE = DepthTest ? 1 : 0,
    COLOR_SIZE = Color ? 4 : 0,
    RHW_SIZE = 1,
    TEX0_SIZE = Texture0 ? 2 : 0,
    TEX1_SIZE = Texture1 ? 2 : 0,
    MIP0_SIZE = Mip0 ? 1 : 0,
    MIP1_SIZE = Mip1 ? 1 : 0,
    FOG_SIZE = Fog ? 1 : 0,

    REG_DEPTH = 0,
    REG_COLOR = REG_DEPTH + DEPTH_SIZE,
    REG_RHW = REG_COLOR + COLOR_SIZE,
    REG_TEX0 = REG_RHW + RHW_SIZE,
    REG_TEX1 = REG_TEX0 + TEX0_SIZE,
    REG_MIP0 = REG_TEX1 + TEX1_SIZE,
    REG_MIP1 = REG_MIP0 + MIP0_SIZE,
    REG_FOG = REG_MIP1 + MIP1_SIZE,
    REG_SIZE = REG_FOG + FOG_SIZE,

    NativeColor = (((0 == NumTextures) && (0 == InterpolateColor) &&
                    (0 == InterpolateAlpha)) ||
                   ((1 == NumTextures) && (ENVMODE_REPLACE == Tex0_EnvMode) &&
                    (ColorFormat == Tex0_Format)))
                      ? 1
                      : 0,
  };

  inline static void Execute(const RasterData &rasterData, int y, int lx,
                             int rx) {
    const Sampler *sampler0, *sampler1;
    ColorARGB cEnvColor0, cEnvColor1;
    SurfaceDesc surface0, surface1;

    if
      constexpr(Texture0) {
        sampler0 = &rasterData.Samplers[0];
        cEnvColor0 = sampler0->cEnvColor_MaxMipLevel;

        if
          constexpr(!Mip0) { surface0 = sampler0->pMipLevels[0]; }

        if
          constexpr(Texture1) {
            sampler1 = &rasterData.Samplers[1];
            cEnvColor1 = sampler1->cEnvColor_MaxMipLevel;

            if
              constexpr(!Mip1) { surface1 = sampler1->pMipLevels[0]; }
          }
      }

    uint16_t stencilRef;
    uint16_t stencilMask;
    uint16_t stencilWriteMask;

    if
      constexpr(StencilTest) {
        stencilRef = rasterData.StencilRef;
        stencilMask = rasterData.StencilMask;
        stencilWriteMask = rasterData.StencilWriteMask;
      }

    ColorARGB cFogColor;

    if
      constexpr(Fog) {
        if
          constexpr(NativeColor) {
            cFogColor =
                Format::TConvertToNative<ColorFormat>(rasterData.cFogColor);
          }
        else {
          cFogColor = rasterData.cFogColor;
        }
      }

    const fixed4 fOffsetX = fixed4(lx) - rasterData.fRefX;
    const fixed4 fOffsetY = fixed4(y) - rasterData.fRefY;

    fixedRX fZ, fZdA;

    if
      constexpr(DepthTest) {
        fZ = rasterData.Registers[REG_DEPTH].m[2];

        if
          constexpr(InterpolateDepth) {
            fZdA = rasterData.Registers[REG_DEPTH].m[0];
            const fixedRX fZdB = rasterData.Registers[REG_DEPTH].m[1];
            fZ += fZdA * fOffsetX + fZdB * fOffsetY;
          }
      }

    fixedRX fB, fG, fR, fA;
    fixedRX fBdA, fGdA, fRdA, fAdA;
    Color4 cNativeColor;

    if
      constexpr(Color) {
        fB = rasterData.Registers[REG_COLOR + 0].m[2];
        fG = rasterData.Registers[REG_COLOR + 1].m[2];
        fR = rasterData.Registers[REG_COLOR + 2].m[2];
        fA = rasterData.Registers[REG_COLOR + 3].m[2];

        if
          constexpr(InterpolateColor) {
            fBdA = rasterData.Registers[REG_COLOR + 0].m[0];
            const fixedRX fBdB = rasterData.Registers[REG_COLOR + 0].m[1];
            fB += fBdA * fOffsetX + fBdB * fOffsetY;

            fGdA = rasterData.Registers[REG_COLOR + 1].m[0];
            const fixedRX fGdB = rasterData.Registers[REG_COLOR + 1].m[1];
            fG += fGdA * fOffsetX + fGdB * fOffsetY;

            fRdA = rasterData.Registers[REG_COLOR + 2].m[0];
            const fixedRX fRdB = rasterData.Registers[REG_COLOR + 2].m[1];
            fR += fRdA * fOffsetX + fRdB * fOffsetY;
          }

        if
          constexpr(InterpolateAlpha) {
            fAdA = rasterData.Registers[REG_COLOR + 3].m[0];
            const fixedRX fAdB = rasterData.Registers[REG_COLOR + 3].m[1];
            fA += fAdA * fOffsetX + fAdB * fOffsetY;
          }

        if
          constexpr(NativeColor) {
            ToColor4(&cNativeColor, fA, fR, fG, fB);
            cNativeColor.b =
                Format::TConvertToNative<ColorFormat>(cNativeColor);
          }
      }

    fixedRX fRhw, fRhwdA;
    fixedRX fU0OverW, fV0OverW, fU1OverW, fV1OverW;
    fixedRX fU0OverWdA, fV0OverWdA, fU1OverWdA, fV1OverWdA;
    fixedRX fM0OverW2, fM1OverW2;
    fixedRX fM0OverW2dA, fM1OverW2dA;

    if
      constexpr(Texture0) {
        const fixedRX fRhwdA = rasterData.Registers[REG_RHW].m[0];
        const fixedRX fRhwdB = rasterData.Registers[REG_RHW].m[1];
        const fixedRX fRhwdC = rasterData.Registers[REG_RHW].m[2];
        fRhw = fRhwdA * fOffsetX + fRhwdB * fOffsetY + fRhwdC;

        fU0OverWdA = rasterData.Registers[REG_TEX0 + 0].m[0];
        const fixedRX fU0OverWdB = rasterData.Registers[REG_TEX0 + 0].m[1];
        const fixedRX fU0OverWdC = rasterData.Registers[REG_TEX0 + 0].m[2];
        fU0OverW = fU0OverWdA * fOffsetX + fU0OverWdB * fOffsetY + fU0OverWdC;

        fV0OverWdA = rasterData.Registers[REG_TEX0 + 1].m[0];
        const fixedRX fV0OverWdB = rasterData.Registers[REG_TEX0 + 1].m[1];
        const fixedRX fV0OverWdC = rasterData.Registers[REG_TEX0 + 1].m[2];
        fV0OverW = fV0OverWdA * fOffsetX + fV0OverWdB * fOffsetY + fV0OverWdC;

        if
          constexpr(Mip0) {
            fM0OverW2 = rasterData.Registers[REG_MIP0].m[2];

            if
              constexpr(InterpolateMips & 0x1) {
                fM0OverW2dA = rasterData.Registers[REG_MIP0].m[0];
                const fixedRX fM0OverW2dB =
                    rasterData.Registers[REG_MIP0].m[1];
                fM0OverW2 += fM0OverW2dA * fOffsetX + fM0OverW2dB * fOffsetY;
              }
          }

        if
          constexpr(Texture1) {
            fU1OverWdA = rasterData.Registers[REG_TEX1 + 0].m[0];
            const fixedRX fU1OverWdB = rasterData.Registers[REG_TEX1 + 0].m[1];
            const fixedRX fU1OverWdC = rasterData.Registers[REG_TEX1 + 0].m[2];
            fixedRX fU1OverW =
                fU1OverWdA * fOffsetX + fU1OverWdB * fOffsetY + fU1OverWdC;

            fV1OverWdA = rasterData.Registers[REG_TEX1 + 1].m[0];
            const fixedRX fV1OverWdB = rasterData.Registers[REG_TEX1 + 1].m[1];
            const fixedRX fV1OverWdC = rasterData.Registers[REG_TEX1 + 1].m[2];
            fixedRX fV1OverW =
                fV1OverWdA * fOffsetX + fV1OverWdB * fOffsetY + fV1OverWdC;

            if
              constexpr(Mip1) {
                fM1OverW2 = rasterData.Registers[REG_MIP1].m[2];

                if
                  constexpr(InterpolateMips & 0x2) {
                    fM1OverW2dA = rasterData.Registers[REG_MIP1].m[0];
                    const fixedRX fM1OverW2dB =
                        rasterData.Registers[REG_MIP1].m[1];
                    fM1OverW2 +=
                        fM1OverW2dA * fOffsetX + fM1OverW2dB * fOffsetY;
                  }
              }
          }
      }

    fixedRX fFog, fFogdA;

    if
      constexpr(Fog) {
        fFog = rasterData.Registers[REG_FOG + 0].m[2];

        if
          constexpr(InterpolateFog) {
            fFogdA = rasterData.Registers[REG_FOG].m[0];
            const fixedRX fFogdB = rasterData.Registers[REG_FOG].m[1];
            fFog += fFogdA * fOffsetX + fFogdB * fOffsetY;
          }
      }

    const unsigned colorWriteMask = rasterData.ColorWriteMask;
    uint8_t *const pColorBits = rasterData.pColorBits;
    const unsigned colorPitch = rasterData.ColorPitch;
    uint8_t *pCB = lx * ColorStride + y * colorPitch + pColorBits;

    uint8_t *const pDepthStencilBits = rasterData.pDepthStencilBits;
    const unsigned depthStencilPitch = rasterData.DepthStencilPitch;
    uint8_t *pDS =
        lx * DepthStencilStride + y * depthStencilPitch + pDepthStencilBits;

    Color4 cColor(0xff, 0xff, 0xff, 0xff);

    int count = rx - lx;

    do {
      int start = count;

      do {
        unsigned depthValue = 0;

        if
          constexpr(DepthTest) {
            depthValue = static_cast<fixed16>(fZ).GetRaw() & 0xFFFF;
          }

        if
          constexpr(StencilTest) {
            // Execute the depth stencil test
            if (TStencilTest<DepthTest, DepthFunc, false, StencilFunc,
                             STENCIL_KEEP, StencilFail, StencilZFail,
                             StencilWrite>(depthValue, stencilRef, stencilMask,
                                           stencilWriteMask, pDS)) {
              break;
            }
          }
        else {
          // Early out if depth test passes
          if (TCompare<DepthFunc>(depthValue,
                                  *reinterpret_cast<const uint16_t *>(pDS))) {
            break;
          }
        }

        pDS += DepthStencilStride;

        if
          constexpr(InterpolateDepth) { fZ += fZdA; }

        --count;
      } while (count);

      if (0 == count) {
        break;
      }

      const int offset = start - count;
      start = count;

      do {
        unsigned depthValue = 0;

        if
          constexpr(DepthTest) {
            depthValue = static_cast<fixed16>(fZ).GetRaw() & 0xFFFF;
          }

        if
          constexpr(StencilTest) {
            // Execute the depth stencil test
            if (!TStencilTest<DepthTest, DepthFunc, DepthWrite, StencilFunc,
                              StencilPass, STENCIL_KEEP, STENCIL_KEEP,
                              StencilWrite>(depthValue, stencilRef, stencilMask,
                                            stencilWriteMask, pDS)) {
              break;
            }
          }
        else {
          // Early out depth test
          if (!TCompare<DepthFunc>(depthValue,
                                   *reinterpret_cast<const uint16_t *>(pDS))) {
            break;
          }

          if
            constexpr(DepthTest && DepthWrite) {
              // Apply DepthWrite
              *reinterpret_cast<uint16_t *>(pDS) =
                  static_cast<uint16_t>(depthValue);
            }
        }

        pDS += DepthStencilStride;

        if
          constexpr(InterpolateDepth) { fZ += fZdA; }

        --count;
      } while (count);

      int width = start - count;

      pCB += offset * ColorStride;

      if
        constexpr(InterpolateColor) {
          fB += fBdA * offset;
          fG += fGdA * offset;
          fR += fRdA * offset;
        }

      if
        constexpr(InterpolateAlpha) { fA += fAdA * offset; }

      fRhw += fRhwdA * offset;

      const fixedW fW = Math::TInv<fixedW>(fRhw);
      fixedW fW2;

      if
        constexpr(Mip0 || Mip1) { fW2 = fW * fW; }

      fixedRX fU0, fV0, fU1, fV1;
      fixedRX fM0, fM1;

      if
        constexpr(Texture0) {
          fU0OverW += fU0OverWdA * offset;
          fV0OverW += fV0OverWdA * offset;

          fU0 = fU0OverW * fW;
          fV0 = fV0OverW * fW;

          if
            constexpr(Mip0) {
              if
                constexpr(InterpolateMips & 0x1) {
                  fM0OverW2 += fM0OverW2dA * offset;
                  fM0 = fM0OverW2 * fW2;
                }
              else {
                fM0 = fM0OverW2;
              }
            }

          if
            constexpr(Texture1) {
              fU1OverW += fU1OverWdA * offset;
              fV1OverW += fV1OverWdA * offset;

              fU1 = fU1OverW * fW;
              fV1 = fV1OverW * fW;

              if
                constexpr(Mip1) {
                  if
                    constexpr(InterpolateMips & 0x2) {
                      fM1OverW2 += fM1OverW2dA * offset;
                      fM1 = fM1OverW2 * fW2;
                    }
                  else {
                    fM1 = fM1OverW2;
                  }
                }
            }
        }

      if
        constexpr(InterpolateFog) { fFog += fFogdA * offset; }

      do {
        const unsigned int blockWidth1 = Math::TMin(width, MAX_BLOCK_SIZE);
        const unsigned int log2width = Math::iLog2(blockWidth1);
        const unsigned int blockWidth = 1 << log2width;
        width -= blockWidth;
        uint8_t *const pCBEnd = pCB + blockWidth * ColorStride;

        fRhw += fRhwdA << log2width;
        const fixedW fWr = Math::TInv<fixedW>(fRhw);
        fixedW fWr2;

        if
          constexpr(InterpolateMips) { fWr2 = fWr * fWr; }

        fixedRX fdUr0, fdVr0, fdUr1, fdVr1;
        fixedRX fdMr0, fdMr1;

        if
          constexpr(Texture0) {
            fU0OverW += fU0OverWdA << log2width;
            fV0OverW += fV0OverWdA << log2width;
            fdUr0 = (fU0OverW * fWr - fU0) >> log2width;
            fdVr0 = (fV0OverW * fWr - fV0) >> log2width;

            if
              constexpr(InterpolateMips & 0x1) {
                fM0OverW2 += fM0OverW2dA << log2width;
                fdMr0 = (fM0OverW2 * fWr2 - fM0) >> log2width;
              }

            if
              constexpr(Texture1) {
                fU1OverW += fU1OverWdA << log2width;
                fV1OverW += fV1OverWdA << log2width;
                fdUr1 = (fU1OverW * fWr - fU1) >> log2width;
                fdVr1 = (fV1OverW * fWr - fV1) >> log2width;

                if
                  constexpr(InterpolateMips & 0x2) {
                    fM1OverW2 += fM1OverW2dA << log2width;
                    fdMr1 = (fM1OverW2 * fWr2 - fM1) >> log2width;
                  }
              }
          }

        do {
          if
            constexpr(Color) {
              if
                constexpr(NativeColor) { cColor = cNativeColor; }
              else {
                ToColor4(&cColor, fA, fR, fG, fB);
              }
            }

          // Execute texture operations
          if
            constexpr(Texture0) {
              unsigned texture;

              if
                constexpr(Mip0) {
                  texture = TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                             Tex0_MagFilter, Tex0_Format,
                                             Tex0_AddressS, Tex0_AddressT>(
                      sampler0, fU0, fV0, fM0);
                }
              else {
                texture =
                    TGetSamplerColor<Tex0_MipFilter, Tex0_MinFilter,
                                     Tex0_MagFilter, Tex0_Format, Tex0_AddressS,
                                     Tex0_AddressT>(surface0, fU0, fV0);
              }

              TGetTexEnvColor<Tex0_EnvMode, Tex0_Format, NativeColor>(
                  &cColor, texture, cEnvColor0);

              if
                constexpr(Texture1) {
                  if
                    constexpr(Mip1) {
                      texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                                 Tex1_MagFilter, Tex1_Format,
                                                 Tex1_AddressS, Tex1_AddressT>(
                          sampler1, fU1, fV1, fM1);
                    }
                  else {
                    texture = TGetSamplerColor<Tex1_MipFilter, Tex1_MinFilter,
                                               Tex1_MagFilter, Tex1_Format,
                                               Tex1_AddressS, Tex1_AddressT>(
                        surface1, fU1, fV1);
                  }

                  TGetTexEnvColor<Tex1_EnvMode, Tex1_Format, NativeColor>(
                      &cColor, texture, cEnvColor1);
                }
            }

          if
            constexpr(Fog) {
              // Execute fog blend
              TCalcFog<ColorFormat, NativeColor>(&cColor, cFogColor, fFog);
            }

          unsigned color;

          const unsigned dstColor =
              *reinterpret_cast<typename TFormatInfo<ColorFormat>::TYPE *>(pCB);

          if
            constexpr(Blend) {
              // Execute pixel blend
              color =
                  TBlend<ColorFormat, BlendSrc, BlendDst, NativeColor>::Execute(
                      cColor, dstColor);
            }
          else {
            if
              constexpr(NativeColor) { color = cColor.b; }
            else {
              // Convert color to native
              color = Format::TConvertToNative<ColorFormat>(cColor);
            }
          }

          // Write output color
          TWriteColor<ColorFormat, LogicOp ? LogicFunc : LOGICOP_COPY,
                      ColorWriteMask>(color, dstColor, colorWriteMask, pCB);

          if
            constexpr(InterpolateColor) {
              fB += fBdA;
              fG += fGdA;
              fR += fRdA;
            }

          if
            constexpr(InterpolateAlpha) { fA += fAdA; }

          if
            constexpr(Texture0) {
              fU0 += fdUr0;
              fV0 += fdVr0;

              if
                constexpr(InterpolateMips & 0x1) { fM0 += fdMr0; }

              if
                constexpr(Texture1) {
                  fU1 += fdUr1;
                  fV1 += fdVr1;

                  if
                    constexpr(InterpolateMips & 0x2) { fM1 += fdMr1; }
                }
            }

          if
            constexpr(InterpolateFog) { fFog += fFogdA; }

          pCB += ColorStride;
        } while (pCB < pCBEnd);
      } while (width);
    } while (count);
  }
};

template <unsigned Flags, unsigned States, unsigned Texture0, unsigned Texture1>
class TOptimizedScanline {
private:
  enum {
    Perspective = __get_bitfield(Flags, RASTERFLAGS::PERSPECTIVE),
    AlphaTest = __get_bitfield(Flags, RASTERFLAGS::ALPHATEST),
    DepthTest = __get_bitfield(Flags, RASTERFLAGS::DEPTHTEST),
    StencilTest = __get_bitfield(Flags, RASTERFLAGS::STENCILTEST),
  };

public:
  static void Execute(const RasterData &rasterData, int y, int lx, int rx) {
    if
      constexpr(Perspective) {
        if
          constexpr(Texture0) {
            if
              constexpr(!AlphaTest && (DepthTest || StencilTest)) {
                TOptimizedScanlinePZ<Flags, States, Texture0,
                                     Texture1>::Execute(rasterData, y, lx, rx);
              }
            else {
              TOptimizedScanlineP<Flags, States, Texture0, Texture1>::Execute(
                  rasterData, y, lx, rx);
            }
          }
        else {
          __unreferenced(rasterData);
          __unreferenced(y);
          __unreferenced(lx);
          __unreferenced(rx);
        }
      }
    else {
      if
        constexpr(!AlphaTest && (DepthTest || StencilTest)) {
          TOptimizedScanlineAZ<Flags, States, Texture0, Texture1>::Execute(
              rasterData, y, lx, rx);
        }
      else {
        TOptimizedScanlineA<Flags, States, Texture0, Texture1>::Execute(
            rasterData, y, lx, rx);
      }
    }
  }
};