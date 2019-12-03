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

void CRasterizer::drawTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEXDATA_FLAGS]);
  uint32_t flags0 = pwFlags[i0];
  uint32_t flags1 = pwFlags[i1];
  uint32_t flags2 = pwFlags[i2];

  // Check if we need to clip vertices
  uint32_t clipUnion = (flags0 | flags1 | flags2) & CLIPPING_MASK;
  if (0 == clipUnion) {
    // Do back-face culling in screen space
    if (this->cullScreenSpaceTriangle(i0, i1, i2)) {
      // Raster the triangle
      this->rasterTriangle(i0, i1, i2);
    }
  } else {
    // Discard primitives falling outside of the same plane.
    uint32_t clipIntersect = (flags0 & flags1 & flags2) & CLIPPING_MASK;
    if (0 == clipIntersect) {
      // Do Back-face culling in clip space
      if (this->cullClipSpaceTriangle(i0, i1, i2)) {
        // Clip and raster the triangle
        this->rasterClippedTriangle(i0, i1, i2, clipUnion);
      }
    }
  }
}

bool CRasterizer::cullScreenSpaceTriangle(uint32_t i0, uint32_t i1,
                                          uint32_t i2) {
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2];

  auto fdx10 = v1.x - v0.x;
  auto fdy10 = v1.y - v0.y;
  auto fdx20 = v2.x - v0.x;
  auto fdy20 = v2.y - v0.y;

  uint32_t culled;

  auto cullStates = cullStates_;
  if (CULL_FRONT_AND_BACK != cullStates.CullFace) {
    auto fSign = Math::TFastMul<fixed8>(fdx10, fdy20) -
                 Math::TFastMul<fixed8>(fdx20, fdy10);
    if (CULL_BACK == cullStates.CullFace) {
      culled = static_cast<uint32_t>(fSign < TConst<fixed8>::Zero()) ^ cullStates.FrontFace;
    } else {
      culled = static_cast<uint32_t>(fSign > TConst<fixed8>::Zero()) ^ cullStates.FrontFace;
    }
  } else {
    culled = 1;
  }

  if (cullStates.bCullFaceEnabled) {
    if (culled) {
      return false;
    }
  }

  uint32_t colorIndex = cullStates.bTwoSidedLighting && culled;
  pbVertexColor_ = pbVertexData_[VERTEXDATA_COLOR0 + colorIndex];

  return true;
}

void CRasterizer::rasterTriangle(uint32_t i0, uint32_t i1, uint32_t i2) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  // Setup the triangle attributes
  if (!this->setupTriangleAttributes(i0, i1, i2))
    return;

  auto pfnScanline = rasterData_.pRasterOp->getScanline();

  // Lookup screenspace positions
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2];

  // Sort vertices by y-coordinate
  const RDVECTOR *vertices[3] = {&v0, &v1, &v2};
  if (v1.y < vertices[0]->y) {
    vertices[1] = vertices[0];
    vertices[0] = &v1;
  }

  if (v2.y < vertices[0]->y) {
    vertices[2] = vertices[1];
    vertices[1] = vertices[0];
    vertices[0] = &v2;
  } else if (v2.y < vertices[1]->y) {
    vertices[2] = vertices[1];
    vertices[1] = &v2;
  }

  // Setup triangle coordinates
  auto i4x0 = vertices[0]->x;
  auto i4x1 = vertices[1]->x;
  auto i4x2 = vertices[2]->x;

  auto i4y0 = vertices[0]->y;
  auto i4y1 = vertices[1]->y;
  auto i4y2 = vertices[2]->y;

  auto i4dx12 = (i4x1 - i4x0);
  auto i4dy12 = (i4y1 - i4y0);
  auto i4dx13 = (i4x2 - i4x0);

  auto i4dy13 = (i4y2 - i4y0);
  auto i4dx23 = (i4x2 - i4x1);
  auto i4dy23 = (i4y2 - i4y1);

  // Calculate the edge direction
  auto i8Area = Math::TFastMul<fixed8>(i4dx12, i4dy13) -
                Math::TFastMul<fixed8>(i4dx13, i4dy12);
  bool bMiddleIsRight = (i8Area >= TConst<fixed8>::Zero());
  auto fRndCeil = fixedDDA::make(fixedDDA::MASK) - TConst<fixedDDA>::Half();

  int y = Math::TMax<int>(Math::TCeili<int>(i4y0 - TConst<fixed4>::Half()), scissorRect_.top);
  auto i4Y0Diff = fixed4(y) - (i4y0 - TConst<fixed4>::Half());

