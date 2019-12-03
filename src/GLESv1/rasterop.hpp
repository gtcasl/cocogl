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

template <uint32_t Format> struct TColorNative {
  uint32_t Low;
  uint32_t High;

  TColorNative() {}

  explicit TColorNative(uint32_t value) {
    if constexpr (Format == FORMAT_A8) {
      this->Low = value;
    }

    if constexpr (Format == FORMAT_L8) {
      this->Low = value;
    }

    if constexpr (Format == FORMAT_A8L8) {
      this->Low = (value | (value << 8)) & 0x00ff00ff;
    }

    if constexpr (Format == FORMAT_R5G6B5) {
      this->Low = (value | (value << 16)) & 0x07e0f81f;
    }

    if constexpr (Format == FORMAT_A4R4G4B4) {
      this->Low = (value | (value << 12)) & 0x0f0f0f0f;
    }

    if constexpr (Format == FORMAT_A8R8G8B8) {
      this->Low = value & 0x00ff00ff;
      this->High = (value >> 8) & 0x00ff00ff;
    }
  }

  operator uint32_t() const {
    if constexpr (Format == FORMAT_A8) {
      return this->Low;
    }

    if constexpr (Format == FORMAT_L8) {
      return this->Low;
    }

    if constexpr (Format == FORMAT_A8L8) {
      return (uint16_t)(this->Low | (this->Low >> 8));
    }

    if constexpr (Format == FORMAT_R5G6B5) {
      return (uint16_t)(this->Low | (this->Low >> 16));
    }

    if constexpr (Format == FORMAT_A4R4G4B4) {
      return (uint16_t)(this->Low | (this->Low >> 12));
    }

    if constexpr (Format == FORMAT_A8R8G8B8) {
      return this->Low | (this->High << 8);
    }
  }

  TColorNative<Format> multiply(uint32_t frac) const {
    TColorNative<Format> result;

    if constexpr (Format == FORMAT_A8) {
      result.Low = (this->Low * frac) >> 8;
    }

    if constexpr (Format == FORMAT_L8) {
      result.Low = (this->Low * frac) >> 8;
    }

    if constexpr (Format == FORMAT_A8L8) {
      result.Low = ((this->Low * frac) >> 8) & 0x00ff00ff;
    }

    if constexpr (Format == FORMAT_R5G6B5) {
      result.Low = ((this->Low * frac) >> 5) & 0x07e0f81f;
    }

    if constexpr (Format == FORMAT_A4R4G4B4) {
      result.Low = ((this->Low * frac) >> 4) & 0x0f0f0f0f;
    }

    if constexpr (Format == FORMAT_A8R8G8B8) {
      result.Low = ((this->Low * frac) >> 8) & 0x00ff00ff;
      result.High = ((this->High * frac) >> 8) & 0x00ff00ff;
    }

    return result;
  }

  TColorNative<Format> lerp(const TColorNative<Format> &c1,
                            uint32_t frac) const {
    TColorNative<Format> result;

    if constexpr (Format == FORMAT_A8) {
      result.Low = this->Low + (((c1.Low - this->Low) * frac) >> 8);
    }

    if constexpr (Format == FORMAT_L8) {
      result.Low = this->Low + (((c1.Low - this->Low) * frac) >> 8);
    }

    if constexpr (Format == FORMAT_A8L8) {
      result.Low =
          (this->Low + (((c1.Low - this->Low) * frac) >> 8)) & 0x00ff00ff;
    }

    if constexpr (Format == FORMAT_R5G6B5) {
      result.Low =
          (this->Low + (((c1.Low - this->Low) * frac) >> 5)) & 0x07e0f81f;
    }

    if constexpr (Format == FORMAT_A4R4G4B4) {
      result.Low =
          (this->Low + (((c1.Low - this->Low) * frac) >> 4)) & 0x0f0f0f0f;
    }

    if constexpr (Format == FORMAT_A8R8G8B8) {
      result.Low =
          (this->Low + (((c1.Low - this->Low) * frac) >> 8)) & 0x00ff00ff;
      result.High =
          (this->High + (((c1.High - this->High) * frac) >> 8)) & 0x00ff00ff;
    }

    return result;
  }
};

