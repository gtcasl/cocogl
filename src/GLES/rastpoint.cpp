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

void CRasterizer::DrawPoint(unsigned index) {
  const uint16_t *const pwFlags = (const uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  const unsigned flags = pwFlags[index];

  // Check if the vertex is clipped
  if (flags & CLIPPING_MASK) {
    return;
  }

  // Raster the point
  this->RasterPoint(index);
}


void CRasterizer::RasterPoint(unsigned index) {
  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const fixed4 *const pfPointSize =
      (const fixed4 *)m_pbVertexData[VERTEXDATA_POINTSIZE];

  const RDVECTOR &vertex = pvScreenPos[index];
  const fixed4 fPointSize = pfPointSize[index];
  const fixed4 fHalfSize = fPointSize >> 1;

  const int ymin = Math::TMax<int>(Math::TRoundi<int>(vertex.y - fHalfSize),
                                   m_scissorRect.top);

  const int ymax = Math::TMin<int>(Math::TRoundi<int>(vertex.y + fHalfSize),
                                   m_scissorRect.bottom);

  const int xmin = Math::TMax<int>(Math::TRoundi<int>(vertex.x - fHalfSize),
                                   m_scissorRect.left);

  const int xmax = Math::TMin<int>(Math::TRoundi<int>(vertex.x + fHalfSize),
                                   m_scissorRect.right);

  // Early out if the point has no size
  if ((xmin >= xmax) || (ymin >= ymax)) {
    return;
  }

  if (!this->GenerateRasterOp()) {
    __glLogError(_T("CRasterizer::GenerateRasterOp() failed.\r\n"));
    return;
  }

  // Obtain the scanline routine
  const PFN_Scanline pfnScanline = m_rasterData.pRasterOp->GetScanline();

  Register *pRegisters = m_rasterData.Registers;
  const RASTERFLAGS rasterFlags = m_rasterID.Flags;

  if (rasterFlags.DepthTest) {
    pRegisters->fM[0] = TConst<fixedRX>::Zero();
    pRegisters->fM[1] = TConst<fixedRX>::Zero();
    pRegisters->fM[2] = static_cast<fixedRX>(vertex.z);
    ++pRegisters;
  }

  if (rasterFlags.Color) {
    const ColorARGB *const pcColors = (const ColorARGB *)m_pbVertexColor;
    const ColorARGB cColor = pcColors[index];

    pRegisters[0].fM[0] = TConst<fixedRX>::Zero();
    pRegisters[0].fM[1] = TConst<fixedRX>::Zero();
    pRegisters[0].fM[2] = static_cast<fixedRX>(cColor.b) >> fixed8::FRAC;

    pRegisters[1].fM[0] = TConst<fixedRX>::Zero();
    pRegisters[1].fM[1] = TConst<fixedRX>::Zero();
    pRegisters[1].fM[2] = static_cast<fixedRX>(cColor.g) >> fixed8::FRAC;

    pRegisters[2].fM[0] = TConst<fixedRX>::Zero();
    pRegisters[2].fM[1] = TConst<fixedRX>::Zero();
    pRegisters[2].fM[2] = static_cast<fixedRX>(cColor.r) >> fixed8::FRAC;

    pRegisters[3].fM[0] = TConst<fixedRX>::Zero();
    pRegisters[3].fM[1] = TConst<fixedRX>::Zero();
    pRegisters[3].fM[2] = static_cast<fixedRX>(cColor.a) >> fixed8::FRAC;

    pRegisters += 4;
  }

  if (rasterFlags.NumTextures) {
    fixedRX fDelta;

    if (m_caps.PointSprite) {
      fDelta = Math::TInv<fixedRX>(fPointSize);
    }

    for (unsigned i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      ASSERT(i < MAX_TEXTURES);

      if (m_caps.PointSprite && m_texUnits[i].bCoordReplace) {
        pRegisters[0].fM[0] = fDelta;
        pRegisters[0].fM[1] = TConst<fixedRX>::Zero();
        pRegisters[0].fM[2] = (fDelta >> 1) - fDelta * xmin;

        pRegisters[1].fM[0] = TConst<fixedRX>::Zero();
        pRegisters[1].fM[1] = fDelta;
        pRegisters[1].fM[2] = (fDelta >> 1) - fDelta * ymin;
      } else {
        const TEXCOORD2 *const vTexCoords =
            (const TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + i];
        const TEXCOORD2 &vTexCoord = vTexCoords[index];

        pRegisters[0].fM[0] = TConst<fixedRX>::Zero();
        pRegisters[0].fM[1] = TConst<fixedRX>::Zero();
        pRegisters[0].fM[2] = static_cast<fixedRX>(vTexCoord.m[0]);

        pRegisters[1].fM[0] = TConst<fixedRX>::Zero();
        pRegisters[1].fM[1] = TConst<fixedRX>::Zero();
        pRegisters[1].fM[2] = static_cast<fixedRX>(vTexCoord.m[1]);
      }

      pRegisters += 2;
    }
  }

  if (rasterFlags.Fog) {
    const float20 *const pfFogs =
        (const float20 *)m_pbVertexData[VERTEXDATA_FOG];
    const float20 fFog = pfFogs[index];

    pRegisters->fM[0] = TConst<fixedRX>::Zero();
    pRegisters->fM[1] = TConst<fixedRX>::Zero();
    pRegisters->fM[2] = static_cast<fixedRX>(fFog);
    ++pRegisters;
  }

  // Set the reference offset
  m_rasterData.fRefX = TConst<fixed4>::Zero();
  m_rasterData.fRefY = TConst<fixed4>::Zero();

#ifdef COCOGL_RASTER_PROFILE
  const int64_t startTime = m_timer.GetTime();
  m_rasterData.pRasterOp->StartProfile((ymax - ymin) * (xmax - xmin));
#endif

  for (int y = ymin; y < ymax; ++y) {
    // Raster scanline
    (pfnScanline)(m_rasterData, y, xmin, xmax);
  }

#ifdef COCOGL_RASTER_PROFILE
  m_rasterData.pRasterOp->EndProfile(m_timer.GetElapsedTime(startTime));
#endif
}
