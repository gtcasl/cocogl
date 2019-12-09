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

void Rasterizer::drawTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEX_FLAGS]);
  auto flags0 = pwFlags[i0];
  auto flags1 = pwFlags[i1];
  auto flags2 = pwFlags[i2];

  // Check if we need to clip vertices
  auto clipUnion = (flags0 | flags1 | flags2) & CLIPPING_MASK;
  if (0 == clipUnion) {
    // Do back-face culling in screen space
    if (this->cullScreenSpaceTriangle(i0, i1, i2)) {
      // Raster the triangle
      this->rasterTriangle(i0, i1, i2);
    }
  } else {
    // Discard primitives falling outside of the same plane.
    auto clipIntersect = (flags0 & flags1 & flags2) & CLIPPING_MASK;
    if (0 == clipIntersect) {
      // Do Back-face culling in clip space
      if (this->cullClipSpaceTriangle(i0, i1, i2)) {
        // Clip and raster the triangle
        this->rasterClippedTriangle(i0, i1, i2, clipUnion);
      }
    }
  }
}

bool Rasterizer::cullScreenSpaceTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEX_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2];

  auto i4dx10 = v1.x - v0.x;
  auto i4dy10 = v1.y - v0.y;
  auto i4dx20 = v2.x - v0.x;
  auto i4dy20 = v2.y - v0.y;

  uint32_t culled;

  auto cullStates = cullStates_;
  if (CULL_FRONT_AND_BACK != cullStates.CullFace) {
    auto i8Sign = Math::FastMul<fixed8>(i4dx10, i4dy20) -
                  Math::FastMul<fixed8>(i4dx20, i4dy10);
    if (CULL_BACK == cullStates.CullFace) {
      culled = static_cast<uint32_t>(i8Sign < Math::Zero<fixed8>()) ^
               cullStates.FrontFace;
    } else {
      culled = static_cast<uint32_t>(i8Sign > Math::Zero<fixed8>()) ^
               cullStates.FrontFace;
    }
  } else {
    culled = 1;
  }

  if (cullStates.bCullFaceEnabled) {
    if (culled) {
      return false;
    }
  }

  if (rasterID_.Flags.Color) {
    auto colorIndex = cullStates.bTwoSidedLighting && culled;
    pbVertexColor_ = pbVertexData_[VERTEX_COLOR0 + colorIndex];
  }

  return true;
}

void Rasterizer::rasterTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  TriangleGradient g;

  // Setup triangle attributes  
  if (!this->setupTriangleAttributes(&g, i0, i1, i2))
    return;

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  rasterData_.pRasterOp->startProfile(static_cast<int>(std::abs(g.i8Area)));
#endif

  auto pfnScanline = rasterData_.pRasterOp->getScanline();
  auto bMiddleIsRight = (g.fRatio >= Math::Zero<floatQ>());
  auto fRndCeil = fixedDDA::make(fixedDDA::MASK) - Math::Half<fixedDDA>();

  auto y = std::max<int>(Math::Ceil<int>(g.i4y0 - Math::Half<fixed4>()), scissorRect_.top);
  auto i4Y0Diff = fixed4(y) - (g.i4y0 - Math::Half<fixed4>());

  fixedDDA fx0, fx1;
  fixedDDA fdx0, fdx1;

  if (g.i4dy12.data()) {
    auto y1 = std::min<int>(Math::Ceil<int>(g.i4y1 - Math::Half<fixed4>()), scissorRect_.bottom);
    fdx0 = fixedDDA(g.i4dx12) / g.i4dy12;
    fdx1 = fixedDDA(g.i4dx13) / g.i4dy13;

    if (bMiddleIsRight) {
      __swap(fdx0, fdx1);
    }

    fx0 = fixedDDA(g.i4x0) + fdx0 * i4Y0Diff + fRndCeil;
    fx1 = fixedDDA(g.i4x0) + fdx1 * i4Y0Diff + fRndCeil;

    for (; y < y1; ++y, fx0 += fdx0, fx1 += fdx1) {
      auto x0 = std::max<int>(static_cast<int>(fx0), scissorRect_.left);
      auto x1 = std::min<int>(static_cast<int>(fx1), scissorRect_.right);

      if (x0 < x1) {
        (pfnScanline)(rasterData_, y, x0, x1);
      }
    }
  } else {
    auto fdx2 = fixedDDA(g.i4dx13) / g.i4dy13;
    auto fx2 = fixedDDA(g.i4x0) + fdx2 * i4Y0Diff + fRndCeil;

    if (bMiddleIsRight) {
      fdx0 = fdx2;
      fx0 = fx2;
    } else {
      fdx1 = fdx2;
      fx1 = fx2;
    }
  }

  if (g.i4dy23.data()) {
    auto y2 = std::min<int>(Math::Ceil<int>(g.i4y2 - Math::Half<fixed4>()), scissorRect_.bottom);
    auto i4Y1Diff = fixed4(y) - (g.i4y1 - Math::Half<fixed4>());
    auto fdx2 = fixedDDA(g.i4dx23) / g.i4dy23;
    auto fx2 = fixedDDA(g.i4x1) + fdx2 * i4Y1Diff + fRndCeil;

    if (bMiddleIsRight) {
      fdx1 = fdx2;
      fx1 = fx2;
    } else {
      fdx0 = fdx2;
      fx0 = fx2;
    }

    for (; y < y2; ++y, fx0 += fdx0, fx1 += fdx1) {
      auto x0 = std::max<int>(static_cast<int>(fx0), scissorRect_.left);
      auto x1 = std::min<int>(static_cast<int>(fx1), scissorRect_.right);

      if (x0 < x1) {
        (pfnScanline)(rasterData_, y, x0, x1);
      }
    }
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
  rasterData_.pRasterOp->endProfile(elapsed_time.count());
#endif
}