#ifdef COCOGL_RASTER_PROFILE
  auto start_time = std::chrono::high_resolution_clock::now();
  rasterData_.pRasterOp->StartProfile(static_cast<int>(Math::TAbs(i8Area)));
#endif

  fixedDDA fx0, fx1;
  fixedDDA fdx0, fdx1;

  if (i4dy12.data()) {
    int y1 = Math::TMin<int>(Math::TCeili<int>(i4y1 - TConst<fixed4>::Half()), scissorRect_.bottom);
    fdx0 = fixedDDA(i4dx12) / i4dy12;
    fdx1 = fixedDDA(i4dx13) / i4dy13;

    if (bMiddleIsRight) {
      __swap(fdx0, fdx1);
    }

    fx0 = fixedDDA(i4x0) + fdx0 * i4Y0Diff + fRndCeil;
    fx1 = fixedDDA(i4x0) + fdx1 * i4Y0Diff + fRndCeil;

    for (; y < y1; ++y, fx0 += fdx0, fx1 += fdx1) {
      auto x0 = Math::TMax<int>(static_cast<int>(fx0), scissorRect_.left);
      auto x1 = Math::TMin<int>(static_cast<int>(fx1), scissorRect_.right);

      if (x0 < x1) {
        (pfnScanline)(rasterData_, y, x0, x1);
      }
    }
  } else {
    fixedDDA fdx2 = fixedDDA(i4dx13) / i4dy13;
    fixedDDA fx2 = fixedDDA(i4x0) + fdx2 * i4Y0Diff + fRndCeil;

    if (bMiddleIsRight) {
      fdx0 = fdx2;
      fx0 = fx2;
    } else {
      fdx1 = fdx2;
      fx1 = fx2;
    }
  }

  if (i4dy23.data()) {
    auto y2 = Math::TMin<int>(Math::TCeili<int>(i4y2 - TConst<fixed4>::Half()),  scissorRect_.bottom);
    auto i4Y1Diff = fixed4(y) - (i4y1 - TConst<fixed4>::Half());
    auto fdx2 = fixedDDA(i4dx23) / i4dy23;
    auto fx2 = fixedDDA(i4x1) + fdx2 * i4Y1Diff + fRndCeil;

    if (bMiddleIsRight) {
      fdx1 = fdx2;
      fx1 = fx2;
    } else {
      fdx0 = fdx2;
      fx0 = fx2;
    }

    for (; y < y2; ++y, fx0 += fdx0, fx1 += fdx1) {
      auto x0 = Math::TMax<int>(static_cast<int>(fx0), scissorRect_.left);
      auto x1 = Math::TMin<int>(static_cast<int>(fx1), scissorRect_.right);

      if (x0 < x1) {
        (pfnScanline)(rasterData_, y, x0, x1);
      }
    }
  }

#ifdef COCOGL_RASTER_PROFILE
  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
  rasterData_.pRasterOp->EndProfile(elapsed_time.count());
#endif
}

