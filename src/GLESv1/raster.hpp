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
  GLenum renderPrimitive(GLenum mode, uint32_t count);

  template <class T>
  GLenum renderIndexedPrimitive(GLenum mode, const T *pIndices, uint32_t count,
                                 uint32_t startVertex);

  void postRender();

protected:

  struct TriangleGradient {
    fixed4 i4dx12;
    fixed4 i4dy12;
    fixed4 i4dx13;
    fixed4 i4dy13;
    fixed4 i4dx23;
    fixed4 i4dy23;
    floatQ fRatio;

    bool init(fixed4 i4x0, fixed4 i4y0, fixed4 i4x1, fixed4 i4y1, fixed4 i4x2, fixed4 i4y2) {
      this->i4dx12 = (i4x1 - i4x0);
      this->i4dy12 = (i4y1 - i4y0);
      this->i4dx13 = (i4x2 - i4x0);

      this->i4dy13 = (i4y2 - i4y0);
      this->i4dx23 = (i4x2 - i4x1);
      this->i4dy23 = (i4y2 - i4y1);

      // Calculate the edge direction
      auto i8Area = Math::TFastMul<fixed8>(this->i4dx12, this->i4dy13) -
                    Math::TFastMul<fixed8>(this->i4dx13, this->i4dy12);

      // Reject small areas (1/4 x 1/4 = 1/16)
      auto u8Area = Math::TAbs(i8Area);
      if (u8Area.data() < 16)
        return false;

      fRatio = Math::TInv<floatQ>(i8Area);
      return true;
    }

    template <class T, class D> T calcDeltaX(D delta0, D delta1) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = D::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      auto half = static_cast<int64_t>(1) << (FRAC - 1);
      int64_t uv0 = static_cast<int64_t>(delta0.data()) * this->i4dy13.data();
      int64_t uv1 = static_cast<int64_t>(delta1.data()) * this->i4dy12.data();
      return T::make(((uv0 - uv1) * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>((delta0 * this->i4dy13 - delta1 * this->i4dy12) * this->fRatio);
#endif
    }

    template <class T, class D> T calcDeltaY(D delta0, D delta1) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = D::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      auto half = static_cast<int64_t>(1) << (FRAC - 1);
      int64_t uv0 = static_cast<int64_t>(delta1.data()) * this->i4dx12.data();
      int64_t uv1 = static_cast<int64_t>(delta0.data()) * this->i4dx13.data();
      return T::make(((uv0 - uv1) * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>((delta1 * this->i4dx12 - delta0 * this->i4dx13) * this->fRatio);
#endif
    }

    template <class T> T calcDeltaX(int delta0, int delta1) const {
      auto diff = this->i4dy13 * delta0 - this->i4dy12 * delta1;
#ifdef COCOGL_PIXEDPOINT
      int FRAC = fixed8::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((diff.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(fixed12::make(diff.data()) * this->fRatio);
#endif
    }

    template <class T> T calcDeltaY(int delta0, int delta1) const {
      auto diff = this->i4dx12 * delta1 - this->i4dx13 * delta0;
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

    template <class T, class D> T calcDelta(D delta) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = D::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((delta.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(delta * this->fRatio);
#endif
    }

    template <class T> T calcDelta(int delta) const {
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

  GLenum setupRasterStates(GLenum mode);

  bool generateRasterOp();

  void updateScissorRect();

  void drawTriangle(uint32_t i0, uint32_t i1, uint32_t i2);
  
  void drawLine(uint32_t i0, uint32_t i1);

  void drawPoint(uint32_t index);

  bool cullScreenSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  bool cullClipSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void rasterClippedTriangle(uint32_t i0, uint32_t i1, uint32_t i2,
                             uint32_t clipUnion);

  void rasterClippedLine(uint32_t i0, uint32_t i1, uint32_t clipUnion);

  template <eClipFlags ClipFlags>
  uint32_t clipTriangle(uint32_t nNumVertices, uint32_t *pSrc, uint32_t *pDst,
                         uint32_t *pTmp);

  uint32_t clipTriangle(uint32_t plane, uint32_t nNumVertices,
                            uint32_t *pSrc, uint32_t *pDst, uint32_t *pTmp);

  void interpolateVertex(uint32_t i0, uint32_t i1, floatf fDistA, floatf fDistB,
                         uint32_t i2);

  void rasterTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void rasterLine(uint32_t i0, uint32_t i1);

  void rasterPoint(uint32_t index);

  bool setupLineAttributes(LineGradient *pGradient, uint32_t i0, uint32_t i1);

  bool setupTriangleAttributes(const TriangleGradient& gradient, uint32_t i0, uint32_t i1, uint32_t i2);

  Register *applyDepthGradient(Register *pRegister, const TriangleGradient &gradient,
                          const RDVECTOR &v0, const RDVECTOR &v1,
                          const RDVECTOR &v2);

  void applyPolygonOffset(Register *pRegister);

  Register *applyColorGradient(Register *pRegister, const TriangleGradient &gradient,
                          uint32_t i0, uint32_t i1, uint32_t i2);

  Register *applyAffineTextureGradient(Register *pRegister,
                                  const TriangleGradient &gradient, uint32_t i0,
                                  uint32_t i1, uint32_t i2);

  Register *applyPerspectiveTextureGradient(Register *pRegister,
                                       const TriangleGradient &gradient,
                                       uint32_t i0, uint32_t i1, uint32_t i2);

  Register *applyAffineTextureMipmapGradient(Register *pRegister);

  Register *applyPerspectiveTextureMipmapGradient(
      Register *pRegister, const TriangleGradient &gradient, const RDVECTOR &v0,
      const RDVECTOR &v1, const RDVECTOR &v2, floatRW rhws[3]);

  Register *applyFogGradient(Register *pRegister, const TriangleGradient &gradient,
                        uint32_t i0, uint32_t i1, uint32_t i2);

  void ensureClearColor() {
    Color4 tmp(Math::TToUNORM8(Math::TSat(vClearColor_.w)),
               Math::TToUNORM8(Math::TSat(vClearColor_.x)),
               Math::TToUNORM8(Math::TSat(vClearColor_.y)),
               Math::TToUNORM8(Math::TSat(vClearColor_.z)));
    pSurfDraw_->convertColor(&clearColor_, tmp);
    dirtyFlags_.ClearColor = 0;
  }

  void ensureClearDepth() {
    clearDepth_ = Math::TToUNORM16(Math::TSat(fClearDepth_));
    dirtyFlags_.ClearDepth = 0;
  }

  void ensureColorWriteMask() {
    Color4 tmp(cColorWriteMask_);
    pSurfDraw_->convertColor(&rasterData_.ColorWriteMask, tmp);
    dirtyFlags_.ColorWriteMask = 0;
  }

  void ensureFogColor() {
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