bool Rasterizer::setupTriangleAttributes(TriangleGradient *g, 
                                         uint32_t i0, 
                                         uint32_t i1, 
                                         uint32_t i2) {
   // Backup the rasterID
  auto rasterID = rasterID_;

  auto rasterFlags = rasterID.Flags;
  auto pRegister = rasterData_.Registers;  
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEX_SCREENPOS]);

  auto i4y0 = pvScreenPos[i0].y;
  auto i4y1 = pvScreenPos[i1].y;
  auto i4y2 = pvScreenPos[i2].y;

  // Sort vertices by y-coordinate
  if (i4y0 > i4y1) {
    std::swap(i4y0, i4y1);
    std::swap(i0, i1);
  }
  if (i4y0 > i4y2) {
    i4y0 = std::exchange(i4y2, std::exchange(i4y1, i4y0));
    i0 = std::exchange(i2, std::exchange(i1, i0));
  } else if (i4y1 > i4y2) {
    std::swap(i4y1, i4y2);
    std::swap(i1, i2);
  }

  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2]; 

  auto i4x0 = v0.x;
  auto i4x1 = v1.x;
  auto i4x2 = v2.x; 

  auto i4dx12 = (i4x1 - i4x0);
  auto i4dy12 = (i4y1 - i4y0);
  auto i4dx13 = (i4x2 - i4x0);

  auto i4dy13 = (i4y2 - i4y0);
  auto i4dx23 = (i4x2 - i4x1);
  auto i4dy23 = (i4y2 - i4y1);

  auto i8Area = Math::FastMul<fixed8>(i4dx12, i4dy13) -
                Math::FastMul<fixed8>(i4dx13, i4dy12);

  // Reject small areas (1/4 x 1/4 = 1/16)
  auto u8Area = std::abs(i8Area);
  if (u8Area.data() < 16)
    return false;

  g->i4dx12 = i4dx12;
  g->i4dy12 = i4dy12;
  g->i4dx13 = i4dx13;

  g->i4dy13 = i4dy13;
  g->i4dx23 = i4dx23;
  g->i4dy23 = i4dy23;
  
  g->fRatio = Math::Inverse<floatQ>(i8Area);

  // Set the reference offset
  rasterData_.fRefX = v0.x - Math::Half<fixed4>();
  rasterData_.fRefY = v0.y - Math::Half<fixed4>();

  if (rasterFlags.DepthTest) {
    pRegister = this->applyDepthGradient(pRegister, *g, v0, v1, v2);
  }

  if (rasterFlags.Color) {
    pRegister = this->applyColorGradient(pRegister, *g, i0, i1, i2);
  }

  if (rasterFlags.NumTextures) {
    if ((fixedRX(std::abs(v1.rhw - v0.rhw)) > Math::Epsilon<fixedRX>()) ||
        (fixedRX(std::abs(v2.rhw - v0.rhw)) > Math::Epsilon<fixedRX>())) {
      pRegister = this->applyPerspectiveTextureGradient(pRegister, *g, i0, i1, i2);
    } else {
      pRegister = this->applyAffineTextureGradient(pRegister, *g, i0, i1, i2);
    }
  }

  if (rasterFlags.Fog) {
    pRegister = this->applyFogGradient(pRegister, *g, i0, i1, i2);
  }

  // Generate the rasterization routine
  if (!this->generateRasterOp()) {
    error_ = GL_INVALID_OPERATION;
    __glLogError("Rasterizer::generateRasterOp() failed.\r\n");
    return false;
  }

  // Restore the rasterID
  rasterID_ = rasterID;

  g->i4x0 = i4x0;
  g->i4y0 = i4y0;
  g->i4x1 = i4x1;
  g->i4y1 = i4y1;
  g->i4x2 = i4x2;
  g->i4y2 = i4y2;