bool CRasterizer::setupTriangleAttributes(uint32_t i0, uint32_t i1,
                                          uint32_t i2) {
  // Lookup vertex screen positions
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2];

  auto i4dx12 = v1.x - v0.x;
  auto i4dy12 = v1.y - v0.y;
  auto i4dx13 = v2.x - v0.x;
  auto i4dy13 = v2.y - v0.y;

  auto i8Area = Math::TFastMul<fixed8>(i4dx12, i4dy13) -
                Math::TFastMul<fixed8>(i4dx13, i4dy12);

  // Reject small areas (1/4 x 1/4 = 1/16)
  auto u8Area = Math::TAbs(i8Area);
  if (u8Area.data() < 16) {
    return false;
  }

  auto fRatio = Math::TInv<floatQ>(i8Area);
  TriangleGradient gradient(i4dx12, i4dy12, i4dx13, i4dy13, fRatio);

  // Backup the rasterID
  auto rasterID = rasterID_;

  auto rasterFlags = rasterID.Flags;
  auto pRegister = rasterData_.Registers;

  if (rasterFlags.DepthTest) {
    pRegister = this->applyDepthGradient(pRegister, gradient, v0, v1, v2);
  }

  if (rasterFlags.Color) {
    pRegister = this->applyColorGradient(pRegister, gradient, i0, i1, i2);
  }

  if (rasterFlags.NumTextures) {
    if ((u8Area > TConst<fixed8>::Half()) &&
        ((fixedRX(Math::TAbs(v1.rhw - v0.rhw)) > TConst<fixedRX>::Epsilon()) ||
         (fixedRX(Math::TAbs(v2.rhw - v0.rhw)) > TConst<fixedRX>::Epsilon()))) {
      pRegister =
          this->applyPerspectiveTextureGradient(pRegister, gradient, i0, i1, i2);
    } else {
      pRegister = this->applyAffineTextureGradient(pRegister, gradient, i0, i1, i2);
    }
  }

  if (rasterFlags.Fog) {
    pRegister = this->applyFogGradient(pRegister, gradient, i0, i1, i2);
  }

  // Set the reference offset
  rasterData_.fRefX = v0.x - TConst<fixed4>::Half();
  rasterData_.fRefY = v0.y - TConst<fixed4>::Half();

  // Generate the rasterization routine
  if (!this->generateRasterOp()) {
    error_ = GL_INVALID_OPERATION;
    __glLogError("CRasterizer::generateRasterOp() failed.\r\n");
    return false;
  }

  // Restore the rasterID
  rasterID_ = rasterID;

  return true;
}

Register *CRasterizer::applyDepthGradient(Register *pRegister,
                                     const TriangleGradient &gradient,
                                     const RDVECTOR &v0, const RDVECTOR &v1,
                                     const RDVECTOR &v2) {
  assert(pRegister);

  float20 delta[2] = {v1.z - v0.z, v2.z - v0.z};

  if ((fixed16(Math::TAbs(delta[0])) > TConst<fixed16>::Epsilon()) ||
      (fixed16(Math::TAbs(delta[1])) > TConst<fixed16>::Epsilon())) {
    pRegister->m[0] = gradient.calcDeltaX<fixedRX>(delta[0], delta[1]);
    pRegister->m[1] = gradient.calcDeltaY<fixedRX>(delta[0], delta[1]);
    rasterID_.Flags.InterpolateDepth = 1;
  } else {
    pRegister->m[0] = TConst<fixedRX>::Zero();
    pRegister->m[1] = TConst<fixedRX>::Zero();
  }

  pRegister->m[2] = static_cast<fixedRX>(v0.z);

  if (caps_.PolygonOffsetFill) {
    this->applyPolygonOffset(pRegister);
  }

  return pRegister + 1;
}

void CRasterizer::applyPolygonOffset(Register *pRegister) {
  assert(pRegister);

  auto fSlope = static_cast<float20>(
      Math::TMax(Math::TAbs(pRegister->m[0]), Math::TAbs(pRegister->m[1])));

  auto fOffset = static_cast<floatf>((polygonOffset_.fFactor * fSlope) +
                                     polygonOffset_.fUnits * fEPS);

  pRegister->m[2] = static_cast<fixedRX>(
      Math::TSat(static_cast<floatf>(pRegister->m[2]) + fOffset));
}

