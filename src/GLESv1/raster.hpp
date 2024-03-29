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

class Rasterizer : public TNL {
public:
  GLenum renderPrimitive(GLenum mode, uint32_t count);

  template <typename T>
  GLenum renderIndexedPrimitive(GLenum mode, const T *pIndices, uint32_t count, uint32_t startVertex);  

protected:

  struct TriangleGradient {
    fixed4 i4x0; 
    fixed4 i4y0;
    fixed4 i4x1;    
    fixed4 i4y1;
    fixed4 i4x2;    
    fixed4 i4y2;
    fixed4 i4dx12;
    fixed4 i4dy12;
    fixed4 i4dx13;
    fixed4 i4dy13;
    fixed4 i4dx23;
    fixed4 i4dy23;    
    floatQ fRatio;

#ifdef COCOGL_RASTER_PROFILE
    fixed8 i8Area;
#endif

    template <typename T, typename D>
    T calcDeltaX(D delta0, D delta1) const {
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

    template <typename T, typename D>
    T calcDeltaY(D delta0, D delta1) const {
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

    template <typename T>
    T calcDeltaX(int delta0, int delta1) const {
      auto diff = this->i4dy13 * delta0 - this->i4dy12 * delta1;
#ifdef COCOGL_PIXEDPOINT
      int FRAC = fixed8::FRAC + fixed4::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((diff.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(fixed12::make(diff.data()) * this->fRatio);
#endif
    }

    template <typename T>
    T calcDeltaY(int delta0, int delta1) const {
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
    fixed4 i4x0; 
    fixed4 i4x1;
    fixed4 i4y0;
    fixed4 i4y1;
    fixed4 i4dx;
    fixed4 i4dy;
    floatQ fRatio;

    template <typename T, typename D>
    T calcDelta(D delta) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = D::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((delta.data() * this->fRatio.data() + half) >> FRAC);
#else
      return static_cast<T>(delta * this->fRatio);
#endif
    }

    template <typename T>
    T calcDelta(int delta) const {
#ifdef COCOGL_PIXEDPOINT
      int FRAC = fixed8::FRAC + floatQ::FRAC - T::FRAC;
      int half = 1 << (FRAC - 1);
      return T::make((delta * this->fRatio.data() + half) >> FRAC);
#else
      return T::make(static_cast<TFixed<T::FRAC - fixed8::FRAC>>(delta * this->fRatio).data());
#endif
    }
  };

  Rasterizer() {}

  ~Rasterizer() {}

  GLenum beginRender(GLenum mode, int first, uint32_t count);

  void endRender();

  GLenum setupRasterStates(GLenum mode);

  bool generateRasterOp();

  void updateScissorRect();

  void drawTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void drawLine(uint32_t i0, uint32_t i1);

  void drawPoint(uint32_t index);

  bool cullScreenSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  bool cullClipSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void rasterClippedTriangle(uint32_t i0, uint32_t i1, uint32_t i2, uint32_t clipUnion);

  void rasterClippedLine(uint32_t i0, uint32_t i1, uint32_t clipUnion);

  template <eClipFlags ClipFlags>
  uint32_t clipTriangle(uint32_t nNumVertices, uint32_t *pSrc, uint32_t *pDst, uint32_t *pTmp);

  uint32_t clipTriangle(uint32_t plane, uint32_t nNumVertices, uint32_t *pSrc, uint32_t *pDst, uint32_t *pTmp);

  void interpolateVertex(uint32_t dst, uint32_t i0, uint32_t i1, floatf fDistA, floatf fDistB);

  void rasterTriangle(uint32_t i0, uint32_t i1, uint32_t i2);

  void rasterLine(uint32_t i0, uint32_t i1);

  void rasterPoint(uint32_t index);

  bool setupLineAttributes(LineGradient *g, uint32_t i0, uint32_t i1);

  bool setupTriangleAttributes(TriangleGradient *g, uint32_t i0, uint32_t i1, uint32_t i2);

  Register *applyDepthGradient(Register *pRegister,
                               const TriangleGradient &g,
                               const RDVECTOR &v0, 
                               const RDVECTOR &v1,
                               const RDVECTOR &v2);

  void applyPolygonOffset(Register *pRegister);

  Register *applyColorGradient(Register *pRegister,
                               const TriangleGradient &g, 
                               uint32_t i0,
                               uint32_t i1, 
                               uint32_t i2);

  Register *applyAffineTextureGradient(Register *pRegister,
                                       const TriangleGradient &g,
                                       uint32_t i0, 
                                       uint32_t i1, 
                                       uint32_t i2);

  Register *applyPerspectiveTextureGradient(Register *pRegister,
                                            const TriangleGradient &g,
                                            uint32_t i0, uint32_t i1,
                                            uint32_t i2);

  Register *applyAffineTextureMipmapGradient(Register *pRegister);

  Register *applyPerspectiveTextureMipmapGradient(Register *pRegister, 
                                                  const TriangleGradient &g, 
                                                  const RDVECTOR &v0, 
                                                  const RDVECTOR &v1, 
                                                  const RDVECTOR &v2, 
                                                  floatRW rhws[3]);

  Register *applyFogGradient(Register *pRegister,
                             const TriangleGradient &g, 
                             uint32_t i0,
                             uint32_t i1, 
                             uint32_t i2);

  void ensureClearColor() {
    ColorARGB tmp(ToUNORM8(Sat(vClearColor_.w)),
                  ToUNORM8(Sat(vClearColor_.x)),
                  ToUNORM8(Sat(vClearColor_.y)),
                  ToUNORM8(Sat(vClearColor_.z)));
    pSurfDraw_->convertColor(&clearColor_, tmp);
    dirtyFlags_.ClearColor = 0;
  }

  void ensureClearDepth() {
    clearDepth_ = ToUNORM16(Sat(fClearDepth_));
    dirtyFlags_.ClearDepth = 0;
  }

  void ensureColorWriteMask() {
    ColorARGB tmp(cColorWriteMask_);
    pSurfDraw_->convertColor(&rasterData_.ColorWriteMask, tmp);
    dirtyFlags_.ColorWriteMask = 0;
  }

  void ensureFogColor() {
    rasterData_.cFogColor.a = static_cast<uint8_t>(ToUNORM8(Sat(vFogColor_.w)));
    rasterData_.cFogColor.r = static_cast<uint8_t>(ToUNORM8(Sat(vFogColor_.x)));
    rasterData_.cFogColor.g = static_cast<uint8_t>(ToUNORM8(Sat(vFogColor_.y)));
    rasterData_.cFogColor.b = static_cast<uint8_t>(ToUNORM8(Sat(vFogColor_.z)));
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
