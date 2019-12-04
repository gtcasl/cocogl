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

void Rasterizer::drawPoint(uint32_t index) {
  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEX_FLAGS]);
  uint32_t flags = pwFlags[index];

  // Check if the vertex is clipped
  if (flags & CLIPPING_MASK) {
    return;
  }

  // Raster the point
  this->rasterPoint(index);
}

void Rasterizer::rasterPoint(uint32_t index) {
  auto pvScreenPos =
      reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEX_SCREENPOS]);
  auto pfPointSize =
      reinterpret_cast<fixed4 *>(pbVertexData_[VERTEX_POINTSIZE]);

  auto &vertex = pvScreenPos[index];
  auto fPointSize = pfPointSize[index];
  auto fHalfSize = fPointSize >> 1;

  auto ymin = std::max<int>(Math::Roundi<int>(vertex.y - fHalfSize),
                              scissorRect_.top);
  auto ymax = std::min<int>(Math::Roundi<int>(vertex.y + fHalfSize),
                              scissorRect_.bottom);
  auto xmin = std::max<int>(Math::Roundi<int>(vertex.x - fHalfSize),
                              scissorRect_.left);
  auto xmax = std::min<int>(Math::Roundi<int>(vertex.x + fHalfSize),
                              scissorRect_.right);

  // Early out if the point has no size
  if ((xmin >= xmax) || (ymin >= ymax)) {
    return;
  }

  if (!this->generateRasterOp()) {
    __glLogError("Rasterizer::generateRasterOp() failed.\r\n");
    return;
  }

  // Obtain the scanline routine
  auto pfnScanline = rasterData_.pRasterOp->getScanline();
  auto *pRegisters = rasterData_.Registers;
  auto rasterFlags = rasterID_.Flags;

  if (rasterFlags.DepthTest) {
    pRegisters->m[0] = Math::Zero<fixedRX>();
    pRegisters->m[1] = Math::Zero<fixedRX>();
    pRegisters->m[2] = static_cast<fixedRX>(vertex.z);
    ++pRegisters;
  }

  if (rasterFlags.Color) {
    auto pcColors = reinterpret_cast<ColorARGB *>(pbVertexColor_);
    auto cColor = pcColors[index];

    pRegisters[0].m[0] = Math::Zero<fixedRX>();
    pRegisters[0].m[1] = Math::Zero<fixedRX>();
    pRegisters[0].m[2] = static_cast<fixedRX>(cColor.b) >> fixed8::FRAC;

    pRegisters[1].m[0] = Math::Zero<fixedRX>();
    pRegisters[1].m[1] = Math::Zero<fixedRX>();
    pRegisters[1].m[2] = static_cast<fixedRX>(cColor.g) >> fixed8::FRAC;

    pRegisters[2].m[0] = Math::Zero<fixedRX>();
    pRegisters[2].m[1] = Math::Zero<fixedRX>();
    pRegisters[2].m[2] = static_cast<fixedRX>(cColor.r) >> fixed8::FRAC;

    pRegisters[3].m[0] = Math::Zero<fixedRX>();
    pRegisters[3].m[1] = Math::Zero<fixedRX>();
    pRegisters[3].m[2] = static_cast<fixedRX>(cColor.a) >> fixed8::FRAC;

    pRegisters += 4;
  }

  if (rasterFlags.NumTextures) {
    fixedRX fDelta;

    if (caps_.PointSprite) {
      fDelta = Math::Inverse<fixedRX>(fPointSize);
    }

    for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      assert(i < MAX_TEXTURES);

      if (caps_.PointSprite && texUnits_[i].bCoordReplace) {
        pRegisters[0].m[0] = fDelta;
        pRegisters[0].m[1] = Math::Zero<fixedRX>();
        pRegisters[0].m[2] = (fDelta >> 1) - fDelta * xmin;

        pRegisters[1].m[0] = Math::Zero<fixedRX>();
        pRegisters[1].m[1] = fDelta;
        pRegisters[1].m[2] = (fDelta >> 1) - fDelta * ymin;
      } else {
        auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(
            pbVertexData_[VERTEX_TEXCOORD0 + i]);
        const TEXCOORD2 &vTexCoord = vTexCoords[index];

        pRegisters[0].m[0] = Math::Zero<fixedRX>();
        pRegisters[0].m[1] = Math::Zero<fixedRX>();
        pRegisters[0].m[2] = static_cast<fixedRX>(vTexCoord.m[0]);

        pRegisters[1].m[0] = Math::Zero<fixedRX>();
        pRegisters[1].m[1] = Math::Zero<fixedRX>();
        pRegisters[1].m[2] = static_cast<fixedRX>(vTexCoord.m[1]);
      }

      pRegisters += 2;
    }
  }

  if (rasterFlags.Fog) {
    auto pfFogs = reinterpret_cast<float20 *>(pbVertexData_[VERTEX_FOG]);
    auto fFog = pfFogs[index];

    pRegisters->m[0] = Math::Zero<fixedRX>();
    pRegisters->m[1] = Math::Zero<fixedRX>();
    pRegisters->m[2] = static_cast<fixedRX>(fFog);
    ++pRegisters;
  }

  // Set the reference offset
  rasterData_.fRefX = Math::Zero<fixed4>();
  rasterData_.fRefY = Math::Zero<fixed4>();

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  rasterData_.pRasterOp->StartProfile((ymax - ymin) * (xmax - xmin));
#endif

  for (int y = ymin; y < ymax; ++y) {
    // Raster scanline
    (pfnScanline)(rasterData_, y, xmin, xmax);
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(
      end_time - start_time);
  rasterData_.pRasterOp->EndProfile(elapsed_time.count());
#endif
}
