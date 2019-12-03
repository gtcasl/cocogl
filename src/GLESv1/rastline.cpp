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
#include "stdafx.h"
#include "raster.hpp"
#include "raster.inl"

void Rasterizer::drawLine(uint32_t i0, uint32_t i1) {
  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEXDATA_FLAGS]);
  uint32_t flags0 = pwFlags[i0];
  uint32_t flags1 = pwFlags[i1];

  // Check if we need to clip vertices
  uint32_t clipUnion = (flags0 | flags1) & CLIPPING_MASK;
  if (0 == clipUnion) {
    // Raster the line
    this->rasterLine(i0, i1);
  } else {
    // Discard primitives falling outside of the same plane.
    uint32_t clipIntersect = (flags0 & flags1) & CLIPPING_MASK;
    if (0 == clipIntersect) {
      // Clip and raster the triangle
      this->rasterClippedLine(i0, i1, clipUnion);
    }
  }
}

void Rasterizer::rasterLine(uint32_t i0, uint32_t i1) {
  LineGradient gradient;

  // Setup the line attributes
  if (!this->setupLineAttributes(&gradient, i0, i1))
    return;

  auto pvScreenPos =
      reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];

  auto fLineWidth = static_cast<fixedDDA>(fLineWidth_);
  auto fRndCeil = fixedDDA::make(fixedDDA::MASK) - TConst<fixedDDA>::Half();

  auto i4dx = v1.x - v0.x;
  auto i4dy = v1.y - v0.y;

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  rasterData_.pRasterOp->StartProfile(
      static_cast<int>(Math::TAbs(i4dx) + Math::TAbs(i4dy)));
#endif

  auto pfnScanline = rasterData_.pRasterOp->getScanline();

  if (Math::TAbs(i4dx) > Math::TAbs(i4dy)) {
    // Set the reference offset
    rasterData_.fRefX = v0.x - TConst<fixed4>::Half();
    rasterData_.fRefY = v0.y - TConst<fixed4>::Half();

    // X-axis sort
    const RDVECTOR *vertices[2] = {&v0, &v1};
    if (i4dx < TConst<fixed4>::Zero()) {
      gradient.fRatio = -gradient.fRatio;
      vertices[0] = &v1;
      vertices[1] = &v0;
    }

    auto i4x0 = vertices[0]->x;
    auto i4y0 = vertices[0]->y;
    auto i4x1 = vertices[1]->x;
    auto i4y1 = vertices[1]->y;

    auto fddy = Math::TMul<fixedDDA>(i4y1 - i4y0, gradient.fRatio);
    auto x0 = Math::TMax<int>(Math::TCeili<int>(i4x0 - TConst<fixed4>::Half()),
                              scissorRect_.left);
    auto x1 = Math::TMin<int>(Math::TCeili<int>(i4x1 - TConst<fixed4>::Half()),
                              scissorRect_.right);

    auto i4X0Diff = fixed4(x0) - (i4x0 - TConst<fixed4>::Half());
    auto fty = fixedDDA(i4y0) + fddy * i4X0Diff - (fLineWidth / 2) + fRndCeil;
    auto fby = fty + fLineWidth;

    for (int x = x0; x < x1; ++x) {
      auto y0 = Math::TMax<int>(static_cast<int>(fty), scissorRect_.top);
      auto y1 = Math::TMin<int>(static_cast<int>(fby), scissorRect_.bottom);

      for (int y = y0; y < y1; ++y) {
        (pfnScanline)(rasterData_, y, x, x + 1);
      }

      fty += fddy;
      fby += fddy;
    }
  } else {
    // Set the reference offset
    rasterData_.fRefX = v0.x - TConst<fixed4>::Half();
    rasterData_.fRefY = v0.y - TConst<fixed4>::Half();

    // Y-axis sort
    const RDVECTOR *vertices[2] = {&v0, &v1};
    if (i4dy < TConst<fixed4>::Zero()) {
      gradient.fRatio = -gradient.fRatio;
      vertices[0] = &v1;
      vertices[1] = &v0;
    }

    auto i4x0 = vertices[0]->x;
    auto i4y0 = vertices[0]->y;
    auto i4x1 = vertices[1]->x;
    auto i4y1 = vertices[1]->y;

    auto fddx = Math::TMul<fixedDDA>(i4x1 - i4x0, gradient.fRatio);
    auto y0 = Math::TMax<int>(Math::TCeili<int>(i4y0 - TConst<fixed4>::Half()),
                              scissorRect_.top);
    auto y1 = Math::TMin<int>(Math::TCeili<int>(i4y1 - TConst<fixed4>::Half()),
                              scissorRect_.bottom);

    auto i4Y0Diff = fixed4(y0) - (i4y0 - TConst<fixed4>::Half());
    auto flx = fixedDDA(i4x0) + fddx * i4Y0Diff - (fLineWidth / 2) + fRndCeil;
    auto frx = flx + fLineWidth;

    for (int y = y0; y < y1; ++y) {
      auto x0 = Math::TMax<int>(static_cast<int>(flx), scissorRect_.left);
      auto x1 = Math::TMin<int>(static_cast<int>(frx), scissorRect_.right);

      (pfnScanline)(rasterData_, y, x0, x1);

      flx += fddx;
      frx += fddx;
    }
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(
      end_time - start_time);
  rasterData_.pRasterOp->EndProfile(elapsed_time.count());
#endif
}

