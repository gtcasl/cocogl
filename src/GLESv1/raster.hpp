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

#include "tnl.hpp"

class CRasterizer : public CTNL {
public:
  GLenum RenderPrimitive(GLenum mode, uint32_t count);

  template <class T>
  GLenum TRenderIndexedPrimitive(GLenum mode, const T *pIndices, uint32_t count,
                                 uint32_t startVertex);

  void PostRender();

protected:
  struct TriangleGradient {
    fixed4 fdX0;
    fixed4 fdY0;
    fixed4 fdX1;
    fixed4 fdY1;
    floatQ fRatio;

    TriangleGradient(fixed4 fdX0_, fixed4 fdY0_, fixed4 fdX1_, fixed4 fdY1_,
                     floatQ fRatio_)
        : fdX0(fdX0_), fdY0(fdY0_), fdX1(fdX1_), fdY1(fdY1_), fRatio(fRatio_) {}

    template <class T, class D> T TCalcDeltaX(D delta0, D delta1) const {
#ifdef COCOGL_PIXEDPOINT
      const int FRAC = D::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      const int64_t half = static_cast<int64_t>(1) << (FRAC - 1);
      const int64_t uv0 =
          static_cast<int64_t>(delta0.GetRaw()) * this->fdY1.GetRaw();
      const int64_t uv1 =
          static_cast<int64_t>(delta1.GetRaw()) * this->fdY0.GetRaw();
      return T::Make(((uv0 - uv1) * this->fRatio.GetRaw() + half) >> FRAC);
#else
      return static_cast<T>((delta0 * this->fdY1 - delta1 * this->fdY0) *
                            this->fRatio);
#endif
    }

    template <class T, class D> T TCalcDeltaY(D delta0, D delta1) const {
#ifdef COCOGL_PIXEDPOINT
      const int FRAC = D::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      const int64_t half = static_cast<int64_t>(1) << (FRAC - 1);
      const int64_t uv0 =
          static_cast<int64_t>(delta1.GetRaw()) * this->fdX0.GetRaw();
      const int64_t uv1 =
          static_cast<int64_t>(delta0.GetRaw()) * this->fdX1.GetRaw();
      return T::Make(((uv0 - uv1) * this->fRatio.GetRaw() + half) >> FRAC);
#else
      return static_cast<T>((delta1 * this->fdX0 - delta0 * this->fdX1) *
                            this->fRatio);
#endif
    }

    template <class T> T TCalcDeltaX(int delta0, int delta1) const {
      const fixed4 diff = this->fdY1 * delta0 - this->fdY0 * delta1;
#ifdef COCOGL_PIXEDPOINT
      const int FRAC = fixed8::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      const int half = 1 << (FRAC - 1);
      return T::Make((diff.GetRaw() * this->fRatio.GetRaw() + half) >> FRAC);
#else
      return static_cast<T>(fixed12::Make(diff.GetRaw()) * this->fRatio);
#endif
    }

    template <class T> T TCalcDeltaY(int delta0, int delta1) const {
      const fixed4 diff = this->fdX0 * delta1 - this->fdX1 * delta0;
#ifdef COCOGL_PIXEDPOINT
      const int FRAC = fixed8::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      const int half = 1 << (FRAC - 1);
      return T::Make((diff.GetRaw() * this->fRatio.GetRaw() + half) >> FRAC);
#else
      return static_cast<T>(fixed12::Make(diff.GetRaw()) * this->fRatio);
#endif
    }
  };

  struct LineGradient {
    floatQ fRatio;

    template <class T, class D> T TCalcDelta(D delta) const {
#ifdef COCOGL_PIXEDPOINT
      const int FRAC = D::FRAC + floatQ::FRAC - T::FRAC;
      const int half = 1 << (FRAC - 1);
      return T::Make((delta.GetRaw() * this->fRatio.GetRaw() + half) >> FRAC);
#else
      return static_cast<T>(delta * this->fRatio);
#endif
    }

    template <class T> T TCalcDelta(int delta) const {
#ifdef COCOGL_PIXEDPOINT
      const int FRAC = fixed8::FRAC + floatQ::FRAC - T::FRAC;
      const int half = 1 << (FRAC - 1);
      return T::Make((delta * this->fRatio.GetRaw() + half) >> FRAC);
#else
      return T::Make(
          static_cast<TFixed<T::FRAC - fixed8::FRAC>>(delta * this->fRatio)
              .GetRaw());
#endif
    }
  };

  CRasterizer() {}

  ~CRasterizer() {}

