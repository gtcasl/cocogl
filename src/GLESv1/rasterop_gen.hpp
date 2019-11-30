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

typedef bool (*PFN_Compare)(uint32_t a, uint32_t b);

typedef void (*PFN_GetTexelColor)(Color4 *pOut, const SurfaceDesc &surface,
                                  fixedRX fU, fixedRX fV);

typedef void (*PFN_GetSamplerColor)(Color4 *pOut,
                                    PFN_GetTexelColor pfnGetTexelColorMin,
                                    PFN_GetTexelColor pfnGetTexelColorMag,
                                    const Sampler &sampler, fixedRX fU,
                                    fixedRX fV, fixedRX fM);

typedef void (*PFN_GetTexEnvColor)(Color4 *pInOut, const Color4 &cTexture,
                                   ColorARGB cEnvColor);

typedef void (*PFN_Blend)(Color4 *pInOut, const uint8_t *pCB);

typedef void (*PFN_WriteColor)(const RasterData &rasterData,
                               const Color4 &cSrcColor, uint8_t *pCB);

class CGenericRasterOp : public IRasterOp {
public:
  CGenericRasterOp(const RASTERID &rasterID);
  ~CGenericRasterOp();

  PFN_Scanline GetScanline() const { return pfnScanline_; }

  uint32_t GetCbSize() const { return sizeof(PFN_Scanline); }

  const RASTERID &GetRasterID() const { return rasterID_; }

  uint8_t GetColorStride() const { return colorStride_; }

  uint8_t GetDepthStencilStride() const { return depthStencilStride_; }

  void GetSamplerColor(Color4 *pOut, uint32_t unit,
                       const RasterData &rasterData, fixedRX fU,
                       fixedRX fV) const {
    (samplers_[unit].pfnGetTexelColorMag)(
        pOut, rasterData.Samplers[unit].pMipLevels[0], fU, fV);
  }

  void GetSamplerColor(Color4 *pOut, uint32_t unit,
                       const RasterData &rasterData, fixedRX fU, fixedRX fV,
                       fixedRX fM) const;

  void GetTexEnvColor(Color4 *pInOut, const Color4 &in, uint32_t unit,
                      const RasterData &rasterData) const {
    (samplers_[unit].pfnGetTexEnvColor)(
        pInOut, in, rasterData.Samplers[unit].cEnvColor_MaxMipLevel);
  }

  bool DoAlphaTest(const RasterData &rasterData, const Color4 &cColor) const {
    return (pfnAlphaTest_)(cColor.a, rasterData.AlphaRef);
  }

  bool DoStencilTest(const RasterData &rasterData, uint32_t depthValue,
                     void *pDSBuffer) const;

  bool DoDepthTest(uint32_t depthValue, void *pDSBuffer) const {
    assert(pDSBuffer);
    return (pfnDepthTest_)(depthValue,
                           *reinterpret_cast<const uint16_t *>(pDSBuffer));
  }

  static void ApplyFog(const RasterData &rasterData, Color4 *pInOut,
                       fixedRX fFactor);

  void DoBlend(Color4 *pInOut, const uint8_t *pCB) const {
    (pfnBlend_)(pInOut, pCB);
  }

  void WriteColor(const RasterData &rasterData, const Color4 &cColor,
                  uint8_t *pCB) const {
    (pfnWriteColor_)(rasterData, cColor, pCB);
  }

  static GLenum Create(IRasterOp **ppRasterOp, const RASTERID &rasterID);

private:
  struct _Sampler {
    PFN_GetTexelColor pfnGetTexelColorMin;
    PFN_GetTexelColor pfnGetTexelColorMag;
    PFN_GetTexEnvColor pfnGetTexEnvColor;

    _Sampler()
        : pfnGetTexelColorMin(nullptr), pfnGetTexelColorMag(nullptr),
          pfnGetTexEnvColor(nullptr) {}
  };

  GLenum Initialize();

  void SelectDepthStencilFunc();
  void SelectSamplerFunc();
  void SelectTexEnvFunc();
  void SelectAlphaTestFunc();
  void SelectBlendFunc();
  void SelectWriteColorFunc();

  PFN_Scanline pfnScanline_;

  RASTERID rasterID_;

  _Sampler samplers_[MAX_TEXTURES];

  PFN_Compare pfnAlphaTest_;
  PFN_Compare pfnDepthTest_;
  PFN_Compare pfnStencilTest_;
  PFN_Blend pfnBlend_;
  PFN_WriteColor pfnWriteColor_;

  uint8_t colorStride_;
  uint8_t depthStencilStride_;
};
