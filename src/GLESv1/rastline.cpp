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
  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEX_FLAGS]);
  auto flags0 = pwFlags[i0];
  auto flags1 = pwFlags[i1];

  // Check if we need to clip vertices
  auto clipUnion = (flags0 | flags1) & CLIPPING_MASK;
  if (0 == clipUnion) {
    // Raster the line
    this->rasterLine(i0, i1);
  } else {
    // Discard primitives falling outside of the same plane.
    auto clipIntersect = (flags0 & flags1) & CLIPPING_MASK;
    if (0 == clipIntersect) {
      // Clip and raster the triangle
      this->rasterClippedLine(i0, i1, clipUnion);
    }
  }
}

void Rasterizer::rasterLine(uint32_t i0, uint32_t i1) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  LineGradient g;

  // Setup line attributes
  if (!this->setupLineAttributes(&g, i0, i1))
    return;

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  rasterData_.pRasterOp->startProfile(static_cast<int>(g.i4dx + g.i4dy));
#endif

  auto pfnScanline = rasterData_.pRasterOp->getScanline();  
  auto fLineWidth = static_cast<fixedDDA>(fLineWidth_);
  auto fRndCeil = fixedDDA::make(fixedDDA::MASK) - Half<fixedDDA>();

  if (g.i4dx > g.i4dy) {
    auto fddy = Mul<fixedDDA>(g.i4y1 - g.i4y0, g.fRatio);
    auto x0 = std::max<int>(Ceil<int>(g.i4x0 - Half<fixed4>()), scissorRect_.left);
    auto x1 = std::min<int>(Ceil<int>(g.i4x1 - Half<fixed4>()), scissorRect_.right);

    auto i4X0Diff = fixed4(x0) - (g.i4x0 - Half<fixed4>());
    auto fty = fixedDDA(g.i4y0) + fddy * i4X0Diff - (fLineWidth / 2) + fRndCeil;
    auto fby = fty + fLineWidth;

    for (int x = x0; x < x1; ++x) {
      auto y0 = std::max<int>(static_cast<int>(fty), scissorRect_.top);
      auto y1 = std::min<int>(static_cast<int>(fby), scissorRect_.bottom);

      for (int y = y0; y < y1; ++y) {
        (pfnScanline)(rasterData_, y, x, x + 1);
      }

      fty += fddy;
      fby += fddy;
    }
  } else {
    auto fddx = Mul<fixedDDA>(g.i4x1 - g.i4x0, g.fRatio);
    auto y0 = std::max<int>(Ceil<int>(g.i4y0 - Half<fixed4>()), scissorRect_.top);
    auto y1 = std::min<int>(Ceil<int>(g.i4y1 - Half<fixed4>()), scissorRect_.bottom);

    auto i4Y0Diff = fixed4(y0) - (g.i4y0 - Half<fixed4>());
    auto flx = fixedDDA(g.i4x0) + fddx * i4Y0Diff - (fLineWidth / 2) + fRndCeil;
    auto frx = flx + fLineWidth;

    for (int y = y0; y < y1; ++y) {
      auto x0 = std::max<int>(static_cast<int>(flx), scissorRect_.left);
      auto x1 = std::min<int>(static_cast<int>(frx), scissorRect_.right);

      (pfnScanline)(rasterData_, y, x0, x1);

      flx += fddx;
      frx += fddx;
    }
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
  rasterData_.pRasterOp->endProfile(elapsed_time.count());
#endif
}

