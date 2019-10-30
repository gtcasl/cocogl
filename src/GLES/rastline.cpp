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

void CRasterizer::DrawLine(unsigned i0, unsigned i1) {
  const uint16_t *const pwFlags =
      (const uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  const unsigned flags0 = pwFlags[i0];
  const unsigned flags1 = pwFlags[i1];

  // Check if we need to clip vertices
  const unsigned clipUnion = (flags0 | flags1) & CLIPPING_MASK;
  if (0 == clipUnion) {
    // Raster the line
    this->RasterLine(i0, i1);
  } else {
    // Discard primitives falling outside of the same plane.
    const unsigned clipIntersect = (flags0 & flags1) & CLIPPING_MASK;
    if (0 == clipIntersect) {
      // Clip and raster the triangle
      this->RasterClippedLine(i0, i1, clipUnion);
    }
  }
}

void CRasterizer::RasterLine(unsigned i0, unsigned i1) {
  LineGradient gradient;

  // Setup the line attributes
  if (!this->SetupLineAttributes(&gradient, i0, i1)) {
    return;
  }

  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const RDVECTOR &v0 = pvScreenPos[i0];
  const RDVECTOR &v1 = pvScreenPos[i1];

  const PFN_Scanline pfnScanline = m_rasterData.pRasterOp->GetScanline();

  const fixedDDA fLineWidth = Math::TCast<fixedDDA>(m_fLineWidth);
  const fixedDDA fRndCeil =
      fixedDDA::Make(fixedDDA::MASK) - TConst<fixedDDA>::Half();

  const fixed4 i4dx = v1.x - v0.x;
  const fixed4 i4dy = v1.y - v0.y;

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  m_rasterData.pRasterOp->StartProfile(
      Math::TCast<int>(Math::TAbs(i4dx) + Math::TAbs(i4dy)));
#endif

  if (Math::TAbs(i4dx) > Math::TAbs(i4dy)) {
    // Set the reference offset
    m_rasterData.fRefX = v0.x - TConst<fixed4>::Half();
    m_rasterData.fRefY = v0.y - TConst<fixed4>::Half();

    // X-axis sort
    const RDVECTOR *vertices[2] = {&v0, &v1};
    if (i4dx < TConst<fixed4>::Zero()) {
      gradient.fRatio = -gradient.fRatio;
      vertices[0] = &v1;
      vertices[1] = &v0;
    }

    const fixed4 i4x0 = vertices[0]->x;
    const fixed4 i4y0 = vertices[0]->y;
    const fixed4 i4x1 = vertices[1]->x;
    const fixed4 i4y1 = vertices[1]->y;

    const fixedDDA fddy = Math::TMul<fixedDDA>(i4y1 - i4y0, gradient.fRatio);

    const int x0 = Math::TMax<int>(
        Math::TCeili<int>(i4x0 - TConst<fixed4>::Half()), m_scissorRect.left);

    const int x1 = Math::TMin<int>(
        Math::TCeili<int>(i4x1 - TConst<fixed4>::Half()), m_scissorRect.right);

    const fixed4 i4X0Diff = fixed4(x0) - (i4x0 - TConst<fixed4>::Half());

    fixedDDA fty =
        fixedDDA(i4y0) + fddy * i4X0Diff - (fLineWidth / 2) + fRndCeil;
    fixedDDA fby = fty + fLineWidth;

    for (int x = x0; x < x1; ++x) {
      const int y0 = Math::TMax<int>(Math::TCast<int>(fty), m_scissorRect.top);
      const int y1 =
          Math::TMin<int>(Math::TCast<int>(fby), m_scissorRect.bottom);

      for (int y = y0; y < y1; ++y) {
        (pfnScanline)(m_rasterData, y, x, x + 1);
      }

      fty += fddy;
      fby += fddy;
    }
  } else {
    // Set the reference offset
    m_rasterData.fRefX = v0.x - TConst<fixed4>::Half();
    m_rasterData.fRefY = v0.y - TConst<fixed4>::Half();

    // Y-axis sort
    const RDVECTOR *vertices[2] = {&v0, &v1};
    if (i4dy < TConst<fixed4>::Zero()) {
      gradient.fRatio = -gradient.fRatio;
      vertices[0] = &v1;
      vertices[1] = &v0;
    }

    const fixed4 i4x0 = vertices[0]->x;
    const fixed4 i4y0 = vertices[0]->y;
    const fixed4 i4x1 = vertices[1]->x;
    const fixed4 i4y1 = vertices[1]->y;

    const fixedDDA fddx = Math::TMul<fixedDDA>(i4x1 - i4x0, gradient.fRatio);

    const int y0 = Math::TMax<int>(
        Math::TCeili<int>(i4y0 - TConst<fixed4>::Half()), m_scissorRect.top);

    const int y1 = Math::TMin<int>(
        Math::TCeili<int>(i4y1 - TConst<fixed4>::Half()), m_scissorRect.bottom);

    const fixed4 i4Y0Diff = fixed4(y0) - (i4y0 - TConst<fixed4>::Half());

    fixedDDA flx =
        fixedDDA(i4x0) + fddx * i4Y0Diff - (fLineWidth / 2) + fRndCeil;
    fixedDDA frx = flx + fLineWidth;

    for (int y = y0; y < y1; ++y) {
      const int x0 = Math::TMax<int>(Math::TCast<int>(flx), m_scissorRect.left);
      const int x1 =
          Math::TMin<int>(Math::TCast<int>(frx), m_scissorRect.right);

      (pfnScanline)(m_rasterData, y, x0, x1);

      flx += fddx;
      frx += fddx;
    }
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(
      end_time - start_time);
  m_rasterData.pRasterOp->EndProfile(elapsed_time.count());
#endif
}

bool CRasterizer::SetupLineAttributes(LineGradient *pGradient, unsigned i0,
                                      unsigned i1) {
  const RASTERFLAGS rasterFlags = m_rasterID.Flags;
  Register *pRegister = m_rasterData.Registers;

  const RDVECTOR *const pvScreenPos =
      (const RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];
  const RDVECTOR &v0 = pvScreenPos[i0];
  const RDVECTOR &v1 = pvScreenPos[i1];

  const fixed4 i4dx = v1.x - v0.x;
  const fixed4 i4dy = v1.y - v0.y;

  unsigned attribIdx;

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
    const float20 delta = v1.z - v0.z;
    if (fixed16(Math::TAbs(delta)) > TConst<fixed16>::Epsilon()) {
      pRegister->m[attribIdx] = pGradient->TCalcDelta<fixedRX>(delta);
      pRegister->m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      m_rasterID.Flags.InterpolateDepth = 1;
    } else {
      pRegister->m[0] = TConst<fixedRX>::Zero();
      pRegister->m[1] = TConst<fixedRX>::Zero();
    }
    pRegister->m[2] = static_cast<fixedRX>(v0.z);
    ++pRegister;
  }

  if (rasterFlags.Color) {
    const ColorARGB *const pcColors = (const ColorARGB *)m_pbVertexColor;
    const ColorARGB c0 = pcColors[i0];
    const ColorARGB c1 = pcColors[i1];

    unsigned interpolateMask = (rasterFlags.InterpolateColor ? 0x7 : 0) |
                               (rasterFlags.InterpolateAlpha << 3);
    if (interpolateMask) {
      for (unsigned i = 0; i < 4; ++i) {
        const int delta = c1.m[i] - c0.m[i];
        if (Math::TAbs(delta) > 1) {
          pRegister[i].m[attribIdx] = pGradient->TCalcDelta<fixedRX>(delta);
          pRegister[i].m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
        } else {
          pRegister->m[0] = TConst<fixedRX>::Zero();
          pRegister->m[1] = TConst<fixedRX>::Zero();
          interpolateMask &= ~(1 << i);
        }
        pRegister[i].m[2] = static_cast<fixedRX>(c0.m[i]) >> fixed8::FRAC;
      }

      m_rasterID.Flags.InterpolateColor = (interpolateMask & 0x7) ? 1 : 0;
      m_rasterID.Flags.InterpolateAlpha = interpolateMask >> 3;
    } else {
      for (unsigned i = 0; i < 4; ++i) {
        pRegister[i].m[0] = TConst<fixedRX>::Zero();
        pRegister[i].m[1] = TConst<fixedRX>::Zero();
        pRegister[i].m[2] = static_cast<fixedRX>(c1.m[i]) >> fixed8::FRAC;
      }
    }

    pRegister += 4;
  }

  if (rasterFlags.NumTextures) {
    for (unsigned i = 0, n = rasterFlags.NumTextures; i < n; ++i) {
      ASSERT(i < MAX_TEXTURES);

      const TEXCOORD2 *const vTexCoords =
          (const TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + i];
      const TEXCOORD2 &uv0 = vTexCoords[i0];
      const TEXCOORD2 &uv1 = vTexCoords[i1];

      pRegister[0].m[attribIdx] =
          pGradient->TCalcDelta<fixedRX>(uv1.m[0] - uv0.m[0]);
      pRegister[0].m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      pRegister[0].m[2] = static_cast<fixedRX>(uv0.m[0]);

      pRegister[1].m[attribIdx] =
          pGradient->TCalcDelta<fixedRX>(uv1.m[1] - uv0.m[1]);
      pRegister[1].m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      pRegister[1].m[2] = static_cast<fixedRX>(uv0.m[1]);

      pRegister += 2;
    }
  }

  if (rasterFlags.Fog) {
    const float20 *const pfFogs =
        (const float20 *)m_pbVertexData[VERTEXDATA_FOG];
    const float20 fFog0 = pfFogs[i0];
    const float20 fFog1 = pfFogs[i1];

    const float20 delta = fFog1 - fFog0;
    if (fixedRX(Math::TAbs(delta)) > TConst<fixedRX>::Epsilon()) {
      pRegister->m[attribIdx] = pGradient->TCalcDelta<fixedRX>(delta);
      pRegister->m[attribIdx ^ 0x1] = TConst<fixedRX>::Zero();
      m_rasterID.Flags.InterpolateFog = 1;
    } else {
      pRegister->m[0] = TConst<fixedRX>::Zero();
      pRegister->m[1] = TConst<fixedRX>::Zero();
    }
    pRegister->m[2] = static_cast<fixedRX>(fFog0);
    ++pRegister;
  }

  // Generate the rasterization routine
  if (!this->GenerateRasterOp()) {
    __glLogError(_T("CRasterizer::GenerateRasterOp() failed.\r\n"));
    return false;
  }

  return true;
}