Register *CRasterizer::applyColorGradient(Register *pRegister,
                                     const TriangleGradient &gradient,
                                     uint32_t i0, uint32_t i1, uint32_t i2) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;

  // Lookup vertex colors
  auto pcColors = reinterpret_cast<ColorARGB *>(pbVertexColor_);
  auto c0 = pcColors[i0];
  auto c1 = pcColors[i1];
  auto c2 = pcColors[i2];

  uint32_t interpolateMask = (rasterFlags.InterpolateColor ? 0x7 : 0) |
                             (rasterFlags.InterpolateAlpha << 3);
  if (interpolateMask) {
    for (uint32_t i = 0; i < 4; ++i) {
      int delta[2] = {c1.m[i] - c0.m[i], c2.m[i] - c0.m[i]};
      pRegister[i].m[0] = gradient.calcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = gradient.calcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(c0.m[i]) >> 8;
    }
    rasterID_.Flags.InterpolateColor = (interpolateMask & 0x7) ? 1 : 0;
    rasterID_.Flags.InterpolateAlpha = interpolateMask >> 3;
  } else {
    for (uint32_t i = 0; i < 4; ++i) {
      pRegister[i].m[0] = TConst<fixedRX>::Zero();
      pRegister[i].m[1] = TConst<fixedRX>::Zero();
      pRegister[i].m[2] = static_cast<fixedRX>(c2.m[i]) >> 8;
    }
  }

  return pRegister + 4;
}

Register *CRasterizer::applyAffineTextureGradient(Register *pRegister,
                                             const TriangleGradient &gradient,
                                             uint32_t i0, uint32_t i1,
                                             uint32_t i2) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;
  auto numTextures = rasterFlags.NumTextures;

  for (uint32_t j = 0; j < numTextures; ++j) {
    assert(j < MAX_TEXTURES);

    auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(pbVertexData_[VERTEXDATA_TEXCOORD0 + j]);
    auto &uv0 = vTexCoords[i0];
    auto &uv1 = vTexCoords[i1];
    auto &uv2 = vTexCoords[i2];

    for (uint32_t i = 0; i < 2; ++i) {
      float20 delta[2] = {uv1.m[i] - uv0.m[i], uv2.m[i] - uv0.m[i]};
      pRegister[i].m[0] = gradient.calcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = gradient.calcDeltaY<fixedRX>(delta[0], delta[1]);
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

Register *
CRasterizer::applyPerspectiveTextureGradient(Register *pRegister,
                                        const TriangleGradient &gradient,
                                        uint32_t i0, uint32_t i1, uint32_t i2) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;
  auto numTextures = rasterFlags.NumTextures;

  // Lookup vertex screen positions
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);
  auto &v0 = pvScreenPos[i0];
  auto &v1 = pvScreenPos[i1];
  auto &v2 = pvScreenPos[i2];

  floatRW rhws[3] = {v0.rhw, v1.rhw, v2.rhw};

  // Scale up rhw to avoid fixed point overflow
  auto fSatW = TConst<floatRW>::One() / 16;
  while ((rhws[0] < fSatW) && (rhws[1] < fSatW) && (rhws[2] < fSatW)) {
    rhws[0] *= 16;
    rhws[1] *= 16;
    rhws[2] *= 16;
  }

  float20 delta[2] = {rhws[1] - rhws[0], rhws[2] - rhws[0]};

  pRegister->m[0] = gradient.calcDeltaX<fixedRX>(delta[0], delta[1]);
  pRegister->m[1] = gradient.calcDeltaY<fixedRX>(delta[0], delta[1]);
  pRegister->m[2] = static_cast<fixedRX>(rhws[0]);

  ++pRegister;

  for (uint32_t j = 0; j < numTextures; ++j) {
    auto vTexCoords = reinterpret_cast<TEXCOORD2 *>(pbVertexData_[VERTEXDATA_TEXCOORD0 + j]);
    auto &uv0 = vTexCoords[i0];
    auto &uv1 = vTexCoords[i1];
    auto &uv2 = vTexCoords[i2];

    for (uint32_t i = 0; i < 2; ++i) {
      auto uvw0 = uv0.m[i] * rhws[0];
      auto uvw1 = uv1.m[i] * rhws[1];
      auto uvw2 = uv2.m[i] * rhws[2];

      float20 delta[2] = {uvw1 - uvw0, uvw2 - uvw0};
      pRegister[i].m[0] = gradient.calcDeltaX<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[1] = gradient.calcDeltaY<fixedRX>(delta[0], delta[1]);
      pRegister[i].m[2] = static_cast<fixedRX>(uvw0);
    }

    pRegister += 2;
  }

  if (rasterFlags.TextureMips) {
    pRegister = this->applyPerspectiveTextureMipmapGradient(
                          pRegister, gradient, v0, v1, v2, rhws);
  }

  rasterID_.Flags.Perspective = 1;

  return pRegister;
}