template <uint32_t compare> inline bool DoCompare(uint32_t a, uint32_t b) {

  if constexpr (compare == COMPARE_NEVER) {
    __unreferenced(a);
    __unreferenced(b);
    return false;
  }

  if constexpr (compare == COMPARE_LESS) {
    return (a < b);
  }

  if constexpr (compare == COMPARE_EQUAL) {
    return (a == b);
  }

  if constexpr (compare == COMPARE_LEQUAL) {
    return (a <= b);
  }

  if constexpr (compare == COMPARE_GREATER) {
    return (a > b);
  }

  if constexpr (compare == COMPARE_NOTEQUAL) {
    return (a != b);
  }

  if constexpr (compare == COMPARE_GEQUAL) {
    return (a >= b);
  }

  if constexpr (compare == COMPARE_ALWAYS) {
    __unreferenced(a);
    __unreferenced(b);
    return true;
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t StencilOp>
inline uint32_t TStencilOp(uint32_t stencilValue, uint32_t stencilRef) {

  if constexpr (StencilOp == STENCIL_KEEP) {
    __unreferenced(stencilRef);
    return stencilValue;
  }

  if constexpr (StencilOp == STENCIL_REPLACE) {
    __unreferenced(stencilValue);
    return stencilRef;
  }

  if constexpr (StencilOp == STENCIL_INCR) {
    __unreferenced(stencilRef);
    return stencilValue + (stencilValue != 0xffff) ? 1 : 0;
  }

  if constexpr (StencilOp == STENCIL_DECR) {
    __unreferenced(stencilRef);
    return stencilValue - (stencilValue != 0) ? 1 : 0;
  }

  if constexpr (StencilOp == STENCIL_ZERO) {
    __unreferenced(stencilValue);
    __unreferenced(stencilRef);
    return 0;
  }

  if constexpr (StencilOp == STENCIL_INVERT) {
    __unreferenced(stencilRef);
    return ~stencilValue;
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Address> inline int TAddress(int x) {
  if constexpr (Address == ADDRESS_WRAP) {
    return x & fixedRX::MASK;
  }
  if constexpr (Address == ADDRESS_CLAMP) {
    return Math::TClamp<int>(x, 0, fixedRX::MASK);
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline uint32_t GetTexelColorPtN(const SurfaceDesc &surface, fixedRX fU,
                                 fixedRX fV) {
  auto pBits = reinterpret_cast<const typename TFormatInfo<Format>::TYPE *>(
      surface.getBits());
  uint32_t logWidth = surface.getLogWidth();
  uint32_t logHeight = surface.getLogHeight();

  uint32_t u = TAddress<AddressU>(fU.data());
  uint32_t v = TAddress<AddressV>(fV.data());

  uint32_t x = u >> (fixedRX::FRAC - logWidth);
  uint32_t y = v >> (fixedRX::FRAC - logHeight);
  uint32_t offset = x + (y << logWidth);

  return *(pBits + offset);
}

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline TColorNative<Format> GetTexelColorLnX(const SurfaceDesc &surface,
                                             fixedRX fU, fixedRX fV) {
  int lerpBits = TFormatInfo<Format>::LERP;
  int lerpMask = (1 << lerpBits) - 1;

  auto pBits = reinterpret_cast<const typename TFormatInfo<Format>::TYPE *>(
      surface.getBits());
  uint32_t logWidth = surface.getLogWidth();
  uint32_t logHeight = surface.getLogHeight();

  uint32_t v0 = TAddress<AddressV>(fV.data() - (fixedRX::HALF >> logHeight));
  uint32_t v1 = TAddress<AddressV>(fV.data() + (fixedRX::HALF >> logHeight));
  uint32_t u0 = TAddress<AddressU>(fU.data() - (fixedRX::HALF >> logWidth));
  uint32_t u1 = TAddress<AddressU>(fU.data() + (fixedRX::HALF >> logWidth));

  uint32_t logHeightN = fixedRX::FRAC - (logHeight + lerpBits);
  uint32_t y1 = v1 >> logHeightN;
  uint32_t y0 = v0 >> logHeightN;
  uint32_t beta = y0 & lerpMask;
  uint32_t y3 = (y1 >> lerpBits) << logWidth;
  uint32_t y2 = (y0 >> lerpBits) << logWidth;

  uint32_t logWidthN = fixedRX::FRAC - (logWidth + lerpBits);
  uint32_t x1 = u1 >> logWidthN;
  uint32_t x0 = u0 >> logWidthN;
  uint32_t alpha = x0 & lerpMask;
  uint32_t x3 = x1 >> lerpBits;
  uint32_t x2 = x0 >> lerpBits;

  uint32_t c0 = pBits[y2 + x2];
  uint32_t c1 = pBits[y2 + x3];
  uint32_t c2 = pBits[y3 + x2];
  uint32_t c3 = pBits[y3 + x3];

  TColorNative<Format> nc0(c0);
  TColorNative<Format> nc1(c1);
  nc0 = nc0.lerp(nc1, alpha);

  TColorNative<Format> nc2(c2);
  TColorNative<Format> nc3(c3);
  nc2 = nc2.lerp(nc3, alpha);

  return nc0.lerp(nc2, beta);
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Filter, uint32_t Format, uint32_t AddressU,
          uint32_t AddressV>
inline uint32_t GetMinFilterN(const SurfaceDesc &surface, fixedRX fU,
                              fixedRX fV) {
  if constexpr (Filter == FILTER_NEAREST) {
    return GetTexelColorPtN<Format, AddressU, AddressV>(surface, fU, fV);
  }

  if constexpr (Filter == FILTER_LINEAR) {
    return GetTexelColorLnX<Format, AddressU, AddressV>(surface, fU, fV);
  }
}

template <uint32_t Filter, uint32_t Format, uint32_t AddressU,
          uint32_t AddressV>
inline TColorNative<Format> GetMinFilterX(const SurfaceDesc &surface,
                                          fixedRX fU, fixedRX fV) {
  if constexpr (Filter == FILTER_NEAREST) {
    return TColorNative<Format>(
        GetTexelColorPtN<Format, AddressU, AddressV>(surface, fU, fV));
  }

  if constexpr (Filter == FILTER_LINEAR) {
    return GetTexelColorLnX<Format, AddressU, AddressV>(surface, fU, fV);
  }
}

template <uint32_t MipFilter, uint32_t MinFilter, uint32_t MagFilter,
          uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline uint32_t GetMipFilterN(const Sampler &sampler, fixedRX fU, fixedRX fV,
                              fixedRX fM) {
  if constexpr (MipFilter == FILTER_NONE) {
    if constexpr (MinFilter == MagFilter) {
      return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[0], fU, fV);
    } else {
      auto fJ = fixed16::make(fM.data());
      if (fJ > TConst<fixed16>::One()) {
        return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[0], fU, fV);
      } else {
        return GetMinFilterN<MagFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[0], fU, fV);
      }
    }
  }

  if constexpr (MipFilter == FILTER_NEAREST) {
    if constexpr (MinFilter == MagFilter) {
      auto fJ =
          Math::TMax<fixed16>(fixed16::make(fM.data()), TConst<fixed16>::One());

      int mipLevel = Math::TMin<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                     sampler.MaxMipLevel);

      return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[mipLevel], fU, fV);
    } else {
      auto fJ = fixed16::make(fM.data());
      if (fJ > TConst<fixed16>::One()) {
        int mipLevel = Math::TMin<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                       sampler.MaxMipLevel);

        return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[mipLevel], fU, fV);
      } else {
        return GetMinFilterN<MagFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[0], fU, fV);
      }
    }
  }

  if constexpr (MipFilter == FILTER_LINEAR) {
    int lerpBits = TFormatInfo<Format>::LERP;
    int lerpMask = (1 << lerpBits) - 1;

    if constexpr (MinFilter == MagFilter) {
      auto fJ =
          Math::TMax<fixed16>(fixed16::make(fM.data()), TConst<fixed16>::One());

      int mipLevel0 = Math::TMin<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                      sampler.MaxMipLevel);

      int mipLevel1 = Math::TMin<int>(mipLevel0 + 1, sampler.MaxMipLevel);
      int mipLerp =
          (fJ.data() >> (mipLevel0 + fixed16::FRAC - lerpBits)) & lerpMask;

      const TColorNative<Format> c0 =
          GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
              sampler.pMipLevels[mipLevel0], fU, fV);

      const TColorNative<Format> c1 =
          GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
              sampler.pMipLevels[mipLevel1], fU, fV);

      return c0.Lerp(c1, mipLerp);
    } else {
      auto fJ = fixed16::make(fM.data());
      if (fJ > TConst<fixed16>::One()) {
        int mipLevel0 = Math::TMin<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                        sampler.MaxMipLevel);

        int mipLevel1 = Math::TMin<int>(mipLevel0 + 1, sampler.MaxMipLevel);
        int mipLerp =
            (fJ.data() >> (mipLevel0 + fixed16::FRAC - lerpBits)) & lerpMask;

        const TColorNative<Format> c0 =
            GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
                sampler.pMipLevels[mipLevel0], fU, fV);

        const TColorNative<Format> c1 =
            GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
                sampler.pMipLevels[mipLevel1], fU, fV);

        return c0.Lerp(c1, mipLerp);
      } else {
        return GetMinFilterN<MagFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[0], fU, fV);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t EnvMode>
