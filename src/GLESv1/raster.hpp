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
      int FRAC = D::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      auto half = static_cast<int64_t>(1) << (FRAC - 1);
      int64_t uv0 = static_cast<int64_t>(delta0.data()) * this->fdY1.data();
      int64_t uv1 = static_cast<int64_t>(delta1.data()) * this->fdY0.data();
      return T::make(((uv0 - uv1) * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>((delta0 * this->fdY1 - delta1 * this->fdY0) *
                            this->fRatio);
#endif
    }

    template <class T, class D> T TCalcDeltaY(D delta0, D delta1) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = D::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      auto half = static_cast<int64_t>(1) << (FRAC - 1);
      int64_t uv0 = static_cast<int64_t>(delta1.data()) * this->fdX0.data();
      int64_t uv1 = static_cast<int64_t>(delta0.data()) * this->fdX1.data();
      return T::make(((uv0 - uv1) * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>((delta1 * this->fdX0 - delta0 * this->fdX1) *
                            this->fRatio);
#endif
    }

    template <class T> T TCalcDeltaX(int delta0, int delta1) const {
      auto diff = this->fdY1 * delta0 - this->fdY0 * delta1;
#ifdef COCOGL_PIXEDPOINT
      int FRAC = fixed8::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((diff.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(fixed12::make(diff.data()) * this->fRatio);
#endif
    }

    template <class T> T TCalcDeltaY(int delta0, int delta1) const {
      auto diff = this->fdX0 * delta1 - this->fdX1 * delta0;
#ifdef COCOGL_PIXEDPOINT
      int FRAC = fixed8::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((diff.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(fixed12::make(diff.data()) * this->fRatio);
#endif
    }
  };

  struct LineGradient {
    floatQ fRatio;

    template <class T, class D> T TCalcDelta(D delta) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = D::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((delta.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(delta * this->fRatio);
#endif
    }

    template <class T> T TCalcDelta(int delta) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = fixed8::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((delta * this->fRatio.data() + half) >> FRAC);
#else
      return T::make(
          static_cast<TFixed<T::FRAC - fixed8::FRAC>>(delta * this->fRatio)
              .data());
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
    Color4 tmp(Math::TToUNORM8(Math::TSat(vClearColor_.w)),
               Math::TToUNORM8(Math::TSat(vClearColor_.x)),
               Math::TToUNORM8(Math::TSat(vClearColor_.y)),
               Math::TToUNORM8(Math::TSat(vClearColor_.z)));
    pSurfDraw_->ConvertColor(&clearColor_, tmp);
    dirtyFlags_.ClearColor = 0;
  }

  void EnsureClearDepth() {
    clearDepth_ = Math::TToUNORM16(Math::TSat(fClearDepth_));
    dirtyFlags_.ClearDepth = 0;
  }

  void EnsureColorWriteMask() {
    Color4 tmp(cColorWriteMask_);
    pSurfDraw_->ConvertColor(&rasterData_.ColorWriteMask, tmp);
    dirtyFlags_.ColorWriteMask = 0;
  }

  void EnsureFogColor() {
    rasterData_.cFogColor.a =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(vFogColor_.w)));
    rasterData_.cFogColor.r =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(vFogColor_.x)));
    rasterData_.cFogColor.g =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(vFogColor_.y)));
    rasterData_.cFogColor.b =
        static_cast<uint8_t>(Math::TToUNORM8(Math::TSat(vFogColor_.z)));
    dirtyFlags_.FogColor = 0;
  }

  PolygonOffset polygonOffset_;
  floatf fLineWidth_;
  Rect scissorRect_;

  RasterData rasterData_;

  ColorARGB cColorWriteMask_;
  uint32_t depthWriteMask_;
  uint32_t stencilWriteMask_;
  uint32_t clearColor_;
  uint32_t clearDepth_;

  int clearStencil_;
  floatf fClearDepth_;
  VECTOR4 vClearColor_;
  VECTOR4 vFogColor_;
};
