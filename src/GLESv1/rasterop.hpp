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

template <uint32_t Format>
struct NColor {
  uint32_t Low;
  uint32_t High;

  NColor() {}

  explicit NColor(uint32_t value) {
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
      return (this->Low | (this->Low >> 8)) & 0xffff;
    }

    if constexpr (Format == FORMAT_R5G6B5) {
      return (this->Low | (this->Low >> 16)) & 0xffff;
    }

    if constexpr (Format == FORMAT_A4R4G4B4) {
      return (this->Low | (this->Low >> 12)) & 0xffff;
    }

    if constexpr (Format == FORMAT_A8R8G8B8) {
      return this->Low | (this->High << 8);
    }
  }

  NColor<Format> multiply(uint32_t frac) const {
    NColor<Format> result;

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

  NColor<Format> lerp(const NColor<Format> &c1, uint32_t frac) const {
    NColor<Format> result;

    if constexpr (Format == FORMAT_A8) {
      result.Low = this->Low + (((c1.Low - this->Low) * frac) >> 8);
    }

    if constexpr (Format == FORMAT_L8) {
      result.Low = this->Low + (((c1.Low - this->Low) * frac) >> 8);
    }

    if constexpr (Format == FORMAT_A8L8) {
      result.Low = (this->Low + (((c1.Low - this->Low) * frac) >> 8)) & 0x00ff00ff;
    }

    if constexpr (Format == FORMAT_R5G6B5) {
      result.Low = (this->Low + (((c1.Low - this->Low) * frac) >> 5)) & 0x07e0f81f;
    }

    if constexpr (Format == FORMAT_A4R4G4B4) {
      result.Low = (this->Low + (((c1.Low - this->Low) * frac) >> 4)) & 0x0f0f0f0f;
    }

    if constexpr (Format == FORMAT_A8R8G8B8) {
      result.Low = (this->Low + (((c1.Low - this->Low) * frac) >> 8)) & 0x00ff00ff;
      result.High = (this->High + (((c1.High - this->High) * frac) >> 8)) & 0x00ff00ff;
    }

    return result;
  }
};