#ifdef COCOGL_RASTER_PROFILE
  g->i8Area = i8Area;
#endif

  return true;
}

Register *Rasterizer::applyDepthGradient(Register *pRegister,
                                         const TriangleGradient &g,
                                         const RDVECTOR &v0, 
                                         const RDVECTOR &v1,
                                         const RDVECTOR &v2) {
  assert(pRegister);

  float20 delta[2] = {v1.z - v0.z, v2.z - v0.z};

  if ((fixed16(std::abs(delta[0])) > Math::Epsilon<fixed16>()) ||
      (fixed16(std::abs(delta[1])) > Math::Epsilon<fixed16>())) {
    pRegister->m[0] = g.calcDeltaX<fixedRX>(delta[0], delta[1]);
    pRegister->m[1] = g.calcDeltaY<fixedRX>(delta[0], delta[1]);
    rasterID_.Flags.InterpolateDepth = 1;
  } else {
    pRegister->m[0] = Math::Zero<fixedRX>();
    pRegister->m[1] = Math::Zero<fixedRX>();
  }

  pRegister->m[2] = static_cast<fixedRX>(v0.z);

  if (caps_.PolygonOffsetFill) {
    this->applyPolygonOffset(pRegister);
  }

  return pRegister + 1;
}

void Rasterizer::applyPolygonOffset(Register *pRegister) {
  assert(pRegister);

  auto fSlope = static_cast<float20>(
      std::max(std::abs(pRegister->m[0]), std::abs(pRegister->m[1])));

  auto fOffset = static_cast<floatf>((polygonOffset_.fFactor * fSlope) +
                                     polygonOffset_.fUnits * Math::Epsilon<floatf>());

  pRegister->m[2] = static_cast<fixedRX>(
      Math::Sat(static_cast<floatf>(pRegister->m[2]) + fOffset));
}

Register *Rasterizer::applyColorGradient(Register *pRegister,
                                         const TriangleGradient &g,
                                         uint32_t i0, 
                                         uint32_t i1,
                                         uint32_t i2) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;

  // Lookup vertex colors
  auto pcColors = reinterpret_cast<ColorARGB *>(pbVertexColor_);
  auto c0 = pcColors[i0];
  auto c1 = pcColors[i1];
  auto c2 = pcColors[i2];

  auto interpolateMask = (rasterFlags.InterpolateColor ? 0x7 : 0) |
                         (rasterFlags.InterpolateAlpha << 3);
  if (interpolateMask) {
    for (uint32_t i = 0; i < 4; ++i) {
      int delta[2] = {c1.m[i] - c0.m[i], c2.m[i] - c0.m[i]};
      pRegister[i].m[0] = g.calcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = g.calcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(c0.m[i]) >> 8;
    }
    rasterID_.Flags.InterpolateColor = (interpolateMask & 0x7) ? 1 : 0;
    rasterID_.Flags.InterpolateAlpha = interpolateMask >> 3;
  } else {
    for (uint32_t i = 0; i < 4; ++i) {
      pRegister[i].m[0] = Math::Zero<fixedRX>();
      pRegister[i].m[1] = Math::Zero<fixedRX>();
      pRegister[i].m[2] = static_cast<fixedRX>(c2.m[i]) >> 8;
    }
  }

  return pRegister + 4;
}