Register *CRasterizer::applyAffineTextureMipmapGradient(Register *pRegister) {
  auto rasterFlags = rasterID_.Flags;
  auto numTextures = rasterFlags.NumTextures;
  auto textureMips = rasterFlags.TextureMips;
  auto pCur = pRegister - (2 * numTextures);

  for (uint32_t j = 0; j < numTextures; ++j, pCur += 2) {
    assert(j < MAX_TEXTURES);

    if ((textureMips >> j) & 0x1) {
      float20 fMaxDm[2];

      for (uint32_t i = 0; i < 2; ++i) {
        fMaxDm[i] = static_cast<float20>(
            Math::TMax(Math::TAbs(pCur[i].m[0]), Math::TAbs(pCur[i].m[1])));
      }

      auto &sampler = rasterData_.Samplers[j];
      auto logWidth = sampler.pMipLevels[0].getLogWidth();
      auto logHeight = sampler.pMipLevels[0].getLogHeight();
      auto fMU = Math::TMul<floatf>(fMaxDm[0], 1 << logWidth);
      auto fMV = Math::TMul<floatf>(fMaxDm[1], 1 << logHeight);
      auto fM = Math::TMax(fMU, fMV);

      if (fM > fONE) {
        if (FILTER_NONE == rasterID_.Textures[j].MipFilter) {
          // Turn off trilinear filtering
          rasterID_.Textures[j].MipFilter = FILTER_NONE;
          rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
          rasterID_.Flags.TextureMips &= ~(1 << j);
        } else {
          auto fJ = static_cast<fixed16>(fM);
          uint32_t mipLevel = Math::iLog2(fJ.data()) - fixed16::FRAC;
          if (mipLevel >= sampler.MaxMipLevel) {
            // Use nearest mipmap filtering
            rasterID_.Textures[j].MipFilter = FILTER_NEAREST;
            rasterID_.Textures[j].MagFilter = rasterID_.Textures[j].MinFilter;
            fM = static_cast<floatf>(1 << sampler.MaxMipLevel);
          }

          pRegister->m[2] = fixedRX::make(static_cast<fixed16>(fM).data());
          pRegister->m[0] = TConst<fixedRX>::Zero();
          pRegister->m[1] = TConst<fixedRX>::Zero();
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

Register *CRasterizer::applyPerspectiveTextureMipmapGradient(
    Register *pRegister, const TriangleGradient &gradient, const RDVECTOR &v0,
    const RDVECTOR &v1, const RDVECTOR &v2, floatRW rhws[3]) {
  assert(pRegister);

  auto rasterFlags = rasterID_.Flags;
  auto textureMips = rasterFlags.TextureMips;
  auto numTextures = rasterFlags.NumTextures;

  floatf fRhw2s[3];

  for (uint32_t k = 0; k < 3; ++k) {
    fRhw2s[k] = Math::TMul<floatf>(rhws[k], rhws[k]);
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

        auto fK1 = Math::TMulSub<float24>(fA, fRhwdB, fB, fRhwdA);
        auto fK2 = Math::TMulSub<float20>(fA, fRhwdC, fRhwdA, fC);
        auto fK3 = Math::TMulSub<float20>(fB, fRhwdC, fRhwdB, fC);

        for (uint32_t k = 0; k < 3; ++k) {
          auto fK1x = Math::TMul<float20>(fK1, vScreenPos[k]->x);
          auto fK1y = Math::TMul<float20>(fK1, vScreenPos[k]->y);
          fMaxDms[k][i] = Math::TMax(Math::TAbs(fK2 + fK1y), Math::TAbs(fK3 - fK1x));
        }
      }

      auto &sampler = rasterData_.Samplers[j];
      auto logWidth = sampler.pMipLevels[0].getLogWidth();
      auto logHeight = sampler.pMipLevels[0].getLogHeight();

      floatf fMs[3];

      for (uint32_t k = 0; k < 3; ++k) {
        auto fMU = Math::TShiftLeft<floatf>(fMaxDms[k][0], logWidth);
        auto fMV = Math::TShiftLeft<floatf>(fMaxDms[k][1], logHeight);
        fMs[k] = Math::TMax(fMU, fMV);
      }

      if ((fMs[0] > fRhw2s[0]) || (fMs[1] > fRhw2s[1]) ||
          (fMs[2] > fRhw2s[2])) {
        if ((Math::TShiftRight<floatf>(fMs[0], sampler.MaxMipLevel) < fRhw2s[0]) ||
            (Math::TShiftRight<floatf>(fMs[1], sampler.MaxMipLevel) < fRhw2s[1]) ||
            (Math::TShiftRight<floatf>(fMs[2], sampler.MaxMipLevel) < fRhw2s[2])) {
          floatf delta[2] = {fMs[1] - fMs[0], fMs[2] - fMs[0]};

          if ((fixed16(Math::TAbs(delta[0])) > TConst<fixed16>::Epsilon()) ||
              (fixed16(Math::TAbs(delta[1])) > TConst<fixed16>::Epsilon())) {
            rasterID_.Flags.InterpolateMips |= (1 << j);
            pRegister->m[2] = fixedRX::make(static_cast<fixed16>(fMs[0]).data());
            pRegister->m[0] = fixedRX::make(gradient.calcDeltaX<fixed16>(delta[0], delta[1]).data());
            pRegister->m[1] = fixedRX::make(gradient.calcDeltaY<fixed16>(delta[0], delta[1]).data());
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
                pRegister->m[0] = TConst<fixedRX>::Zero();
                pRegister->m[1] = TConst<fixedRX>::Zero();
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
            pRegister->m[0] = TConst<fixedRX>::Zero();
            pRegister->m[1] = TConst<fixedRX>::Zero();
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

Register *CRasterizer::applyFogGradient(Register *pRegister,
                                   const TriangleGradient &gradient,
                                   uint32_t i0, uint32_t i1, uint32_t i2) {
  assert(pRegister);

  auto pfFogs = reinterpret_cast<float20 *>(pbVertexData_[VERTEXDATA_FOG]);
  auto fFog0 = pfFogs[i0];
  auto fFog1 = pfFogs[i1];
  auto fFog2 = pfFogs[i2];

  float20 delta[2] = {fFog1 - fFog0, fFog2 - fFog0};

  if ((fixed8(Math::TAbs(delta[0])) > TConst<fixed8>::Epsilon()) ||
      (fixed8(Math::TAbs(delta[1])) > TConst<fixed8>::Epsilon())) {
    pRegister->m[0] = gradient.calcDeltaX<fixedRX>(delta[0], delta[1]);
    pRegister->m[1] = gradient.calcDeltaY<fixedRX>(delta[0], delta[1]);
    rasterID_.Flags.InterpolateFog = 1;
  } else {
    pRegister->m[0] = TConst<fixedRX>::Zero();
    pRegister->m[1] = TConst<fixedRX>::Zero();
  }

  pRegister->m[2] = static_cast<fixedRX>(fFog0);

  return pRegister + 1;
}