bool Rasterizer::setupLineAttributes(LineGradient *g, uint32_t i0, uint32_t i1) {  
  auto rasterFlags = rasterID_.Flags;
  auto pRegister = rasterData_.Registers;  
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEX_SCREENPOS]);

  auto i4x0 = pvScreenPos[i0].x;
  auto i4y0 = pvScreenPos[i0].y;  
  auto i4x1 = pvScreenPos[i1].x;
  auto i4y1 = pvScreenPos[i1].y;

  auto i4dx = std::abs(i4x1 - i4x0);
  auto i4dy = std::abs(i4y1 - i4y0);

  int attribIdx;

  if (i4dx > i4dy) {
    // Early out for empty lines
    if (i4dx == Zero<fixed4>())
      return false;
    
    // X-axis sort
    if (i4x0 > i4x1) {
      std::swap(i4x0, i4x1);
      std::swap(i0, i1);
    }
    g->fRatio = Inverse<floatQ>(i4dx);
    attribIdx = 0;
  } else {
    // Early out for empty lines
    if (i4dy == Zero<fixed4>())
      return false;
    
    // Y-axis sort
    if (i4y0 > i4y1) {     
      std::swap(i4y0, i4y1);
      std::swap(i0, i1);
    }
    g->fRatio = Inverse<floatQ>(i4dy);
    attribIdx = 1;
  }  

  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];

  // Set the reference offset
  rasterData_.fRefX = i4x0 - Half<fixed4>();
  rasterData_.fRefY = i4y0 - Half<fixed4>();  

  if (rasterFlags.DepthTest) {
    auto delta = v1.z - v0.z;
    if (fixed16(std::abs(delta)) > Epsilon<fixed16>()) {
      pRegister->m[attribIdx] = g->calcDelta<fixedRX>(delta);
      pRegister->m[attribIdx ^ 0x1] = Zero<fixedRX>();
      rasterID_.Flags.InterpolateDepth = 1;
    } else {
      pRegister->m[0] = Zero<fixedRX>();
      pRegister->m[1] = Zero<fixedRX>();
    }
    pRegister->m[2] = static_cast<fixedRX>(v0.z);
    ++pRegister;
  }

  if (rasterFlags.Color) {
    auto pcColors = reinterpret_cast<ColorARGB *>(pbVertexColor_);
    auto c0 = pcColors[i0];
    auto c1 = pcColors[i1];

    auto interpolateMask = (rasterFlags.InterpolateColor ? 0x7 : 0) |
                           (rasterFlags.InterpolateAlpha << 3);
    if (interpolateMask) {
      for (uint32_t i = 0; i < 4; ++i) {
        int delta = c1.m[i] - c0.m[i];
        if (delta != 0) {
          pRegister[i].m[attribIdx] = g->calcDelta<fixedRX>(delta);
          pRegister[i].m[attribIdx ^ 0x1] = Zero<fixedRX>();
        } else {
          pRegister[i].m[0] = Zero<fixedRX>();
          pRegister[i].m[1] = Zero<fixedRX>();
          interpolateMask &= ~(1 << i);
        }
        pRegister[i].m[2] = static_cast<fixedRX>(c0.m[i]) >> fixed8::FRAC;
      }

      rasterID_.Flags.InterpolateColor = (interpolateMask & 0x7) ? 1 : 0;
      rasterID_.Flags.InterpolateAlpha = interpolateMask >> 3;
    } else {
      for (uint32_t i = 0; i < 4; ++i) {
        pRegister[i].m[0] = Zero<fixedRX>();
        pRegister[i].m[1] = Zero<fixedRX>();
        pRegister[i].m[2] = static_cast<fixedRX>(c1.m[i]) >> fixed8::FRAC;
      }
    }

    pRegister += 4;
  }

  if (rasterFlags.NumTextures) {
    for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      assert(i < MAX_TEXTURES);

      auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(pbVertexData_[VERTEX_TEXCOORD0 + i]);
      auto &uv0 = vTexCoords[i0];
      auto &uv1 = vTexCoords[i1];

      pRegister[0].m[attribIdx] = g->calcDelta<fixedRX>(uv1.m[0] - uv0.m[0]);
      pRegister[0].m[attribIdx ^ 0x1] = Zero<fixedRX>();
      pRegister[0].m[2] = static_cast<fixedRX>(uv0.m[0]);

      pRegister[1].m[attribIdx] = g->calcDelta<fixedRX>(uv1.m[1] - uv0.m[1]);
      pRegister[1].m[attribIdx ^ 0x1] = Zero<fixedRX>();
      pRegister[1].m[2] = static_cast<fixedRX>(uv0.m[1]);

      pRegister += 2;
    }
  }

  if (rasterFlags.Fog) {
    auto pfFogs = reinterpret_cast<floatRX *>(pbVertexData_[VERTEX_FOG]);
    auto fFog0 = pfFogs[i0];
    auto fFog1 = pfFogs[i1];

    auto delta = fFog1 - fFog0;
    if (fixedRX(std::abs(delta)) > Epsilon<fixedRX>()) {
      pRegister->m[attribIdx] = g->calcDelta<fixedRX>(delta);
      pRegister->m[attribIdx ^ 0x1] = Zero<fixedRX>();
      rasterID_.Flags.InterpolateFog = 1;
    } else {
      pRegister->m[0] = Zero<fixedRX>();
      pRegister->m[1] = Zero<fixedRX>();
    }
    pRegister->m[2] = static_cast<fixedRX>(fFog0);
    ++pRegister;
  }

  // Generate the rasterization routine
  if (!this->generateRasterOp()) {
    __glLogError("Rasterizer::generateRasterOp() failed.\r\n");
    return false;
  }

  g->i4x0 = i4x0;
  g->i4y0 = i4y0;
  g->i4x1 = i4x1;
  g->i4y1 = i4y1;
  
  g->i4dx = i4dx;
  g->i4dy = i4dy;

  return true;
}