Register *
Rasterizer::applyAffineTextureGradient(Register *pRegister,
                                       const TriangleGradient &g,
                                       uint32_t i0, 
                                       uint32_t i1, 
                                       uint32_t i2) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;
  auto numTextures = rasterFlags.NumTextures;

  for (uint32_t j = 0; j < numTextures; ++j) {
    assert(j < MAX_TEXTURES);

    auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(pbVertexData_[VERTEX_TEXCOORD0 + j]);
    auto &uv0 = vTexCoords[i0];
    auto &uv1 = vTexCoords[i1];
    auto &uv2 = vTexCoords[i2];

    for (uint32_t i = 0; i < 2; ++i) {
      float20 delta[2] = {uv1.m[i] - uv0.m[i], uv2.m[i] - uv0.m[i]};
      pRegister[i].m[0] = g.calcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = g.calcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(uv0.m[i]);
    }

    pRegister += 2;
  }

  if (rasterFlags.TextureMips) {
    pRegister = this->applyAffineTextureMipmapGradient(pRegister);
  }

  rasterID_.Flags.Perspective = 0;

  return pRegister;
}

Register *Rasterizer::applyPerspectiveTextureGradient(Register *pRegister, 
                                                      const TriangleGradient &g, 
                                                      uint32_t i0, 
                                                      uint32_t i1, 
                                                      uint32_t i2) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;
  auto numTextures = rasterFlags.NumTextures;

  // Lookup vertex screen positions
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEX_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2];

  floatRW rhws[3] = {v0.rhw, v1.rhw, v2.rhw};

  // Scale up rhw to avoid fixed point overflow
  auto fSatW = Math::One<floatRW>() / 16;
  while ((rhws[0] < fSatW) && 
         (rhws[1] < fSatW) && 
         (rhws[2] < fSatW)) {
    rhws[0] *= 16;
    rhws[1] *= 16;
    rhws[2] *= 16;
  }

  floatRW delta[2] = {rhws[1] - rhws[0], rhws[2] - rhws[0]};

  pRegister->m[0] = g.calcDeltaX<fixedRX>(delta[0], delta[1]);
  pRegister->m[1] = g.calcDeltaY<fixedRX>(delta[0], delta[1]);
  pRegister->m[2] = static_cast<fixedRX>(rhws[0]);

  ++pRegister;

  for (uint32_t j = 0; j < numTextures; ++j) {
    auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(pbVertexData_[VERTEX_TEXCOORD0 + j]);
    auto &uv0 = vTexCoords[i0];
    auto &uv1 = vTexCoords[i1];
    auto &uv2 = vTexCoords[i2];

    for (uint32_t i = 0; i < 2; ++i) {
      auto uvw0 = uv0.m[i] * rhws[0];
      auto uvw1 = uv1.m[i] * rhws[1];
      auto uvw2 = uv2.m[i] * rhws[2];

      float20 delta[2] = {uvw1 - uvw0, uvw2 - uvw0};
      pRegister[i].m[0] = g.calcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = g.calcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(uvw0);
    }

    pRegister += 2;
  }

  if (rasterFlags.TextureMips) {
    pRegister = this->applyPerspectiveTextureMipmapGradient(pRegister, g, v0, v1, v2, rhws);
  }

  rasterID_.Flags.Perspective = 1;

  return pRegister;
}

Register *Rasterizer::applyAffineTextureMipmapGradient(Register *pRegister) {
  auto rasterFlags = rasterID_.Flags;
  auto numTextures = rasterFlags.NumTextures;
  auto textureMips = rasterFlags.TextureMips;
  auto pCur = pRegister - (2 * numTextures);

  for (uint32_t j = 0; j < numTextures; ++j, pCur += 2) {
    assert(j < MAX_TEXTURES);

    if ((textureMips >> j) & 0x1) {
      float20 fMaxDm[2];

      for (uint32_t i = 0; i < 2; ++i) {
        fMaxDm[i] = static_cast<float20>(std::max(std::abs(pCur[i].m[0]), std::abs(pCur[i].m[1])));
      }

      auto &sampler = rasterData_.Samplers[j];
      auto logWidth = sampler.pMipLevels[0].getLogWidth();
      auto logHeight = sampler.pMipLevels[0].getLogHeight();
      auto fMU = Math::Mul<floatf>(fMaxDm[0], 1 << logWidth);
      auto fMV = Math::Mul<floatf>(fMaxDm[1], 1 << logHeight);
      auto fM = std::max(fMU, fMV);

      if (fM > Math::One<floatf>()) {
        if (FILTER_NONE == rasterID_.Textures[j].MipFilter) {
          // Turn off trilinear filtering
          rasterID_.Textures[j].MipFilter = FILTER_NONE;
          rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
          rasterID_.Flags.TextureMips &= ~(1 << j);
        } else {
          auto fJ = static_cast<fixed16>(fM);
          auto mipLevel = Math::iLog2(fJ.data()) - fixed16::FRAC;
          if (mipLevel >= sampler.MaxMipLevel) {
            // Use nearest mipmap filtering
            rasterID_.Textures[j].MipFilter = FILTER_NEAREST;
            rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
            fM = static_cast<floatf>(1 << sampler.MaxMipLevel);
          }

          pRegister->m[2] = fixedRX::make(static_cast<fixed16>(fM).data());
          pRegister->m[0] = Math::Zero<fixedRX>();
          pRegister->m[1] = Math::Zero<fixedRX>();
          ++pRegister;
        }
      } else {
        // Turn off trilinear filtering
        rasterID_.Textures[j].MipFilter = FILTER_NONE;
        rasterID_.Textures[j].MinFilter = rasterID_.Textures[j].MagFilter;
        rasterID_.Flags.TextureMips &= ~(1 << j);
      }
    }
  }

  return pRegister;
}

