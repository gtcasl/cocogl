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

typedef bool (*PFN_Compare)(unsigned a, unsigned b);

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

  PFN_Scanline GetScanline() const { return m_pfnScanline; }

  unsigned GetCbSize() const { return sizeof(PFN_Scanline); }

  const RASTERID &GetRasterID() const { return m_rasterID; }

  uint8_t GetColorStride() const { return m_colorStride; }

  uint8_t GetDepthStencilStride() const { return m_depthStencilStride; }

  void GetSamplerColor(Color4 *pOut, unsigned unit,
                       const RasterData &rasterData, fixedRX fU,
                       fixedRX fV) const {
    (m_samplers[unit].pfnGetTexelColorMag)(
        pOut, rasterData.Samplers[unit].pMipLevels[0], fU, fV);
  }

  void GetSamplerColor(Color4 *pOut, unsigned unit,
                       const RasterData &rasterData, fixedRX fU, fixedRX fV,
                       fixedRX fM) const;

  void GetTexEnvColor(Color4 *pInOut, const Color4 &in, unsigned unit,
                      const RasterData &rasterData) const {
    (m_samplers[unit].pfnGetTexEnvColor)(
        pInOut, in, rasterData.Samplers[unit].cEnvColor_MaxMipLevel);
  }

  bool DoAlphaTest(const RasterData &rasterData, const Color4 &cColor) const {
    return (m_pfnAlphaTest)(cColor.a, rasterData.AlphaRef);
  }

  bool DoStencilTest(const RasterData &rasterData, unsigned depthValue,
                     void *pDSBuffer) const;

  bool DoDepthTest(unsigned depthValue, void *pDSBuffer) const {
    ASSERT(pDSBuffer);
    return (m_pfnDepthTest)(depthValue,
                            *reinterpret_cast<const uint16_t *>(pDSBuffer));
  }

  static void ApplyFog(const RasterData &rasterData, Color4 *pInOut,
                       fixedRX fFactor);

  void DoBlend(Color4 *pInOut, const uint8_t *pCB) const {
    (m_pfnBlend)(pInOut, pCB);
  }

  void WriteColor(const RasterData &rasterData, const Color4 &cColor,
                  uint8_t *pCB) const {
    (m_pfnWriteColor)(rasterData, cColor, pCB);
  }

  static GLenum Create(IRasterOp **ppRasterOp, const RASTERID &rasterID);

private:
  struct _Sampler {
    PFN_GetTexelColor pfnGetTexelColorMin;
    PFN_GetTexelColor pfnGetTexelColorMag;
    PFN_GetTexEnvColor pfnGetTexEnvColor;

    _Sampler()
        : pfnGetTexelColorMin(NULL), pfnGetTexelColorMag(NULL),
          pfnGetTexEnvColor(NULL) {}
  };

  GLenum Initialize();

  void SelectDepthStencilFunc();
  void SelectSamplerFunc();
  void SelectTexEnvFunc();
  void SelectAlphaTestFunc();
  void SelectBlendFunc();
  void SelectWriteColorFunc();

  PFN_Scanline m_pfnScanline;

  RASTERID m_rasterID;

  _Sampler m_samplers[MAX_TEXTURES];

  PFN_Compare m_pfnAlphaTest;
  PFN_Compare m_pfnDepthTest;
  PFN_Compare m_pfnStencilTest;
  PFN_Blend m_pfnBlend;
  PFN_WriteColor m_pfnWriteColor;

  uint8_t m_colorStride;
  uint8_t m_depthStencilStride;
};
