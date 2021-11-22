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

typedef bool (*Pfncompare)(uint32_t a, uint32_t b);

typedef ColorARGB (*PfnGetTexelColor)(const SurfaceDesc &surface,
                                      fixedRX fU, 
                                      fixedRX fV);

typedef ColorARGB (*PfnGetSamplerColor)(
    PfnGetTexelColor pfnGetTexelColorMin,
    PfnGetTexelColor pfnGetTexelColorMag,
    const Sampler &sampler, 
    fixedRX fU,
    fixedRX fV, 
    fixedRX fM);

typedef ColorARGB (*PfnGetTexEnvColor)(const ColorARGB &cColor, 
                                       const ColorARGB &cTexture,
                                       const ColorARGB &cEnvColor);

typedef ColorARGB (*PfnBlend)(const ColorARGB &cColor, const uint8_t *pCB);

typedef void (*PfnWriteColor)(const RasterData &rasterData,
                              const ColorARGB &cSrcColor, 
                              uint8_t *pCB);

class GenericRasterOp : public IRasterOp {
public:
  GenericRasterOp(const RASTERID &rasterID);
  ~GenericRasterOp();

  PfnScanline getScanline() const {
    return pfnScanline_;
  }

  uint32_t getCbSize() const {
    return sizeof(PfnScanline);
  }

  const RASTERID &getRasterID() const {
    return rasterID_;
  }

  uint8_t getColorStride() const {
    return colorStride_;
  }

  uint8_t getDepthStencilStride() const {
    return depthStencilStride_;
  }

  ColorARGB getSamplerColor(uint32_t unit, 
                            const RasterData &rasterData, 
                            fixedRX fU,
                            fixedRX fV) const {
    return (samplers_[unit].pfnGetTexelColorMag)(
        rasterData.Samplers[unit].pMipLevels[0], fU, fV);
  }

  ColorARGB getSamplerColor(uint32_t unit,
                            const RasterData &rasterData, 
                            fixedRX fU, 
                            fixedRX fV,
                            fixedRX fM) const;

  ColorARGB getTexEnvColor(const ColorARGB &cColor, 
                           const ColorARGB &in, 
                           uint32_t unit,
                           const RasterData &rasterData) const {
    return (samplers_[unit].pfnGetTexEnvColor)(
        cColor, in, rasterData.Samplers[unit].cEnvColor_MaxMipLevel);
  }

  bool doAlphaTest(const RasterData &rasterData, const ColorARGB &cColor) const {
    return (pfnAlphaTest_)(cColor.a, rasterData.AlphaRef);
  }

  bool doStencilTest(const RasterData &rasterData, 
                     uint32_t depthValue,
                     void *pDSBuffer) const;

  bool doDepthTest(uint32_t depthValue, void *pDSBuffer) const {
    assert(pDSBuffer);
    return (pfnDepthTest_)(depthValue, *reinterpret_cast<const uint16_t *>(pDSBuffer));
  }

  static ColorARGB applyFog(const RasterData &rasterData, 
                            const ColorARGB &cColor,
                            fixedRX fFactor);

  ColorARGB doBlend(const ColorARGB &cColor, const uint8_t *pCB) const {
    return (pfnBlend_)(cColor, pCB);
  }

  void writeColor(const RasterData &rasterData, 
                  const ColorARGB &cColor,
                  uint8_t *pCB) const {
    (pfnWriteColor_)(rasterData, cColor, pCB);
  }

  static GLenum Create(IRasterOp **ppRasterOp, const RASTERID &rasterID);

private:
  struct _Sampler {
    PfnGetTexelColor  pfnGetTexelColorMin;
    PfnGetTexelColor  pfnGetTexelColorMag;
    PfnGetTexEnvColor pfnGetTexEnvColor;

    _Sampler()
        : pfnGetTexelColorMin(nullptr)
        , pfnGetTexelColorMag(nullptr),
          pfnGetTexEnvColor(nullptr) 
        {}
  };

  GLenum initialize();

  void selectDepthStencilFunc();
  void selectSamplerFunc();
  void selectTexEnvFunc();
  void selectAlphaTestFunc();
  void selectBlendFunc();
  void selectWriteColorFunc();

  PfnScanline pfnScanline_;

  RASTERID rasterID_;

  _Sampler samplers_[MAX_TEXTURES];

  Pfncompare    pfnAlphaTest_;
  Pfncompare    pfnDepthTest_;
  Pfncompare    pfnStencilTest_;
  PfnBlend      pfnBlend_;
  PfnWriteColor pfnWriteColor_;

  uint8_t colorStride_;
  uint8_t depthStencilStride_;
};
