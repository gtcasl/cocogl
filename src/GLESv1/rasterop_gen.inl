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

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
ColorARGB GetTexelColorPt(const SurfaceDesc &surface, fixedRX fU, fixedRX fV) {
  return Format::ConvertFrom<Format, false>(GetTexelColorPtN<Format, AddressU, AddressV>(surface, fU, fV));
}

template <uint32_t Format, uint32_t AddressU, uint32_t AddressV>
ColorARGB GetTexelColorLn(const SurfaceDesc &surface, fixedRX fU, fixedRX fV) {
  return Format::ConvertFrom<Format, false>(GetTexelColorLnX<Format, AddressU, AddressV>(surface, fU, fV));
}

//////////////////////////////////////////////////////////////////////////////

template <ePixelFormat Format, eBlendOp OpSrc, eBlendOp OpDst>
ColorARGB Blender(const ColorARGB &cColor, const uint8_t *pCB) {
  ColorARGB ret;

  auto cDstColor = Format::ConvertFrom<Format, true>(pCB);
  auto srcCoeff = GetBlendCoeff<OpSrc>(cColor, cColor, cDstColor);
  auto dstCoeff = GetBlendCoeff<OpDst>(cDstColor, cColor, cDstColor);

  ret.r = Math::Add8(srcCoeff.r, dstCoeff.r);
  ret.g = Math::Add8(srcCoeff.g, dstCoeff.g);
  ret.b = Math::Add8(srcCoeff.b, dstCoeff.b);
  ret.a = Math::Add8(srcCoeff.a, dstCoeff.a);

  return ret;
}

//////////////////////////////////////////////////////////////////////////////

template <ePixelFormat Format, bool bWriteMask, eLogicOp LogicOp>
void DoWriteColor(const RasterData &rasterData, const ColorARGB &cColor,
                  uint8_t *pCB) {
  uint32_t dstColor = *reinterpret_cast<typename TFormatInfo<Format>::TYPE *>(pCB);
  auto result = Format::ConvertTo<Format>(cColor);
  result = ApplyLogicOp<LogicOp>(result, dstColor);

  if constexpr (bWriteMask) {
    auto writeMask = rasterData.ColorWriteMask;
    result = (result & writeMask) | (dstColor & ~writeMask);
  } else {
    __unused(rasterData);
    __unused(dstColor);
  }

  *reinterpret_cast<typename TFormatInfo<Format>::TYPE *>(pCB) =
      static_cast<typename TFormatInfo<Format>::TYPE>(result);
}

//////////////////////////////////////////////////////////////////////////////

template <bool Depth, bool Color, uint32_t Texture0, uint32_t Texture1,
          bool Fog>
class GenericScanlineA {
public:
  enum {
    DEPTH_SIZE = Depth ? 1 : 0,
    COLOR_SIZE = Color ? 4 : 0,
    TEX0_SIZE = Texture0 ? 2 : 0,
    TEX1_SIZE = Texture1 ? 2 : 0,
    MIP0_SIZE = Texture0 - TEX0_SIZE,
    MIP1_SIZE = Texture1 - TEX1_SIZE,
    FOG_SIZE = Fog ? 1 : 0,

    REG_DEPTH = 0,
    REG_COLOR = REG_DEPTH + DEPTH_SIZE,
    REG_TEX0 = REG_COLOR + COLOR_SIZE,
    REG_TEX1 = REG_TEX0 + TEX0_SIZE,
    REG_MIP0 = REG_TEX1 + TEX1_SIZE,
    REG_MIP1 = REG_MIP0 + MIP0_SIZE,
    REG_FOG = REG_MIP1 + MIP1_SIZE,
    REG_SIZE = REG_FOG + FOG_SIZE,
  };

  inline static void Execute(const RasterData &rasterData, int y, int lx,
                             int rx) {
    if constexpr (Texture0 != 0) {
      GenericScanlineA<Depth, Color, Texture0, Texture1, Fog>::__Execute(rasterData, y, lx, rx);
    } else {
      if constexpr (Color) {
        GenericScanlineA<Depth, Color, 0, 0, Fog>::__Execute(rasterData, y, lx, rx);
      } else {
        __unused(rasterData);
        __unused(y);
        __unused(lx);
        __unused(rx);
      }
    }
  }