template <uint32_t compare>
inline bool DoCompare(uint32_t a, uint32_t b) {

  if constexpr (compare == COMPARE_NEVER) {
    __unused(a);
    __unused(b);
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
    __unused(a);
    __unused(b);
    return true;
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t StencilOp>
inline uint32_t DoStencilOp(uint32_t stencilValue, uint32_t stencilRef) {

  if constexpr (StencilOp == STENCIL_KEEP) {
    __unused(stencilRef);
    return stencilValue;
  }

  if constexpr (StencilOp == STENCIL_REPLACE) {
    __unused(stencilValue);
    return stencilRef;
  }

  if constexpr (StencilOp == STENCIL_INCR) {
    __unused(stencilRef);
    return stencilValue + (stencilValue != 0xffff) ? 1 : 0;
  }

  if constexpr (StencilOp == STENCIL_DECR) {
    __unused(stencilRef);
    return stencilValue - (stencilValue != 0) ? 1 : 0;
  }

  if constexpr (StencilOp == STENCIL_ZERO) {
    __unused(stencilValue);
    __unused(stencilRef);
    return 0;
  }

  if constexpr (StencilOp == STENCIL_INVERT) {
    __unused(stencilRef);
    return ~stencilValue;
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Address>
inline int DoAddressing(int x) {
  if constexpr (Address == ADDRESS_WRAP) {
    return x & fixedRX::MASK;
  }
  if constexpr (Address == ADDRESS_CLAMP) {
    return std::clamp<int>(x, 0, fixedRX::MASK);
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline uint32_t GetTexelColorPtN(const SurfaceDesc &surface, fixedRX fU,
                                 fixedRX fV) {
  auto pBits = reinterpret_cast<const typename TFormatInfo<Format>::TYPE *>(surface.getBits());
  auto logWidth = surface.getLogWidth();
  auto logHeight = surface.getLogHeight();

  auto u = DoAddressing<AddressU>(fU.data());
  auto v = DoAddressing<AddressV>(fV.data());

  auto x = u >> (fixedRX::FRAC - logWidth);
  auto y = v >> (fixedRX::FRAC - logHeight);
  auto offset = x + (y << logWidth);

  return *(pBits + offset);
}

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline NColor<Format> GetTexelColorLnX(const SurfaceDesc &surface,
                                       fixedRX fU, fixedRX fV) {
  auto lerpBits = TFormatInfo<Format>::LERP;
  auto lerpMask = (1 << lerpBits) - 1;

  auto pBits = reinterpret_cast<const typename TFormatInfo<Format>::TYPE *>(surface.getBits());
  auto logWidth = surface.getLogWidth();
  auto logHeight = surface.getLogHeight();

  auto v0 = DoAddressing<AddressV>(fV.data() - (fixedRX::HALF >> logHeight));
  auto v1 = DoAddressing<AddressV>(fV.data() + (fixedRX::HALF >> logHeight));
  auto u0 = DoAddressing<AddressU>(fU.data() - (fixedRX::HALF >> logWidth));
  auto u1 = DoAddressing<AddressU>(fU.data() + (fixedRX::HALF >> logWidth));

  auto logHeightN = fixedRX::FRAC - (logHeight + lerpBits);
  auto y1 = v1 >> logHeightN;
  auto y0 = v0 >> logHeightN;
  auto beta = y0 & lerpMask;
  auto y3 = (y1 >> lerpBits) << logWidth;
  auto y2 = (y0 >> lerpBits) << logWidth;

  auto logWidthN = fixedRX::FRAC - (logWidth + lerpBits);
  auto x1 = u1 >> logWidthN;
  auto x0 = u0 >> logWidthN;
  auto alpha = x0 & lerpMask;
  auto x3 = x1 >> lerpBits;
  auto x2 = x0 >> lerpBits;

  auto c0 = pBits[y2 + x2];
  auto c1 = pBits[y2 + x3];
  auto c2 = pBits[y3 + x2];
  auto c3 = pBits[y3 + x3];

  NColor<Format> nc0(c0);
  NColor<Format> nc1(c1);
  nc0 = nc0.lerp(nc1, alpha);

  NColor<Format> nc2(c2);
  NColor<Format> nc3(c3);
  nc2 = nc2.lerp(nc3, alpha);

  return nc0.lerp(nc2, beta);
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Filter, uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline uint32_t GetMinFilterN(const SurfaceDesc &surface, fixedRX fU,
                              fixedRX fV) {
  if constexpr (Filter == FILTER_NEAREST) {
    return GetTexelColorPtN<Format, AddressU, AddressV>(surface, fU, fV);
  }

  if constexpr (Filter == FILTER_LINEAR) {
    return GetTexelColorLnX<Format, AddressU, AddressV>(surface, fU, fV);
  }
}

template <uint32_t Filter, uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline NColor<Format> GetMinFilterX(const SurfaceDesc &surface,
                                    fixedRX fU, fixedRX fV) {
  if constexpr (Filter == FILTER_NEAREST) {
    return NColor<Format>(
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
      if (fJ > Math::One<fixed16>()) {
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
      auto fJ = std::max<fixed16>(fixed16::make(fM.data()), Math::One<fixed16>());
      auto mipLevel = std::min<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
                                    sampler.MaxMipLevel);

      return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[mipLevel], fU, fV);
    } else {
      auto fJ = fixed16::make(fM.data());
      if (fJ > Math::One<fixed16>()) {
        int mipLevel = std::min<int>(Math::iLog2(fJ.data()) - fixed16::FRAC,
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
    auto lerpBits = TFormatInfo<Format>::LERP;
    auto lerpMask = (1 << lerpBits) - 1;

    if constexpr (MinFilter == MagFilter) {
      auto fJ = std::max<fixed16>(fixed16::make(fM.data()), Math::One<fixed16>());
      auto mipLevel0 = std::min<int>(Math::iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
      auto mipLevel1 = std::min<int>(mipLevel0 + 1, sampler.MaxMipLevel);
      auto mipLerp = (fJ.data() >> (mipLevel0 + fixed16::FRAC - lerpBits)) & lerpMask;

      auto c0 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[mipLevel0], fU, fV);
      auto c1 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[mipLevel1], fU, fV);
      return c0.Lerp(c1, mipLerp);
    } else {
      auto fJ = fixed16::make(fM.data());
      if (fJ > Math::One<fixed16>()) {
        auto mipLevel0 = std::min<int>(Math::iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
        auto mipLevel1 = std::min<int>(mipLevel0 + 1, sampler.MaxMipLevel);
        auto mipLerp = (fJ.data() >> (mipLevel0 + fixed16::FRAC - lerpBits)) & lerpMask;

        auto c0 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[mipLevel0], fU, fV);
        auto c1 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
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
ColorARGB GetTexEnvColorA(const ColorARGB &cColor, const ColorARGB &cTexture,
                          const ColorARGB &cEnvColor) {
  __unused(cEnvColor);
  ColorARGB ret(cColor);

  if constexpr (EnvMode == ENVMODE_ADD) {
    ret.a = Math::Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    ret.a = Math::Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    ret.a = cTexture.a;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    ret.a = Math::Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    ret.a = cTexture.a;
  }
  return ret;
}

template <uint32_t EnvMode>
ColorARGB GetTexEnvColorRGB(const ColorARGB &cColor, const ColorARGB &cTexture,
                            const ColorARGB &cEnvColor) {
  ColorARGB ret(cColor);

  if constexpr (EnvMode == ENVMODE_ADD) {
    __unused(cEnvColor);
    ret.r = Math::Add8(cColor.r, cTexture.r);
    ret.g = Math::Add8(cColor.g, cTexture.g);
    ret.b = Math::Add8(cColor.b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    ret.r = Math::Lerp8(cColor.r, cEnvColor.r, cTexture.r);
    ret.g = Math::Lerp8(cColor.g, cEnvColor.g, cTexture.g);
    ret.b = Math::Lerp8(cColor.b, cEnvColor.b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    __unused(cEnvColor);
    ret.r = cTexture.r;
    ret.g = cTexture.g;
    ret.b = cTexture.b;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    __unused(cEnvColor);
    ret.r = Math::Mul8(cColor.r, cTexture.r);
    ret.g = Math::Mul8(cColor.g, cTexture.g);
    ret.b = Math::Mul8(cColor.b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    __unused(cEnvColor);
    ret.r = cTexture.r;
    ret.g = cTexture.g;
    ret.b = cTexture.b;
  }

  return ret;
}

template <uint32_t EnvMode>
ColorARGB GetTexEnvColorARGB(const ColorARGB &cColor, const ColorARGB &cTexture,
                             const ColorARGB &cEnvColor) {
  ColorARGB ret;

  if constexpr (EnvMode == ENVMODE_ADD) {
    __unused(cEnvColor);
    ret.r = Math::Add8(cColor.r, cTexture.r);
    ret.g = Math::Add8(cColor.g, cTexture.g);
    ret.b = Math::Add8(cColor.b, cTexture.b);
    ret.a = Math::Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    ret.r = Math::Lerp8(cColor.r, cEnvColor.r, cTexture.r);
    ret.g = Math::Lerp8(cColor.g, cEnvColor.g, cTexture.g);
    ret.b = Math::Lerp8(cColor.b, cEnvColor.b, cTexture.b);
    ret.a = Math::Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    __unused(cEnvColor);
    ret.r = cTexture.r;
    ret.g = cTexture.g;
    ret.b = cTexture.b;
    ret.a = cTexture.a;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    __unused(cEnvColor);
    ret.r = Math::Mul8(cColor.r, cTexture.r);
    ret.g = Math::Mul8(cColor.g, cTexture.g);
    ret.b = Math::Mul8(cColor.b, cTexture.b);
    ret.a = Math::Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    __unused(cEnvColor);
    ret.r = Math::Lerp8(cColor.r, cTexture.r, cTexture.a);
    ret.g = Math::Lerp8(cColor.g, cTexture.g, cTexture.a);
    ret.b = Math::Lerp8(cColor.b, cTexture.b, cTexture.a);
    ret.a = cTexture.a;
  }

  return ret;
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t BlendOp>
ColorARGB GetBlendCoeff(const ColorARGB &cColor, const ColorARGB &cSrc, const ColorARGB &cDst) {
  ColorARGB ret;

  if constexpr (BlendOp == BLEND_ZERO) {
    __unused(cSrc);
    __unused(cDst);

    ret.r = 0;
    ret.g = 0;
    ret.b = 0;
    ret.a = 0;
  }

  if constexpr (BlendOp == BLEND_ONE) {
    __unused(cSrc);
    __unused(cDst);
  }

  if constexpr (BlendOp == BLEND_SRC_COLOR) {
    __unused(cDst);

    ret.r = Math::Mul8(cColor.r, cSrc.r);
    ret.g = Math::Mul8(cColor.g, cSrc.g);
    ret.b = Math::Mul8(cColor.b, cSrc.b);
    ret.a = Math::Mul8(cColor.a, cSrc.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_SRC_COLOR) {
    __unused(cDst);

    ret.r = Math::Mul8(cColor.r, 0xff - cSrc.r);
    ret.g = Math::Mul8(cColor.g, 0xff - cSrc.g);
    ret.b = Math::Mul8(cColor.b, 0xff - cSrc.b);
    ret.a = Math::Mul8(cColor.a, 0xff - cSrc.a);
  }

  if constexpr (BlendOp == BLEND_SRC_ALPHA) {
    __unused(cDst);

    ret.r = Math::Mul8(cColor.r, cSrc.a);
    ret.g = Math::Mul8(cColor.g, cSrc.a);
    ret.b = Math::Mul8(cColor.b, cSrc.a);
    ret.a = Math::Mul8(cColor.a, cSrc.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_SRC_ALPHA) {
    __unused(cDst);

    int invAlpha = 0xff - cSrc.a;
    ret.r = Math::Mul8(cColor.r, invAlpha);
    ret.g = Math::Mul8(cColor.g, invAlpha);
    ret.b = Math::Mul8(cColor.b, invAlpha);
    ret.a = Math::Mul8(cColor.a, invAlpha);
  }

  if constexpr (BlendOp == BLEND_DST_ALPHA) {
    __unused(cSrc);

    ret.r = Math::Mul8(cColor.r, cDst.a);
    ret.g = Math::Mul8(cColor.g, cDst.a);
    ret.b = Math::Mul8(cColor.b, cDst.a);
    ret.a = Math::Mul8(cColor.a, cDst.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_DST_ALPHA) {
    __unused(cSrc);

    int invAlpha = 0xff - cDst.a;
    ret.r = Math::Mul8(cColor.r, invAlpha);
    ret.g = Math::Mul8(cColor.g, invAlpha);
    ret.b = Math::Mul8(cColor.b, invAlpha);
    ret.a = Math::Mul8(cColor.a, invAlpha);
  }

  if constexpr (BlendOp == BLEND_DST_COLOR) {
    __unused(cSrc);

    ret.r = Math::Mul8(cColor.r, cDst.r);
    ret.g = Math::Mul8(cColor.g, cDst.g);
    ret.b = Math::Mul8(cColor.b, cDst.b);
    ret.a = Math::Mul8(cColor.a, cDst.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_DST_COLOR) {
    __unused(cSrc);

    ret.r = Math::Mul8(cColor.r, 0xff - cDst.r);
    ret.g = Math::Mul8(cColor.g, 0xff - cDst.g);
    ret.b = Math::Mul8(cColor.b, 0xff - cDst.b);
    ret.a = Math::Mul8(cColor.a, 0xff - cDst.a);
  }

  if constexpr (BlendOp == BLEND_SRC_ALPHA_SATURATE) {
    auto factor = std::min<int>(cSrc.a, 0xff - cDst.a);
    ret.r = Math::Mul8(cColor.r, factor);
    ret.g = Math::Mul8(cColor.g, factor);
    ret.b = Math::Mul8(cColor.b, factor);
  }

  return ret;
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t LogicOp>
uint32_t ApplyLogicOp(uint32_t srcColor, uint32_t dstColor) {

  if constexpr (LogicOp == LOGICOP_CLEAR) {
    __unused(srcColor);
    __unused(dstColor);
    return 0;
  }

  if constexpr (LogicOp == LOGICOP_AND) {
    return srcColor & dstColor;
  }

  if constexpr (LogicOp == LOGICOP_AND_REVERSE) {
    return srcColor & ~dstColor;
  }

  if constexpr (LogicOp == LOGICOP_COPY) {
    __unused(dstColor);
    return srcColor;
  }

  if constexpr (LogicOp == LOGICOP_AND_INVERTED) {
    return ~srcColor & dstColor;
  }

  if constexpr (LogicOp == LOGICOP_NOOP) {
    __unused(srcColor);
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
    __unused(srcColor);
    return ~dstColor;
  }

  if constexpr (LogicOp == LOGICOP_OR_REVERSE) {
    return srcColor | ~dstColor;
  }

  if constexpr (LogicOp == LOGICOP_COPY_INVERTED) {
    __unused(dstColor);
    return ~srcColor;
  }

  if constexpr (LogicOp == LOGICOP_OR_INVERTED) {
    return ~srcColor | dstColor;
  }

  if constexpr (LogicOp == LOGICOP_NAND) {
    return ~(srcColor & dstColor);
  }

  if constexpr (LogicOp == LOGICOP_SET) {
    __unused(srcColor);
    __unused(dstColor);
    return 0xffffffff;
  }
}

inline ColorARGB ToColor4(fixedRX fA, fixedRX fR, fixedRX fG, fixedRX fB) {
  ColorARGB ret;
  ret.b = std::clamp(fB.data() >> (fixedRX::FRAC - 8), 0, 255);
  ret.g = std::clamp(fG.data() >> (fixedRX::FRAC - 8), 0, 255);
  ret.r = std::clamp(fR.data() >> (fixedRX::FRAC - 8), 0, 255);
  ret.a = std::clamp(fA.data() >> (fixedRX::FRAC - 8), 0, 255);
  return ret;
}