void GetTexEnvColorA(Color4 *pInOut, const Color4 &cTexture,
                     ColorARGB cEnvColor) {

  __unreferenced(cEnvColor);

  if constexpr (EnvMode == ENVMODE_ADD) {
    pInOut->a = Math::Mul8(pInOut->a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    pInOut->a = Math::Mul8(pInOut->a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    pInOut->a = cTexture.a;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    pInOut->a = Math::Mul8(pInOut->a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    pInOut->a = cTexture.a;
  }
}

template <uint32_t EnvMode>
void GetTexEnvColorRGB(Color4 *pInOut, const Color4 &cTexture,
                       ColorARGB cEnvColor) {

  if constexpr (EnvMode == ENVMODE_ADD) {
    __unreferenced(cEnvColor);
    pInOut->r = Math::Add8(pInOut->r, cTexture.r);
    pInOut->g = Math::Add8(pInOut->g, cTexture.g);
    pInOut->b = Math::Add8(pInOut->b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    pInOut->r = Math::Lerp8(pInOut->r, cEnvColor.r, cTexture.r);
    pInOut->g = Math::Lerp8(pInOut->g, cEnvColor.g, cTexture.g);
    pInOut->b = Math::Lerp8(pInOut->b, cEnvColor.b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    __unreferenced(cEnvColor);
    pInOut->r = cTexture.r;
    pInOut->g = cTexture.g;
    pInOut->b = cTexture.b;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    __unreferenced(cEnvColor);
    pInOut->r = Math::Mul8(pInOut->r, cTexture.r);
    pInOut->g = Math::Mul8(pInOut->g, cTexture.g);
    pInOut->b = Math::Mul8(pInOut->b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    __unreferenced(cEnvColor);
    pInOut->r = cTexture.r;
    pInOut->g = cTexture.g;
    pInOut->b = cTexture.b;
  }
}

template <uint32_t EnvMode>
void GetTexEnvColorARGB(Color4 *pInOut, const Color4 &cTexture,
                        ColorARGB cEnvColor) {

  if constexpr (EnvMode == ENVMODE_ADD) {
    __unreferenced(cEnvColor);
    pInOut->r = Math::Add8(pInOut->r, cTexture.r);
    pInOut->g = Math::Add8(pInOut->g, cTexture.g);
    pInOut->b = Math::Add8(pInOut->b, cTexture.b);
    pInOut->a = Math::Mul8(pInOut->a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    pInOut->r = Math::Lerp8(pInOut->r, cEnvColor.r, cTexture.r);
    pInOut->g = Math::Lerp8(pInOut->g, cEnvColor.g, cTexture.g);
    pInOut->b = Math::Lerp8(pInOut->b, cEnvColor.b, cTexture.b);
    pInOut->a = Math::Mul8(pInOut->a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    __unreferenced(cEnvColor);
    pInOut->r = cTexture.r;
    pInOut->g = cTexture.g;
    pInOut->b = cTexture.b;
    pInOut->a = cTexture.a;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    __unreferenced(cEnvColor);
    pInOut->r = Math::Mul8(pInOut->r, cTexture.r);
    pInOut->g = Math::Mul8(pInOut->g, cTexture.g);
    pInOut->b = Math::Mul8(pInOut->b, cTexture.b);
    pInOut->a = Math::Mul8(pInOut->a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    __unreferenced(cEnvColor);
    pInOut->r = Math::Lerp8(pInOut->r, cTexture.r, cTexture.a);
    pInOut->g = Math::Lerp8(pInOut->g, cTexture.g, cTexture.a);
    pInOut->b = Math::Lerp8(pInOut->b, cTexture.b, cTexture.a);
    pInOut->a = cTexture.a;
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t BlendOp>
void GetBlendCoeff(Color4 *pInOut, const Color4 &cSrc, const Color4 &cDst) {

  if constexpr (BlendOp == BLEND_ZERO) {
    __unreferenced(cSrc);
    __unreferenced(cDst);

    pInOut->r = 0;
    pInOut->g = 0;
    pInOut->b = 0;
    pInOut->a = 0;
  }

  if constexpr (BlendOp == BLEND_ONE) {
    __unreferenced(pInOut);
    __unreferenced(cSrc);
    __unreferenced(cDst);
  }

  if constexpr (BlendOp == BLEND_SRC_COLOR) {
    __unreferenced(cDst);

    pInOut->r = Math::Mul8(pInOut->r, cSrc.r);
    pInOut->g = Math::Mul8(pInOut->g, cSrc.g);
    pInOut->b = Math::Mul8(pInOut->b, cSrc.b);
    pInOut->a = Math::Mul8(pInOut->a, cSrc.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_SRC_COLOR) {
    __unreferenced(cDst);

    pInOut->r = Math::Mul8(pInOut->r, 0xff - cSrc.r);
    pInOut->g = Math::Mul8(pInOut->g, 0xff - cSrc.g);
    pInOut->b = Math::Mul8(pInOut->b, 0xff - cSrc.b);
    pInOut->a = Math::Mul8(pInOut->a, 0xff - cSrc.a);
  }

  if constexpr (BlendOp == BLEND_SRC_ALPHA) {
    __unreferenced(cDst);

    pInOut->r = Math::Mul8(pInOut->r, cSrc.a);
    pInOut->g = Math::Mul8(pInOut->g, cSrc.a);
    pInOut->b = Math::Mul8(pInOut->b, cSrc.a);
    pInOut->a = Math::Mul8(pInOut->a, cSrc.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_SRC_ALPHA) {
    __unreferenced(cDst);

    int invAlpha = 0xff - cSrc.a;
    pInOut->r = Math::Mul8(pInOut->r, invAlpha);
    pInOut->g = Math::Mul8(pInOut->g, invAlpha);
    pInOut->b = Math::Mul8(pInOut->b, invAlpha);
    pInOut->a = Math::Mul8(pInOut->a, invAlpha);
  }

  if constexpr (BlendOp == BLEND_DST_ALPHA) {
    __unreferenced(cSrc);

    pInOut->r = Math::Mul8(pInOut->r, cDst.a);
    pInOut->g = Math::Mul8(pInOut->g, cDst.a);
    pInOut->b = Math::Mul8(pInOut->b, cDst.a);
    pInOut->a = Math::Mul8(pInOut->a, cDst.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_DST_ALPHA) {
    __unreferenced(cSrc);

    int invAlpha = 0xff - cDst.a;
    pInOut->r = Math::Mul8(pInOut->r, invAlpha);
    pInOut->g = Math::Mul8(pInOut->g, invAlpha);
    pInOut->b = Math::Mul8(pInOut->b, invAlpha);
    pInOut->a = Math::Mul8(pInOut->a, invAlpha);
  }

  if constexpr (BlendOp == BLEND_DST_COLOR) {
    __unreferenced(cSrc);

    pInOut->r = Math::Mul8(pInOut->r, cDst.r);
    pInOut->g = Math::Mul8(pInOut->g, cDst.g);
    pInOut->b = Math::Mul8(pInOut->b, cDst.b);
    pInOut->a = Math::Mul8(pInOut->a, cDst.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_DST_COLOR) {
    __unreferenced(cSrc);

    pInOut->r = Math::Mul8(pInOut->r, 0xff - cDst.r);
    pInOut->g = Math::Mul8(pInOut->g, 0xff - cDst.g);
    pInOut->b = Math::Mul8(pInOut->b, 0xff - cDst.b);
    pInOut->a = Math::Mul8(pInOut->a, 0xff - cDst.a);
  }

  if constexpr (BlendOp == BLEND_SRC_ALPHA_SATURATE) {
    int factor = Math::TMin(cSrc.a, 0xff - cDst.a);
    pInOut->r = Math::Mul8(pInOut->r, factor);
    pInOut->g = Math::Mul8(pInOut->g, factor);
    pInOut->b = Math::Mul8(pInOut->b, factor);
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t LogicOp>
uint32_t ApplyLogicOp(uint32_t srcColor, uint32_t dstColor) {

  if constexpr (LogicOp == LOGICOP_CLEAR) {
    __unreferenced(srcColor);
    __unreferenced(dstColor);
    return 0;
  }

  if constexpr (LogicOp == LOGICOP_AND) {
    return srcColor & dstColor;
  }

  if constexpr (LogicOp == LOGICOP_AND_REVERSE) {
    return srcColor & ~dstColor;
  }

  if constexpr (LogicOp == LOGICOP_COPY) {
    __unreferenced(dstColor);
    return srcColor;
  }

  if constexpr (LogicOp == LOGICOP_AND_INVERTED) {
    return ~srcColor & dstColor;
  }

  if constexpr (LogicOp == LOGICOP_NOOP) {
    __unreferenced(srcColor);
    return dstColor;
  }

  if constexpr (LogicOp == LOGICOP_XOR) {
    return srcColor ^ dstColor;
  }

  if constexpr (LogicOp == LOGICOP_OR) {
    return srcColor | dstColor;
  }

  if constexpr (LogicOp == LOGICOP_NOR) {
    return ~(srcColor | dstColor);
  }

  if constexpr (LogicOp == LOGICOP_EQUIV) {
    return ~(srcColor ^ dstColor);
  }

  if constexpr (LogicOp == LOGICOP_INVERT) {
    __unreferenced(srcColor);
    return ~dstColor;
  }

  if constexpr (LogicOp == LOGICOP_OR_REVERSE) {
    return srcColor | ~dstColor;
  }

  if constexpr (LogicOp == LOGICOP_COPY_INVERTED) {
    __unreferenced(dstColor);
    return ~srcColor;
  }

  if constexpr (LogicOp == LOGICOP_OR_INVERTED) {
    return ~srcColor | dstColor;
  }

  if constexpr (LogicOp == LOGICOP_NAND) {
    return ~(srcColor & dstColor);
  }

  if constexpr (LogicOp == LOGICOP_SET) {
    __unreferenced(srcColor);
    __unreferenced(dstColor);
    return 0xffffffff;
  }
}

inline void ToColor4(Color4 *pColor, fixedRX fA, fixedRX fR, fixedRX fG,
                     fixedRX fB) {
  pColor->b = fB.data() >> fixedRC::FRAC;
  pColor->g = fG.data() >> fixedRC::FRAC;
  pColor->r = fR.data() >> fixedRC::FRAC;
  pColor->a = fA.data() >> fixedRC::FRAC;
}
