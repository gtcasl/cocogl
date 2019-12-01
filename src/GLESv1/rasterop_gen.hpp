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

typedef void (*PfnGetTexelColor)(Color4 *pOut, const SurfaceDesc &surface,
                                  fixedRX fU, fixedRX fV);

typedef void (*PfnGetSamplerColor)(Color4 *pOut,
                                    PfnGetTexelColor pfnGetTexelColorMin,
                                    PfnGetTexelColor pfnGetTexelColorMag,
                                    const Sampler &sampler, fixedRX fU,
                                    fixedRX fV, fixedRX fM);

typedef void (*PfnGetTexEnvColor)(Color4 *pInOut, const Color4 &cTexture,
                                   ColorARGB cEnvColor);

typedef void (*PfnBlend)(Color4 *pInOut, const uint8_t *pCB);

typedef void (*PfnWriteColor)(const RasterData &rasterData,
                               const Color4 &cSrcColor, uint8_t *pCB);

class CGenericRasterOp : public IRasterOp {
public:
  CGenericRasterOp(const RASTERID &rasterID);
  ~CGenericRasterOp();

  PfnScanline getScanline() const { return pfnScanline_; }

  uint32_t getCbSize() const { return sizeof(PfnScanline); }

  const RASTERID &getRasterID() const { return rasterID_; }

  uint8_t getColorStride() const { return colorStride_; }

  uint8_t getDepthStencilStride() const { return depthStencilStride_; }

  void getSamplerColor(Color4 *pOut, uint32_t unit,
                       const RasterData &rasterData, fixedRX fU,
                       fixedRX fV) const {
    (samplers_[unit].pfnGetTexelColorMag)(
        pOut, rasterData.Samplers[unit].pMipLevels[0], fU, fV);
  }

  void getSamplerColor(Color4 *pOut, uint32_t unit,
                       const RasterData &rasterData, fixedRX fU, fixedRX fV,
                       fixedRX fM) const;

  void getTexEnvColor(Color4 *pInOut, const Color4 &in, uint32_t unit,
                      const RasterData &rasterData) const {
    (samplers_[unit].pfnGetTexEnvColor)(
        pInOut, in, rasterData.Samplers[unit].cEnvColor_MaxMipLevel);
  }

  bool doAlphaTest(const RasterData &rasterData, const Color4 &cColor) const {
    return (pfnAlphaTest_)(cColor.a, rasterData.AlphaRef);
  }

  bool doStencilTest(const RasterData &rasterData, uint32_t depthValue,
                     void *pDSBuffer) const;

  bool doDepthTest(uint32_t depthValue, void *pDSBuffer) const {
    assert(pDSBuffer);
    return (pfnDepthTest_)(depthValue,
                           *reinterpret_cast<const uint16_t *>(pDSBuffer));
  }

  static void applyFog(const RasterData &rasterData, Color4 *pInOut,
                       fixedRX fFactor);

  void doBlend(Color4 *pInOut, const uint8_t *pCB) const {
    (pfnBlend_)(pInOut, pCB);
  }

  void writeColor(const RasterData &rasterData, const Color4 &cColor,
                  uint8_t *pCB) const {
    (pfnWriteColor_)(rasterData, cColor, pCB);
  }

  static GLenum Create(IRasterOp **ppRasterOp, const RASTERID &rasterID);

private:
  struct _Sampler {
    PfnGetTexelColor pfnGetTexelColorMin;
    PfnGetTexelColor pfnGetTexelColorMag;
    PfnGetTexEnvColor pfnGetTexEnvColor;

    _Sampler()
        : pfnGetTexelColorMin(nullptr), pfnGetTexelColorMag(nullptr),
          pfnGetTexEnvColor(nullptr) {}
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

  Pfncompare pfnAlphaTest_;
  Pfncompare pfnDepthTest_;
  Pfncompare pfnStencilTest_;
  PfnBlend pfnBlend_;
  PfnWriteColor pfnWriteColor_;

  uint8_t colorStride_;
  uint8_t depthStencilStride_;
};