Register *Rasterizer::applyPerspectiveTextureMipmapGradient(Register *pRegister, 
                                                            const TriangleGradient &g, 
                                                            const RDVECTOR &v0,
                                                            const RDVECTOR &v1, 
                                                            const RDVECTOR &v2, 
                                                            floatRW rhws[3]) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;
  auto textureMips = rasterFlags.TextureMips;
  auto numTextures = rasterFlags.NumTextures;

  floatf fRhw2s[3];

  for (uint32_t k = 0; k < 3; ++k) {
    fRhw2s[k] = Math::Mul<floatf>(rhws[k], rhws[k]);
  }

  auto pCur = pRegister - 2 * numTextures - 1;
  auto fRhwdA = static_cast<float20>(pCur->m[0]);
  auto fRhwdB = static_cast<float20>(pCur->m[1]);
  auto fRhwdC = static_cast<float20>(pCur->m[2]);

  ++pCur;

  const RDVECTOR *vScreenPos[3] = {&v0, &v1, &v2};

  for (uint32_t j = 0; j < numTextures; ++j, pCur += 2) {
    if ((textureMips >> j) & 0x1) {
      float20 fMaxDms[3][2];

      for (uint32_t i = 0; i < 2; ++i) {
        auto fA = static_cast<float20>(pCur[i].m[0]);
        auto fB = static_cast<float20>(pCur[i].m[1]);
        auto fC = static_cast<float20>(pCur[i].m[2]);

        auto fK1 = Math::MulSub<float24>(fA, fRhwdB, fB, fRhwdA);
        auto fK2 = Math::MulSub<float20>(fA, fRhwdC, fRhwdA, fC);
        auto fK3 = Math::MulSub<float20>(fB, fRhwdC, fRhwdB, fC);

        for (uint32_t k = 0; k < 3; ++k) {
          auto fK1x = Math::Mul<float20>(fK1, vScreenPos[k]->x);
          auto fK1y = Math::Mul<float20>(fK1, vScreenPos[k]->y);
          fMaxDms[k][i] = std::max(std::abs(fK2 + fK1y), std::abs(fK3 - fK1x));
        }
      }

      auto &sampler = rasterData_.Samplers[j];
      auto logWidth = sampler.pMipLevels[0].getLogWidth();
      auto logHeight = sampler.pMipLevels[0].getLogHeight();

      floatf fMs[3];

      for (uint32_t k = 0; k < 3; ++k) {
        auto fMU = Math::ShiftLeft<floatf>(fMaxDms[k][0], logWidth);
        auto fMV = Math::ShiftLeft<floatf>(fMaxDms[k][1], logHeight);
        fMs[k] = std::max(fMU, fMV);
      }

      if ((fMs[0] > fRhw2s[0]) || (fMs[1] > fRhw2s[1]) ||
          (fMs[2] > fRhw2s[2])) {
        if ((Math::ShiftRight<floatf>(fMs[0], sampler.MaxMipLevel) < fRhw2s[0]) ||
            (Math::ShiftRight<floatf>(fMs[1], sampler.MaxMipLevel) < fRhw2s[1]) ||
            (Math::ShiftRight<floatf>(fMs[2], sampler.MaxMipLevel) < fRhw2s[2])) {
          floatf delta[2] = {fMs[1] - fMs[0], fMs[2] - fMs[0]};

          if ((fixed16(std::abs(delta[0])) > Math::Epsilon<fixed16>()) ||
              (fixed16(std::abs(delta[1])) > Math::Epsilon<fixed16>())) {
            rasterID_.Flags.InterpolateMips |= (1 << j);
            pRegister->m[2] = fixedRX::make(static_cast<fixed16>(fMs[0]).data());
            pRegister->m[0] = fixedRX::make(g.calcDeltaX<fixed16>(delta[0], delta[1]).data());
            pRegister->m[1] = fixedRX::make(g.calcDeltaY<fixed16>(delta[0], delta[1]).data());
            ++pRegister;
          } else {
            if (fMs[0] > fRhw2s[0]) {
              if (FILTER_NONE == rasterID_.Textures[j].MipFilter) {
                // Turn off trilinear filtering
                rasterID_.Textures[j].MipFilter = FILTER_NONE;
                rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
                rasterID_.Flags.TextureMips &= ~(1 << j);
              } else {
                floatf fM = fMs[0] / fRhw2s[0];
                auto fJ = static_cast<fixed16>(fM);
                auto mipLevel = Math::iLog2(fJ.data()) - fixed16::FRAC;

                if (mipLevel >= sampler.MaxMipLevel) {
                  // Use nearest mipmap filtering
                  rasterID_.Textures[j].MipFilter = FILTER_NEAREST;
                  rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
                  fM = static_cast<floatf>(1 << sampler.MaxMipLevel);
                }

                pRegister->m[2] = fixedRX::make(static_cast<fixed16>(fM).data());
                pRegister->m[0] = Math::Zero<fixedRX>();
                pRegister->m[1] = Math::Zero<fixedRX>();
                ++pRegister;
              }
            } else {
              // Turn off trilinear filtering
              rasterID_.Textures[j].MipFilter = FILTER_NONE;
              rasterID_.Textures[j].MinFilter = rasterID_.Textures[j].MagFilter;
              rasterID_.Flags.TextureMips &= ~(1 << j);
            }
          }
        } else {
          if (FILTER_NONE == rasterID_.Textures[j].MipFilter) {
            // Turn off trilinear filtering
            rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
            rasterID_.Flags.TextureMips &= ~(1 << j);
          } else {
            // Use nearest mipmap filtering
            rasterID_.Textures[j].MipFilter = FILTER_NEAREST;
            rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
            pRegister->m[2] = fixedRX::make(fixed16(1 << sampler.MaxMipLevel).data());
            pRegister->m[0] = Math::Zero<fixedRX>();
            pRegister->m[1] = Math::Zero<fixedRX>();
            ++pRegister;
          }
        }
      } else {
        // Turn off trilinear filtering
        rasterID_.Textures[j].MipFilter = FILTER_NONE;
        rasterID_.Textures[j].MinFilter = rasterID_.Textures[j].MagFilter;
        rasterID_.Flags.TextureMips &= ~(1 << j);
      }
    }
  }

  return pRegister;
}

Register *Rasterizer::applyFogGradient(Register *pRegister,
                                       const TriangleGradient &g,
                                       uint32_t i0, 
                                       uint32_t i1, 
                                       uint32_t i2) {
  assert(pRegister);

  auto pfFogs = reinterpret_cast<float20 *>(pbVertexData_[VERTEX_FOG]);
  auto fFog0 = pfFogs[i0];
  auto fFog1 = pfFogs[i1];
  auto fFog2 = pfFogs[i2];

  float20 delta[2] = {fFog1 - fFog0, fFog2 - fFog0};

  if ((fixed8(std::abs(delta[0])) > Math::Epsilon<fixed8>()) ||
      (fixed8(std::abs(delta[1])) > Math::Epsilon<fixed8>())) {
    pRegister->m[0] = g.calcDeltaX<fixedRX>(delta[0], delta[1]);
    pRegister->m[1] = g.calcDeltaY<fixedRX>(delta[0], delta[1]);
    rasterID_.Flags.InterpolateFog = 1;
  } else {
    pRegister->m[0] = Math::Zero<fixedRX>();
    pRegister->m[1] = Math::Zero<fixedRX>();
  }

  pRegister->m[2] = static_cast<fixedRX>(fFog0);

  return pRegister + 1;
}