  GLenum SetupRasterStates(GLenum mode);

  bool GenerateRasterOp();

  void UpdateScissorRect();

  void DrawTriangle(uint32_t i0, uint32_t i1, uint32_t i2);
  void DrawLine(uint32_t i0, uint32_t i1);
  void DrawPoint(uint32_t index);

  bool CullScreenSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  bool CullClipSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void RasterClippedTriangle(uint32_t i0, uint32_t i1, uint32_t i2,
                             uint32_t clipUnion);

  void RasterClippedLine(uint32_t i0, uint32_t i1, uint32_t clipUnion);

  template <eClipFlags ClipFlags>
  uint32_t TClipTriangle(uint32_t nNumVertices, uint32_t *pSrc, uint32_t *pDst,
                         uint32_t *pTmp);

  uint32_t UserClipTriangle(uint32_t plane, uint32_t nNumVertices,
                            uint32_t *pSrc, uint32_t *pDst, uint32_t *pTmp);

  void InterpolateVertex(uint32_t i0, uint32_t i1, floatf fDistA, floatf fDistB,
                         uint32_t i2);

  void RasterTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void RasterLine(uint32_t i0, uint32_t i1);

  void RasterPoint(uint32_t index);

  bool SetupLineAttributes(LineGradient *pGradient, uint32_t i0, uint32_t i1);

  bool SetupTriangleAttributes(uint32_t i0, uint32_t i1, uint32_t i2);

  Register *DepthGradient(Register *pRegister, const TriangleGradient &gradient,
                          const RDVECTOR &v0, const RDVECTOR &v1,
                          const RDVECTOR &v2);

  void ApplyPolygonOffset(Register *pRegister);

  Register *ColorGradient(Register *pRegister, const TriangleGradient &gradient,
                          uint32_t i0, uint32_t i1, uint32_t i2);

  Register *AffineTextureGradient(Register *pRegister,
                                  const TriangleGradient &gradient, uint32_t i0,
                                  uint32_t i1, uint32_t i2);

  Register *PerspectiveTextureGradient(Register *pRegister,
                                       const TriangleGradient &gradient,
                                       uint32_t i0, uint32_t i1, uint32_t i2);

  Register *AffineTextureMipmapGradient(Register *pRegister);

  Register *PerspectiveTextureMipmapGradient(
      Register *pRegister, const TriangleGradient &gradient, const RDVECTOR &v0,
      const RDVECTOR &v1, const RDVECTOR &v2, floatRW rhws[3]);

  Register *FogGradient(Register *pRegister, const TriangleGradient &gradient,
                        uint32_t i0, uint32_t i1, uint32_t i2);

  void EnsureClearColor() {
    Color4 tmp(Math::TToUNORM8(Math::TSat(m_vClearColor.w)),
               Math::TToUNORM8(Math::TSat(m_vClearColor.x)),
               Math::TToUNORM8(Math::TSat(m_vClearColor.y)),
               Math::TToUNORM8(Math::TSat(m_vClearColor.z)));
    m_pSurfDraw->ConvertColor(&m_clearColor, tmp);
    m_dirtyFlags.ClearColor = 0;
  }

  void EnsureClearDepth() {
    m_clearDepth = Math::TToUNORM16(Math::TSat(m_fClearDepth));
    m_dirtyFlags.ClearDepth = 0;
  }

  void EnsureColorWriteMask() {
    Color4 tmp(m_cColorWriteMask);
    m_pSurfDraw->ConvertColor(&m_rasterData.ColorWriteMask, tmp);
    m_dirtyFlags.ColorWriteMask = 0;
  }

  void EnsureFogColor() {
    m_rasterData.cFogColor.a =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(m_vFogColor.w)));
    m_rasterData.cFogColor.r =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(m_vFogColor.x)));
    m_rasterData.cFogColor.g =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(m_vFogColor.y)));
    m_rasterData.cFogColor.b =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(m_vFogColor.z)));
    m_dirtyFlags.FogColor = 0;
  }

  PolygonOffset m_polygonOffset;
  floatf m_fLineWidth;
  Rect m_scissorRect;

  RasterData m_rasterData;

  ColorARGB m_cColorWriteMask;
  uint32_t m_depthWriteMask;
  uint32_t m_stencilWriteMask;
  uint32_t m_clearColor;
  uint32_t m_clearDepth;

  int m_clearStencil;
  floatf m_fClearDepth;
  VECTOR4 m_vClearColor;
  VECTOR4 m_vFogColor;
};
