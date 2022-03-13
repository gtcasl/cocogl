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
inline uint32_t DoAddressing(int32_t x) {
  if constexpr (Address == ADDRESS_WRAP) {
    return x & fixedRX::MASK;
  }
  if constexpr (Address == ADDRESS_CLAMP) {
    return std::clamp<int>(x, 0, fixedRX::MASK);
  }
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline uint32_t GetTexelColorPtN(const SurfaceDesc &surface, 
                                 fixedRX fU,
                                 fixedRX fV) {
  auto pBits = reinterpret_cast<const typename TFormatInfo<(ePixelFormat)Format>::TYPE *>(surface.getBits());
  uint32_t logWidth  = surface.getLogWidth();
  uint32_t logHeight = surface.getLogHeight();

  // addressing mode

  uint32_t u = DoAddressing<AddressU>(fU.data());
  uint32_t v = DoAddressing<AddressV>(fV.data());

  // address generation

  uint32_t x = u >> (fixedRX::FRAC - logWidth);
  uint32_t y = v >> (fixedRX::FRAC - logHeight);
  uint32_t offset = x + (y << logWidth);

  // memory lookup

  return *(pBits + offset);
}

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
inline NColor<Format> GetTexelColorLnX(const SurfaceDesc &surface,
                                       fixedRX fU, 
                                       fixedRX fV) {
  auto lerpBits = TFormatInfo<(ePixelFormat)Format>::LERP;
  auto lerpMask = (1 << lerpBits) - 1;
  
  auto pBits = reinterpret_cast<const typename TFormatInfo<(ePixelFormat)Format>::TYPE *>(surface.getBits());
  uint32_t logWidth  = surface.getLogWidth();
  uint32_t logHeight = surface.getLogHeight();

  // addressing mode

  uint32_t delta_u = (fixedRX::HALF >> logWidth);
  uint32_t delta_v = (fixedRX::HALF >> logHeight);

  uint32_t u0 = DoAddressing<AddressU>(fU.data() - delta_u);
  uint32_t u1 = DoAddressing<AddressU>(fU.data() + delta_u);
  uint32_t v0 = DoAddressing<AddressV>(fV.data() - delta_v);
  uint32_t v1 = DoAddressing<AddressV>(fV.data() + delta_v);

  // address generation

  uint32_t shift_u = (fixedRX::FRAC - logWidth);
  uint32_t shift_v = (fixedRX::FRAC - logHeight);

  uint32_t x0s = (u0 << lerpBits) >> shift_u;
  uint32_t y0s = (v0 << lerpBits) >> shift_v;  

  uint32_t x0 = x0s >> lerpBits;  
  uint32_t y0 = y0s >> lerpBits;  
  uint32_t x1 = u1 >> shift_u;
  uint32_t y1 = v1 >> shift_v;

  // memory lookup

  auto c0 = pBits[x0 + (y0 << logWidth)];
  auto c1 = pBits[x1 + (y0 << logWidth)];
  auto c2 = pBits[x0 + (y1 << logWidth)];
  auto c3 = pBits[x1 + (y1 << logWidth)];

  // filtering

  uint32_t alpha = x0s & lerpMask;
  uint32_t beta  = y0s & lerpMask;

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
inline uint32_t GetMinFilterN(const SurfaceDesc &surface, 
                              fixedRX fU,
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
                                    fixedRX fU, 
                                    fixedRX fV) {
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
inline uint32_t GetMipFilterN(const Sampler &sampler, 
                              fixedRX fU, 
                              fixedRX fV,
                              fixedRX fM) {
  auto fJ = fixed16::make(fM.data()); // fM is actually stored as 16:16

  if constexpr (MipFilter == FILTER_NONE) {
    if constexpr (MinFilter == MagFilter) {
      return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[0], fU, fV);
    } else {      
      if (fJ > One<fixed16>()) {
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
      fJ = std::max<fixed16>(fJ, One<fixed16>());
      uint32_t l = std::min<int>(iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
      return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[l], fU, fV);
    } else {
      if (fJ > One<fixed16>()) {
        uint32_t l = std::min<int>(iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
        return GetMinFilterN<MinFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[l], fU, fV);
      } else {
        return GetMinFilterN<MagFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[0], fU, fV);
      }
    }
  }

  if constexpr (MipFilter == FILTER_LINEAR) {
    auto lerpBits = TFormatInfo<(ePixelFormat)Format>::LERP;
    auto lerpMask = (1 << lerpBits) - 1;
    if constexpr (MinFilter == MagFilter) {
      fJ = std::max<fixed16>(fJ, One<fixed16>());
      uint32_t l0 = std::min<int>(iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
      uint32_t l1 = std::min<int>(l0 + 1, sampler.MaxMipLevel);
      uint32_t f  = (fJ.data() - (1 << (l0 + fixed16::FRAC))) >> (l0 + fixed16::FRAC - TFormatInfo<(ePixelFormat)Format>::LERP);
      auto c0 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[l0], fU, fV);
      auto c1 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
          sampler.pMipLevels[l1], fU, fV);
      return c0.Lerp(c1, f);
    } else {
      if (fJ > One<fixed16>()) {
        uint32_t l0 = std::min<int>(iLog2(fJ.data()) - fixed16::FRAC, sampler.MaxMipLevel);
        uint32_t l1 = std::min<int>(l0 + 1, sampler.MaxMipLevel);
        uint32_t f  = (fJ.data() - (1 << (l0 + fixed16::FRAC))) >> (l0 + fixed16::FRAC - TFormatInfo<(ePixelFormat)Format>::LERP);
        auto c0 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[l0], fU, fV);
        auto c1 = GetMinFilterX<MinFilter, Format, AddressU, AddressV>(
            sampler.pMipLevels[l1], fU, fV);
        return c0.Lerp(c1, f);
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
    ret.a = Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    ret.a = Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    ret.a = cTexture.a;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    ret.a = Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    ret.a = cTexture.a;
  }
  return ret;
}

template <uint32_t EnvMode>
ColorARGB GetTexEnvColorRGB(const ColorARGB &cColor, const 
                            ColorARGB &cTexture,
                            const ColorARGB &cEnvColor) {
  ColorARGB ret(cColor);

  if constexpr (EnvMode == ENVMODE_ADD) {
    __unused(cEnvColor);
    ret.r = Add8(cColor.r, cTexture.r);
    ret.g = Add8(cColor.g, cTexture.g);
    ret.b = Add8(cColor.b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    ret.r = Lerp8(cColor.r, cEnvColor.r, cTexture.r);
    ret.g = Lerp8(cColor.g, cEnvColor.g, cTexture.g);
    ret.b = Lerp8(cColor.b, cEnvColor.b, cTexture.b);
  }

  if constexpr (EnvMode == ENVMODE_REPLACE) {
    __unused(cEnvColor);
    ret.r = cTexture.r;
    ret.g = cTexture.g;
    ret.b = cTexture.b;
  }

  if constexpr (EnvMode == ENVMODE_MODULATE) {
    __unused(cEnvColor);
    ret.r = Mul8(cColor.r, cTexture.r);
    ret.g = Mul8(cColor.g, cTexture.g);
    ret.b = Mul8(cColor.b, cTexture.b);
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
ColorARGB GetTexEnvColorARGB(const ColorARGB &cColor, 
                             const ColorARGB &cTexture,
                             const ColorARGB &cEnvColor) {
  ColorARGB ret;

  if constexpr (EnvMode == ENVMODE_ADD) {
    __unused(cEnvColor);
    ret.r = Add8(cColor.r, cTexture.r);
    ret.g = Add8(cColor.g, cTexture.g);
    ret.b = Add8(cColor.b, cTexture.b);
    ret.a = Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_BLEND) {
    ret.r = Lerp8(cColor.r, cEnvColor.r, cTexture.r);
    ret.g = Lerp8(cColor.g, cEnvColor.g, cTexture.g);
    ret.b = Lerp8(cColor.b, cEnvColor.b, cTexture.b);
    ret.a = Mul8(cColor.a, cTexture.a);
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
    ret.r = Mul8(cColor.r, cTexture.r);
    ret.g = Mul8(cColor.g, cTexture.g);
    ret.b = Mul8(cColor.b, cTexture.b);
    ret.a = Mul8(cColor.a, cTexture.a);
  }

  if constexpr (EnvMode == ENVMODE_DECAL) {
    __unused(cEnvColor);
    ret.r = Lerp8(cColor.r, cTexture.r, cTexture.a);
    ret.g = Lerp8(cColor.g, cTexture.g, cTexture.a);
    ret.b = Lerp8(cColor.b, cTexture.b, cTexture.a);
    ret.a = cTexture.a;
  }

  return ret;
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t BlendOp>
ColorARGB GetBlendCoeff(const ColorARGB &cColor, 
                        const ColorARGB &cSrc, 
                        const ColorARGB &cDst) {
  ColorARGB ret(0);

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

    ret.r = Mul8(cColor.r, cSrc.r);
    ret.g = Mul8(cColor.g, cSrc.g);
    ret.b = Mul8(cColor.b, cSrc.b);
    ret.a = Mul8(cColor.a, cSrc.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_SRC_COLOR) {
    __unused(cDst);

    ret.r = Mul8(cColor.r, 0xff - cSrc.r);
    ret.g = Mul8(cColor.g, 0xff - cSrc.g);
    ret.b = Mul8(cColor.b, 0xff - cSrc.b);
    ret.a = Mul8(cColor.a, 0xff - cSrc.a);
  }

  if constexpr (BlendOp == BLEND_SRC_ALPHA) {
    __unused(cDst);

    ret.r = Mul8(cColor.r, cSrc.a);
    ret.g = Mul8(cColor.g, cSrc.a);
    ret.b = Mul8(cColor.b, cSrc.a);
    ret.a = Mul8(cColor.a, cSrc.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_SRC_ALPHA) {
    __unused(cDst);

    int invAlpha = 0xff - cSrc.a;
    ret.r = Mul8(cColor.r, invAlpha);
    ret.g = Mul8(cColor.g, invAlpha);
    ret.b = Mul8(cColor.b, invAlpha);
    ret.a = Mul8(cColor.a, invAlpha);
  }

  if constexpr (BlendOp == BLEND_DST_ALPHA) {
    __unused(cSrc);

    ret.r = Mul8(cColor.r, cDst.a);
    ret.g = Mul8(cColor.g, cDst.a);
    ret.b = Mul8(cColor.b, cDst.a);
    ret.a = Mul8(cColor.a, cDst.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_DST_ALPHA) {
    __unused(cSrc);

    int invAlpha = 0xff - cDst.a;
    ret.r = Mul8(cColor.r, invAlpha);
    ret.g = Mul8(cColor.g, invAlpha);
    ret.b = Mul8(cColor.b, invAlpha);
    ret.a = Mul8(cColor.a, invAlpha);
  }

  if constexpr (BlendOp == BLEND_DST_COLOR) {
    __unused(cSrc);

    ret.r = Mul8(cColor.r, cDst.r);
    ret.g = Mul8(cColor.g, cDst.g);
    ret.b = Mul8(cColor.b, cDst.b);
    ret.a = Mul8(cColor.a, cDst.a);
  }

  if constexpr (BlendOp == BLEND_ONE_MINUS_DST_COLOR) {
    __unused(cSrc);

    ret.r = Mul8(cColor.r, 0xff - cDst.r);
    ret.g = Mul8(cColor.g, 0xff - cDst.g);
    ret.b = Mul8(cColor.b, 0xff - cDst.b);
    ret.a = Mul8(cColor.a, 0xff - cDst.a);
  }

  if constexpr (BlendOp == BLEND_SRC_ALPHA_SATURATE) {
    auto factor = std::min<int>(cSrc.a, 0xff - cDst.a);
    ret.r = Mul8(cColor.r, factor);
    ret.g = Mul8(cColor.g, factor);
    ret.b = Mul8(cColor.b, factor);
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
