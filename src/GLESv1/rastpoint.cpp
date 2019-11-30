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

void CRasterizer::DrawPoint(uint32_t index) {
  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEXDATA_FLAGS]);
  uint32_t flags = pwFlags[index];

  // Check if the vertex is clipped
  if (flags & CLIPPING_MASK) {
    return;
  }

  // Raster the point
  this->RasterPoint(index);
}

void CRasterizer::RasterPoint(uint32_t index) {
  auto pvScreenPos =
      reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);
  auto pfPointSize =
      reinterpret_cast<fixed4 *>(pbVertexData_[VERTEXDATA_POINTSIZE]);

  const RDVECTOR &vertex = pvScreenPos[index];
  auto fPointSize = pfPointSize[index];
  auto fHalfSize = fPointSize >> 1;

  int ymin = Math::TMax<int>(Math::TRoundi<int>(vertex.y - fHalfSize),
                             scissorRect_.top);

  int ymax = Math::TMin<int>(Math::TRoundi<int>(vertex.y + fHalfSize),
                             scissorRect_.bottom);

  int xmin = Math::TMax<int>(Math::TRoundi<int>(vertex.x - fHalfSize),
                             scissorRect_.left);

  int xmax = Math::TMin<int>(Math::TRoundi<int>(vertex.x + fHalfSize),
                             scissorRect_.right);

  // Early out if the point has no size
  if ((xmin >= xmax) || (ymin >= ymax)) {
    return;
  }

  if (!this->GenerateRasterOp()) {
    __glLogError(_T("CRasterizer::GenerateRasterOp() failed.\r\n"));
    return;
  }

  // Obtain the scanline routine
  PFN_Scanline pfnScanline = rasterData_.pRasterOp->GetScanline();

  Register *pRegisters = rasterData_.Registers;
  RASTERFLAGS rasterFlags = rasterID_.Flags;

  if (rasterFlags.DepthTest) {
    pRegisters->m[0] = TConst<fixedRX>::Zero();
    pRegisters->m[1] = TConst<fixedRX>::Zero();
    pRegisters->m[2] = static_cast<fixedRX>(vertex.z);
    ++pRegisters;
  }

  if (rasterFlags.Color) {
    auto pcColors = reinterpret_cast<ColorARGB *>(pbVertexColor_);
    ColorARGB cColor = pcColors[index];

    pRegisters[0].m[0] = TConst<fixedRX>::Zero();
    pRegisters[0].m[1] = TConst<fixedRX>::Zero();
    pRegisters[0].m[2] = static_cast<fixedRX>(cColor.b) >> fixed8::FRAC;

    pRegisters[1].m[0] = TConst<fixedRX>::Zero();
    pRegisters[1].m[1] = TConst<fixedRX>::Zero();
    pRegisters[1].m[2] = static_cast<fixedRX>(cColor.g) >> fixed8::FRAC;

    pRegisters[2].m[0] = TConst<fixedRX>::Zero();
    pRegisters[2].m[1] = TConst<fixedRX>::Zero();
    pRegisters[2].m[2] = static_cast<fixedRX>(cColor.r) >> fixed8::FRAC;

    pRegisters[3].m[0] = TConst<fixedRX>::Zero();
    pRegisters[3].m[1] = TConst<fixedRX>::Zero();
    pRegisters[3].m[2] = static_cast<fixedRX>(cColor.a) >> fixed8::FRAC;

    pRegisters += 4;
  }

  if (rasterFlags.NumTextures) {
    fixedRX fDelta;

    if (caps_.PointSprite) {
      fDelta = Math::TInv<fixedRX>(fPointSize);
    }

    for (uint32_t i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      assert(i < MAX_TEXTURES);

      if (caps_.PointSprite && texUnits_[i].bCoordReplace) {
        pRegisters[0].m[0] = fDelta;
        pRegisters[0].m[1] = TConst<fixedRX>::Zero();
        pRegisters[0].m[2] = (fDelta >> 1) - fDelta * xmin;

        pRegisters[1].m[0] = TConst<fixedRX>::Zero();
        pRegisters[1].m[1] = fDelta;
        pRegisters[1].m[2] = (fDelta >> 1) - fDelta * ymin;
      } else {
        auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(
            pbVertexData_[VERTEXDATA_TEXCOORD0 + i]);
        const TEXCOORD2 &vTexCoord = vTexCoords[index];

        pRegisters[0].m[0] = TConst<fixedRX>::Zero();
        pRegisters[0].m[1] = TConst<fixedRX>::Zero();
        pRegisters[0].m[2] = static_cast<fixedRX>(vTexCoord.m[0]);

        pRegisters[1].m[0] = TConst<fixedRX>::Zero();
        pRegisters[1].m[1] = TConst<fixedRX>::Zero();
        pRegisters[1].m[2] = static_cast<fixedRX>(vTexCoord.m[1]);
      }

      pRegisters += 2;
    }
  }

  if (rasterFlags.Fog) {
    auto pfFogs = reinterpret_cast<float20 *>(pbVertexData_[VERTEXDATA_FOG]);
    auto fFog = pfFogs[index];

    pRegisters->m[0] = TConst<fixedRX>::Zero();
    pRegisters->m[1] = TConst<fixedRX>::Zero();
    pRegisters->m[2] = static_cast<fixedRX>(fFog);
    ++pRegisters;
  }

  // Set the reference offset
  rasterData_.fRefX = TConst<fixed4>::Zero();
  rasterData_.fRefY = TConst<fixed4>::Zero();

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