  inline static void __Execute(const RasterData &rasterData, int y, int lx,
                               int rx) {
    auto pRasterOp = reinterpret_cast<const GenericRasterOp *>(rasterData.pRasterOp);
    RASTERFLAGS rasterFlags = pRasterOp->getRasterID().Flags;

    auto pColorBits = rasterData.pColorBits;
    auto colorPitch = rasterData.ColorPitch;
    auto colorStride = pRasterOp->getColorStride();

    auto pDepthStencilBits = rasterData.pDepthStencilBits;
    auto depthStencilPitch = rasterData.DepthStencilPitch;
    auto depthStencilStride = pRasterOp->getDepthStencilStride();

    auto fOffsetX = fixed4(lx) - rasterData.fRefX;
    auto fOffsetY = fixed4(y) - rasterData.fRefY;

    fixedRX fZ, fZdA;

    if constexpr (Depth) {
      fZdA = rasterData.Registers[REG_DEPTH + 0].m[0];
      fixedRX fZdB = rasterData.Registers[REG_DEPTH + 0].m[1];
      fixedRX fZdC = rasterData.Registers[REG_DEPTH + 0].m[2];
      fZ = fZdA * fOffsetX + fZdB * fOffsetY + fZdC;
    }

    fixedRX fB, fG, fR, fA;
    fixedRX fBdA, fGdA, fRdA, fAdA;

    if constexpr (Color) {
      fBdA = rasterData.Registers[REG_COLOR + 0].m[0];
      fixedRX fBdB = rasterData.Registers[REG_COLOR + 0].m[1];
      fixedRX fBdC = rasterData.Registers[REG_COLOR + 0].m[2];
      fB = fBdA * fOffsetX + fBdB * fOffsetY + fBdC;

      fGdA = rasterData.Registers[REG_COLOR + 1].m[0];
      fixedRX fGdB = rasterData.Registers[REG_COLOR + 1].m[1];
      fixedRX fGdC = rasterData.Registers[REG_COLOR + 1].m[2];
      fG = fGdA * fOffsetX + fGdB * fOffsetY + fGdC;

      fRdA = rasterData.Registers[REG_COLOR + 2].m[0];
      fixedRX fRdB = rasterData.Registers[REG_COLOR + 2].m[1];
      fixedRX fRdC = rasterData.Registers[REG_COLOR + 2].m[2];
      fR = fRdA * fOffsetX + fRdB * fOffsetY + fRdC;

      fAdA = rasterData.Registers[REG_COLOR + 3].m[0];
      fixedRX fAdB = rasterData.Registers[REG_COLOR + 3].m[1];
      fixedRX fAdC = rasterData.Registers[REG_COLOR + 3].m[2];
      fA = fAdA * fOffsetX + fAdB * fOffsetY + fAdC;
    }

    fixedRX fU0, fV0, fU1, fV1;
    fixedRX fM0, fM1;
    fixedRX fU0dA, fV0dA, fU1dA, fV1dA;

    if constexpr (Texture0 != 0) {
      fU0dA = rasterData.Registers[REG_TEX0 + 0].m[0];
      fixedRX fU0dB = rasterData.Registers[REG_TEX0 + 0].m[1];
      fixedRX fU0dC = rasterData.Registers[REG_TEX0 + 0].m[2];
      fU0 = fU0dA * fOffsetX + fU0dB * fOffsetY + fU0dC;

      fV0dA = rasterData.Registers[REG_TEX0 + 1].m[0];
      fixedRX fV0dB = rasterData.Registers[REG_TEX0 + 1].m[1];
      fixedRX fV0dC = rasterData.Registers[REG_TEX0 + 1].m[2];
      fV0 = fV0dA * fOffsetX + fV0dB * fOffsetY + fV0dC;

      if constexpr (2 == Texture0) {
        fM0 = rasterData.Registers[REG_MIP0 + 0].m[2];
      }

      if constexpr (Texture1 != 0) {
        fU1dA = rasterData.Registers[REG_TEX1 + 0].m[0];
        fixedRX fU1dB = rasterData.Registers[REG_TEX1 + 0].m[1];
        fixedRX fU1dC = rasterData.Registers[REG_TEX1 + 0].m[2];
        fU1 = fU1dA * fOffsetX + fU1dB * fOffsetY + fU1dC;

        fV1dA = rasterData.Registers[REG_TEX1 + 1].m[0];
        fixedRX fV1dB = rasterData.Registers[REG_TEX1 + 1].m[1];
        fixedRX fV1dC = rasterData.Registers[REG_TEX1 + 1].m[2];
        fV1 = fV1dA * fOffsetX + fV1dB * fOffsetY + fV1dC;

        if constexpr (2 == Texture1) {
          fM1 = rasterData.Registers[REG_MIP1 + 0].m[2];
        }
      }
    }

    fixedRX fFog, fFogdA;

    if constexpr (Fog) {
      fFogdA = rasterData.Registers[REG_FOG + 0].m[0];
      fixedRX fFogdB = rasterData.Registers[REG_FOG + 0].m[1];
      fixedRX fFogdC = rasterData.Registers[REG_FOG + 0].m[2];
      fFog = fFogdA * fOffsetX + fFogdB * fOffsetY + fFogdC;
    }

    auto pDS = lx * depthStencilStride + y * depthStencilPitch + pDepthStencilBits;
    auto pCB = lx * colorStride + y * colorPitch + pColorBits;
    auto pCBEnd = pCB + (rx - lx) * colorStride;

    ColorARGB cColor(0xff, 0xff, 0xff, 0xff);

    do {
      for (;;) {
        uint32_t depthValue = 0;

        if constexpr (Depth) {
          depthValue = static_cast<fixed16>(fZ).data() & 0xFFFF;

          // Early out depth test
          if (!rasterFlags.StencilTest &&
              !pRasterOp->doDepthTest(depthValue, pDS)) {
            break;
          }
        }

        if constexpr (Color) {
          cColor = ToColor4(fA, fR, fG, fB);
        }

        // Execute texture operations
        if constexpr (Texture0 != 0) {
          ColorARGB cTexture;

          if constexpr (2 == Texture0) {
            cTexture = pRasterOp->getSamplerColor(0, rasterData, fU0, fV0, fM0);
          } else {
            cTexture = pRasterOp->getSamplerColor(0, rasterData, fU0, fV0);
          }

          cColor = pRasterOp->getTexEnvColor(cColor, cTexture, 0, rasterData);

          if constexpr (Texture1 != 0) {
            if constexpr (2 == Texture1) {
              cTexture = pRasterOp->getSamplerColor(1, rasterData, fU1, fV1, fM1);
            } else {
              cTexture = pRasterOp->getSamplerColor(1, rasterData, fU1, fV1);
            }

            cColor = pRasterOp->getTexEnvColor(cColor, cTexture, 1, rasterData);
          }
        }

        if (rasterFlags.AlphaTest) {
          // Execute alpha test
          if (!pRasterOp->doAlphaTest(rasterData, cColor)) {
            break;
          }
        }

        if (rasterFlags.StencilTest) {
          // Execute stencil test
          if (!pRasterOp->doStencilTest(rasterData, depthValue, pDS)) {
            break;
          }
        } else {
          if constexpr (Depth) {
            if (rasterFlags.DepthWrite) {
              // Write output depth
              *reinterpret_cast<uint16_t *>(pDS) =
                  static_cast<uint16_t>(depthValue);
            }
          }
        }

        if constexpr (Fog) {
          // Execute fog blend
          cColor = pRasterOp->applyFog(rasterData, cColor, fFog);
        }

        if (rasterFlags.Blend) {
          // Execute pixel blend
          cColor = pRasterOp->doBlend(cColor, pCB);
        }

        // Write output color
        pRasterOp->writeColor(rasterData, cColor, pCB);

        break;
      }

      pDS += depthStencilStride;

      if constexpr (Depth) {
        fZ += fZdA;
      }

      if constexpr (Color) {
        fB += fBdA;
        fG += fGdA;
        fR += fRdA;
        fA += fAdA;
      }

      if constexpr (Texture0 != 0) {
        fU0 += fU0dA;
        fV0 += fV0dA;
      }

      if constexpr (Texture1 != 0) {
        fU1 += fU1dA;
        fV1 += fV1dA;
      }

      if constexpr (Fog) {
        fFog += fFogdA;
      }

      pCB += colorStride;
    } while (pCB < pCBEnd);
  }
};