bool Rasterizer::setupLineAttributes(LineGradient *pGradient, uint32_t i0,
                                      uint32_t i1) {
  auto rasterFlags = rasterID_.Flags;
  auto pRegister = rasterData_.Registers;
  auto pvScreenPos =
      reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);

  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];

  auto i4dx = v1.x - v0.x;
  auto i4dy = v1.y - v0.y;

  uint32_t attribIdx;

  if (Math::TAbs(i4dx) > Math::TAbs(i4dy)) {
    // Early out for empty lines
    if (i4dx == TConst<fixed4>::Zero()) {
      return false;
    }

    pGradient->fRatio = TConst<floatQ>::One() / i4dx;
    attribIdx = 0;
  } else {
    // Early out for empty lines
    if (i4dy == TConst<fixed4>::Zero()) {
      return false;
    }

    pGradient->fRatio = TConst<floatQ>::One() / i4dy;
    attribIdx = 1;
  }

  if (rasterFlags.DepthTest) {
    auto delta = v1.z - v0.z;
    if (fixed16(Math::TAbs(delta)) > TConst<fixed16>::Epsilon()) {
      pRegister->m[attribIdx] = pGradient->calcDelta<fixedRX>(delta);
      pRegister->m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      rasterID_.Flags.InterpolateDepth = 1;
    } else {
      pRegister->m[0] = TConst<fixedRX>::Zero();
      pRegister->m[1] = TConst<fixedRX>::Zero();
    }
    pRegister->m[2] = static_cast<fixedRX>(v0.z);
    ++pRegister;
  }

  if (rasterFlags.Color) {
    auto pcColors = reinterpret_cast<ColorARGB *>(pbVertexColor_);
    auto c0 = pcColors[i0];
    auto c1 = pcColors[i1];

    uint32_t interpolateMask = (rasterFlags.InterpolateColor ? 0x7 : 0) |
                               (rasterFlags.InterpolateAlpha << 3);
    if (interpolateMask) {
      for (uint32_t i = 0; i < 4; ++i) {
        int delta = c1.m[i] - c0.m[i];
        if (Math::TAbs(delta) > 1) {
          pRegister[i].m[attribIdx] = pGradient->calcDelta<fixedRX>(delta);
          pRegister[i].m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
        } else {
          pRegister->m[0] = TConst<fixedRX>::Zero();
          pRegister->m[1] = TConst<fixedRX>::Zero();
          interpolateMask &= ~(1 << i);
        }
        pRegister[i].m[2] = static_cast<fixedRX>(c0.m[i]) >> fixed8::FRAC;
      }

      rasterID_.Flags.InterpolateColor = (interpolateMask & 0x7) ? 1 : 0;
      rasterID_.Flags.InterpolateAlpha = interpolateMask >> 3;
    } else {
      for (uint32_t i = 0; i < 4; ++i) {
        pRegister[i].m[0] = TConst<fixedRX>::Zero();
        pRegister[i].m[1] = TConst<fixedRX>::Zero();
        pRegister[i].m[2] = static_cast<fixedRX>(c1.m[i]) >> fixed8::FRAC;
      }
    }

    pRegister += 4;
  }

  if (rasterFlags.NumTextures) {
    for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      assert(i < MAX_TEXTURES);

      auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(
          pbVertexData_[VERTEXDATA_TEXCOORD0 + i]);
      auto &uv0 = vTexCoords[i0];
      auto &uv1 = vTexCoords[i1];

      pRegister[0].m[attribIdx] =
          pGradient->calcDelta<fixedRX>(uv1.m[0] - uv0.m[0]);
      pRegister[0].m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      pRegister[0].m[2] = static_cast<fixedRX>(uv0.m[0]);

      pRegister[1].m[attribIdx] =
          pGradient->calcDelta<fixedRX>(uv1.m[1] - uv0.m[1]);
      pRegister[1].m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      pRegister[1].m[2] = static_cast<fixedRX>(uv0.m[1]);

      pRegister += 2;
    }
  }

  if (rasterFlags.Fog) {
    auto pfFogs = reinterpret_cast<float20 *>(pbVertexData_[VERTEXDATA_FOG]);
    auto fFog0 = pfFogs[i0];
    auto fFog1 = pfFogs[i1];

    auto delta = fFog1 - fFog0;
    if (fixedRX(Math::TAbs(delta)) > TConst<fixedRX>::Epsilon()) {
      pRegister->m[attribIdx] = pGradient->calcDelta<fixedRX>(delta);
      pRegister->m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      rasterID_.Flags.InterpolateFog = 1;
    } else {
      pRegister->m[0] = TConst<fixedRX>::Zero();
      pRegister->m[1] = TConst<fixedRX>::Zero();
    }
    pRegister->m[2] = static_cast<fixedRX>(fFog0);
    ++pRegister;
  }

  // Generate the rasterization routine
  if (!this->generateRasterOp()) {
    __glLogError("Rasterizer::generateRasterOp() failed.\r\n");
    return false;
  }

  return true;
}