template <bool Depth, bool Color, uint32_t Texture0, uint32_t Texture1,
          bool Fog>
class GenericScanlineP {
public:
  enum {
    DEPTH_SIZE = Depth ? 1 : 0,
    COLOR_SIZE = Color ? 4 : 0,
    RHW_SIZE = 1,
    TEX0_SIZE = Texture0 ? 2 : 0,
    TEX1_SIZE = Texture1 ? 2 : 0,
    MIP0_SIZE = Texture0 - TEX0_SIZE,
    MIP1_SIZE = Texture1 - TEX1_SIZE,
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
  };

  inline static void Execute(const RasterData &rasterData, int y, int lx,
                             int rx) {
    if constexpr (Texture0 != 0) {
      GenericScanlineP<Depth, Color, Texture0, Texture1, Fog>::__Execute(rasterData, y, lx, rx);
    } else {
      __unused(rasterData);
      __unused(y);
      __unused(lx);
      __unused(rx);
    }
  }

  inline static void __Execute(const RasterData &rasterData, int y, int lx,
                               int rx) {
    auto pRasterOp = reinterpret_cast<const GenericRasterOp *>(rasterData.pRasterOp);
    auto rasterFlags = pRasterOp->getRasterID().Flags;

    auto pColorBits = rasterData.pColorBits;
    auto colorPitch = rasterData.ColorPitch;
    auto colorStride = pRasterOp->getColorStride();

    auto pDepthStencilBits = rasterData.pDepthStencilBits;
    auto depthStencilPitch = rasterData.DepthStencilPitch;
    auto depthStencilStride = pRasterOp->getDepthStencilStride();

    auto fOffsetX = fixed4(lx) - rasterData.fRefX;
    auto fOffsetY = fixed4(y) - rasterData.fRefY;

    fixedRX fZ, fZdA;

    if constexpr (Depth) {
      fZdA = rasterData.Registers[REG_DEPTH + 0].m[0];
      fixedRX fZdB = rasterData.Registers[REG_DEPTH + 0].m[1];
      fixedRX fZdC = rasterData.Registers[REG_DEPTH + 0].m[2];
      fZ = fZdA * fOffsetX + fZdB * fOffsetY + fZdC;
    }

    fixedRX fB, fG, fR, fA;
    fixedRX fBdA, fGdA, fRdA, fAdA;

    if constexpr (Color) {
      fBdA = rasterData.Registers[REG_COLOR + 0].m[0];
      fixedRX fBdB = rasterData.Registers[REG_COLOR + 0].m[1];
      fixedRX fBdC = rasterData.Registers[REG_COLOR + 0].m[2];
      fB = fBdA * fOffsetX + fBdB * fOffsetY + fBdC;

      fGdA = rasterData.Registers[REG_COLOR + 1].m[0];
      fixedRX fGdB = rasterData.Registers[REG_COLOR + 1].m[1];
      fixedRX fGdC = rasterData.Registers[REG_COLOR + 1].m[2];
      fG = fGdA * fOffsetX + fGdB * fOffsetY + fGdC;

      fRdA = rasterData.Registers[REG_COLOR + 2].m[0];
      fixedRX fRdB = rasterData.Registers[REG_COLOR + 2].m[1];
      fixedRX fRdC = rasterData.Registers[REG_COLOR + 2].m[2];
      fR = fRdA * fOffsetX + fRdB * fOffsetY + fRdC;

      fAdA = rasterData.Registers[REG_COLOR + 3].m[0];
      fixedRX fAdB = rasterData.Registers[REG_COLOR + 3].m[1];
      fixedRX fAdC = rasterData.Registers[REG_COLOR + 3].m[2];
      fA = fAdA * fOffsetX + fAdB * fOffsetY + fAdC;
    }

    fixedRX fRhw, fRhwdA;
    fixedRX fU0OverW, fV0OverW, fU1OverW, fV1OverW;
    fixedRX fU0OverWdA, fV0OverWdA, fU1OverWdA, fV1OverWdA;
    fixedRX fM0OverW2, fM1OverW2;
    fixedRX fM0OverW2dA, fM1OverW2dA;

    if constexpr (Texture0 != 0) {
      fRhwdA = rasterData.Registers[REG_RHW + 0].m[0];
      fixedRX fRhwdB = rasterData.Registers[REG_RHW + 0].m[1];
      fixedRX fRhwdC = rasterData.Registers[REG_RHW + 0].m[2];
      fRhw = fRhwdA * fOffsetX + fRhwdB * fOffsetY + fRhwdC;

      fU0OverWdA = rasterData.Registers[REG_TEX0 + 0].m[0];
      fixedRX fU0OverWdB = rasterData.Registers[REG_TEX0 + 0].m[1];
      fixedRX fU0OverWdC = rasterData.Registers[REG_TEX0 + 0].m[2];
      fU0OverW = fU0OverWdA * fOffsetX + fU0OverWdB * fOffsetY + fU0OverWdC;

      fV0OverWdA = rasterData.Registers[REG_TEX0 + 1].m[0];
      fixedRX fV0OverWdB = rasterData.Registers[REG_TEX0 + 1].m[1];
      fixedRX fV0OverWdC = rasterData.Registers[REG_TEX0 + 1].m[2];
      fV0OverW = fV0OverWdA * fOffsetX + fV0OverWdB * fOffsetY + fV0OverWdC;

      if constexpr (2 == Texture0) {
        fM0OverW2dA = rasterData.Registers[REG_MIP0 + 0].m[0];
        fM0OverW2 = rasterData.Registers[REG_MIP0 + 0].m[2];

        if (rasterFlags.InterpolateMips & 1) {
          fixedRX fM0OverW2dB = rasterData.Registers[REG_MIP0 + 0].m[1];
          fM0OverW2 += fM0OverW2dA * fOffsetX + fM0OverW2dB * fOffsetY;
        }
      }

      if constexpr (Texture1 != 0) {
        fU1OverWdA = rasterData.Registers[REG_TEX1 + 0].m[0];
        fixedRX fU1OverWdB = rasterData.Registers[REG_TEX1 + 0].m[1];
        fixedRX fU1OverWdC = rasterData.Registers[REG_TEX1 + 0].m[2];
        fU1OverW = fU1OverWdA * fOffsetX + fU1OverWdB * fOffsetY + fU1OverWdC;

        fV1OverWdA = rasterData.Registers[REG_TEX1 + 1].m[0];
        fixedRX fV1OverWdB = rasterData.Registers[REG_TEX1 + 1].m[1];
        fixedRX fV1OverWdC = rasterData.Registers[REG_TEX1 + 1].m[2];
        fV1OverW = fV1OverWdA * fOffsetX + fV1OverWdB * fOffsetY + fV1OverWdC;

        if constexpr (2 == Texture1) {
          fM1OverW2dA = rasterData.Registers[REG_MIP1 + 0].m[0];
          fM1OverW2 = rasterData.Registers[REG_MIP1 + 0].m[2];

          if (rasterFlags.InterpolateMips & 2) {
            fixedRX fM1OverW2dB = rasterData.Registers[REG_MIP1 + 0].m[1];
            fM1OverW2 += fM1OverW2dA * fOffsetX + fM1OverW2dB * fOffsetY;
          }
        }
      }
    }

    fixedRX fFog, fFogdA;

    if constexpr (Fog) {
      fFogdA = rasterData.Registers[REG_FOG + 0].m[0];
      fixedRX fFogdB = rasterData.Registers[REG_FOG + 0].m[1];
      fixedRX fFogdC = rasterData.Registers[REG_FOG + 0].m[2];
      fFog = fFogdA * fOffsetX + fFogdB * fOffsetY + fFogdC;
    }

    fixedW fW = Math::Inverse<fixedW>(fRhw);
    fixedW fW2;

    if constexpr ((2 == Texture0) || (2 == Texture1)) {
      fW2 = fW * fW;
    }

    fixedRX fU0, fV0, fM0;

    if constexpr (Texture0 != 0) {
      fU0 = fU0OverW * fW;
      fV0 = fV0OverW * fW;

      if constexpr (2 == Texture0) {
        fM0 = fM0OverW2;

        if (rasterFlags.InterpolateMips & 1) {
          fM0 *= fW2;
        }
      }
    }

    fixedRX fU1, fV1, fM1;

    if constexpr (Texture1 != 0) {
      fU1 = fU1OverW * fW;
      fV1 = fV1OverW * fW;

      if constexpr (2 == Texture1) {
        fM1 = fM1OverW2;

        if (rasterFlags.InterpolateMips & 2) {
          fM1 *= fW2;
        }
      }
    }

    auto pDS = lx * depthStencilStride + y * depthStencilPitch + pDepthStencilBits;
    auto pCB = lx * colorStride + y * colorPitch + pColorBits;
    auto width = rx - lx;

    ColorARGB cColor(0xff, 0xff, 0xff, 0xff);

    do {
      uint32_t blockWidth1 = std::min(width, MAX_BLOCK_SIZE);
      uint32_t log2width = Math::iLog2(blockWidth1);
      uint32_t blockWidth = 1 << log2width;
      width -= blockWidth;
      auto pCBEnd = pCB + blockWidth * colorStride;

      fixedRX fdUr0, fdVr0, fdMr0;
      fixedRX fdUr1, fdVr1, fdMr1;

      if (log2width) {
        fRhw += fRhwdA << log2width;
        fixedW fWr = Math::Inverse<fixedW>(fRhw);
        fixedW fWr2;

        if constexpr ((2 == Texture0) || (2 == Texture1)) {
          fWr2 = fWr * fWr;
        }

        if constexpr (Texture0 != 0) {
          fU0OverW += fU0OverWdA << log2width;
          fV0OverW += fV0OverWdA << log2width;
          fdUr0 = (fU0OverW * fWr - fU0) >> log2width;
          fdVr0 = (fV0OverW * fWr - fV0) >> log2width;

          if constexpr (2 == Texture0) {
            if (rasterFlags.InterpolateMips & 1) {
              fM0OverW2 += fM0OverW2dA << log2width;
              fdMr0 = (fM0OverW2 * fWr2 - fM0) >> log2width;
            } else {
              fdMr0 = Math::Zero<fixedRX>();
            }
          }
        }

        if constexpr (Texture1 != 0) {
          fU1OverW += fU1OverWdA << log2width;
          fV1OverW += fV1OverWdA << log2width;
          fdUr1 = (fU1OverW * fWr - fU1) >> log2width;
          fdVr1 = (fV1OverW * fWr - fV1) >> log2width;

          if constexpr (2 == Texture1) {
            if (rasterFlags.InterpolateMips & 2) {
              fM1OverW2 += fM1OverW2dA << log2width;
              fdMr1 = (fM1OverW2 * fWr2 - fM1) >> log2width;
            } else {
              fdMr1 = Math::Zero<fixedRX>();
            }
          }
        }
      }

      do {
        for (;;) {
          uint32_t depthValue = 0;

          if constexpr (Depth) {
            depthValue = static_cast<fixed16>(fZ).data() & 0xFFFF;

            // Execute early out depth test
            if (!rasterFlags.StencilTest &&
                !pRasterOp->doDepthTest(depthValue, pDS)) {
              break;
            }
          }

          if constexpr (Color) {
            cColor = ToColor4(fA, fR, fG, fB);
          }

          // Execute texture operations
          if constexpr (Texture0 != 0) {
            ColorARGB cTexture;

            if constexpr (2 == Texture0) {
              cTexture = pRasterOp->getSamplerColor(0, rasterData, fU0, fV0, fM0);
            } else {
              cTexture = pRasterOp->getSamplerColor(0, rasterData, fU0, fV0);
            }

            cColor = pRasterOp->getTexEnvColor(cColor, cTexture, 0, rasterData);

            if constexpr (Texture1 != 0) {
              if constexpr (2 == Texture1) {
                cTexture = pRasterOp->getSamplerColor(1, rasterData, fU1, fV1, fM1);
              } else {
                cTexture = pRasterOp->getSamplerColor(1, rasterData, fU1, fV1);
              }

              cColor = pRasterOp->getTexEnvColor(cColor, cTexture, 1, rasterData);
            }
          }

          if (rasterFlags.AlphaTest) {
            // Execute alpha test
            if (!pRasterOp->doAlphaTest(rasterData, cColor)) {
              break;
            }
          }

          if (rasterFlags.StencilTest) {
            // Execute stencil test
            if (!pRasterOp->doStencilTest(rasterData, depthValue, pDS)) {
              break;
            }
          } else {
            if constexpr (Depth) {
              if (rasterFlags.DepthWrite) {
                // Write output depth
                *reinterpret_cast<uint16_t *>(pDS) =
                    static_cast<uint16_t>(depthValue);
              }
            }
          }

          if constexpr (Fog) {
            // Execute fog blend
            cColor = pRasterOp->applyFog(rasterData, cColor, fFog);
          }

          if (rasterFlags.Blend) {
            // Execute pixel blend
            cColor = pRasterOp->doBlend(cColor, pCB);
          }

          // Write output color
          pRasterOp->writeColor(rasterData, cColor, pCB);

          break;
        }

        pDS += depthStencilStride;

        if constexpr (Depth) {
          fZ += fZdA;
        }

        if constexpr (Color) {
          fB += fBdA;
          fG += fGdA;
          fR += fRdA;
          fA += fAdA;
        }

        if constexpr (Texture0 != 0) {
          fU0 += fdUr0;
          fV0 += fdVr0;

          if constexpr (2 == Texture0) {
            fM0 += fdMr0;
          }
        }

        if constexpr (Texture1 != 0) {
          fU1 += fdUr1;
          fV1 += fdVr1;

          if constexpr (2 == Texture1) {
            fM1 += fdMr1;
          }
        }

        if constexpr (Fog) {
          fFog += fFogdA;
        }

        pCB += colorStride;
      } while (pCB < pCBEnd);
    } while (width);
  }
};