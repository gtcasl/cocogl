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
#include "tnl.hpp"
#include "tnl.inl"

static const PfnDecodePosition g_decodePosition[] = {
    DecodePosition<VERTEX_BYTE2>,
    DecodePosition<VERTEX_BYTE3>,
    DecodePosition<VERTEX_BYTE4>,
    DecodePosition<VERTEX_SHORT2>,
    DecodePosition<VERTEX_SHORT3>,
    DecodePosition<VERTEX_SHORT4>,
    DecodePosition<VERTEX_FIXED2>,
    DecodePosition<VERTEX_FIXED3>,
    DecodePosition<VERTEX_FIXED4>,
    DecodePosition<VERTEX_FLOAT2>,
    DecodePosition<VERTEX_FLOAT3>,
    DecodePosition<VERTEX_FLOAT4>,
};

static const TNL::PfnDecodeVertices g_processLighting[] = {
    &TNL::processLightingOneSided<false, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &TNL::processLightingOneSided<false, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &TNL::processLightingOneSided<false, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &TNL::processLightingOneSided<false, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &TNL::processLightingOneSided<false, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &TNL::processLightingOneSided<true, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &TNL::processLightingOneSided<true, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &TNL::processLightingOneSided<true, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &TNL::processLightingOneSided<true, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &TNL::processLightingOneSided<true, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &TNL::processLightingOneSided<true, VERTEX_FIXED4, VERTEX_UNKNOWN>,
    &TNL::processLightingOneSided<true, VERTEX_FIXED4, VERTEX_BYTE3>,
    &TNL::processLightingOneSided<true, VERTEX_FIXED4, VERTEX_SHORT3>,
    &TNL::processLightingOneSided<true, VERTEX_FIXED4, VERTEX_FIXED3>,
    &TNL::processLightingOneSided<true, VERTEX_FIXED4, VERTEX_FLOAT3>,
    &TNL::processLightingOneSided<true, VERTEX_FLOAT4, VERTEX_UNKNOWN>,
    &TNL::processLightingOneSided<true, VERTEX_FLOAT4, VERTEX_BYTE3>,
    &TNL::processLightingOneSided<true, VERTEX_FLOAT4, VERTEX_SHORT3>,
    &TNL::processLightingOneSided<true, VERTEX_FLOAT4, VERTEX_FIXED3>,
    &TNL::processLightingOneSided<true, VERTEX_FLOAT4, VERTEX_FLOAT3>,
    &TNL::processLightingOneSided<true, VERTEX_RGBA, VERTEX_UNKNOWN>,
    &TNL::processLightingOneSided<true, VERTEX_RGBA, VERTEX_BYTE3>,
    &TNL::processLightingOneSided<true, VERTEX_RGBA, VERTEX_SHORT3>,
    &TNL::processLightingOneSided<true, VERTEX_RGBA, VERTEX_FIXED3>,
    &TNL::processLightingOneSided<true, VERTEX_RGBA, VERTEX_FLOAT3>,

    &TNL::processLightingTwoSided<false, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &TNL::processLightingTwoSided<false, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &TNL::processLightingTwoSided<false, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &TNL::processLightingTwoSided<false, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &TNL::processLightingTwoSided<false, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &TNL::processLightingTwoSided<true, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &TNL::processLightingTwoSided<true, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &TNL::processLightingTwoSided<true, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &TNL::processLightingTwoSided<true, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &TNL::processLightingTwoSided<true, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &TNL::processLightingTwoSided<true, VERTEX_FIXED4, VERTEX_UNKNOWN>,
    &TNL::processLightingTwoSided<true, VERTEX_FIXED4, VERTEX_BYTE3>,
    &TNL::processLightingTwoSided<true, VERTEX_FIXED4, VERTEX_SHORT3>,
    &TNL::processLightingTwoSided<true, VERTEX_FIXED4, VERTEX_FIXED3>,
    &TNL::processLightingTwoSided<true, VERTEX_FIXED4, VERTEX_FLOAT3>,
    &TNL::processLightingTwoSided<true, VERTEX_FLOAT4, VERTEX_UNKNOWN>,
    &TNL::processLightingTwoSided<true, VERTEX_FLOAT4, VERTEX_BYTE3>,
    &TNL::processLightingTwoSided<true, VERTEX_FLOAT4, VERTEX_SHORT3>,
    &TNL::processLightingTwoSided<true, VERTEX_FLOAT4, VERTEX_FIXED3>,
    &TNL::processLightingTwoSided<true, VERTEX_FLOAT4, VERTEX_FLOAT3>,
    &TNL::processLightingTwoSided<true, VERTEX_RGBA, VERTEX_UNKNOWN>,
    &TNL::processLightingTwoSided<true, VERTEX_RGBA, VERTEX_BYTE3>,
    &TNL::processLightingTwoSided<true, VERTEX_RGBA, VERTEX_SHORT3>,
    &TNL::processLightingTwoSided<true, VERTEX_RGBA, VERTEX_FIXED3>,
    &TNL::processLightingTwoSided<true, VERTEX_RGBA, VERTEX_FLOAT3>,
};

static const TNL::PfnDecodeVertices g_processVertexColor[] = {
    &TNL::processVertexColor<VERTEX_FIXED4>,
    &TNL::processVertexColor<VERTEX_FLOAT4>,
    &TNL::processVertexColor<VERTEX_RGBA>,
};

static const TNL::PfnDecodeTexCoord g_processTexcoords[] = {
    &TNL::processTexCoords<false, VERTEX_BYTE2>,
    &TNL::processTexCoords<false, VERTEX_BYTE3>,
    &TNL::processTexCoords<false, VERTEX_BYTE4>,
    &TNL::processTexCoords<false, VERTEX_SHORT2>,
    &TNL::processTexCoords<false, VERTEX_SHORT3>,
    &TNL::processTexCoords<false, VERTEX_SHORT4>,
    &TNL::processTexCoords<false, VERTEX_FIXED2>,
    &TNL::processTexCoords<false, VERTEX_FIXED3>,
    &TNL::processTexCoords<false, VERTEX_FIXED4>,
    &TNL::processTexCoords<false, VERTEX_FLOAT2>,
    &TNL::processTexCoords<false, VERTEX_FLOAT3>,
    &TNL::processTexCoords<false, VERTEX_FLOAT4>,

    &TNL::processTexCoords<true, VERTEX_BYTE2>,
    &TNL::processTexCoords<true, VERTEX_BYTE3>,
    &TNL::processTexCoords<true, VERTEX_BYTE4>,
    &TNL::processTexCoords<true, VERTEX_SHORT2>,
    &TNL::processTexCoords<true, VERTEX_SHORT3>,
    &TNL::processTexCoords<true, VERTEX_SHORT4>,
    &TNL::processTexCoords<true, VERTEX_FIXED2>,
    &TNL::processTexCoords<true, VERTEX_FIXED3>,
    &TNL::processTexCoords<true, VERTEX_FIXED4>,
    &TNL::processTexCoords<true, VERTEX_FLOAT2>,
    &TNL::processTexCoords<true, VERTEX_FLOAT3>,
    &TNL::processTexCoords<true, VERTEX_FLOAT4>,
};

static const TNL::PfnDecodeVertices g_processPointSize[] = {
    &TNL::processPointSize<false, VERTEX_FIXED>,
    &TNL::processPointSize<false, VERTEX_FLOAT>,
    &TNL::processPointSize<true, VERTEX_FIXED>,
    &TNL::processPointSize<true, VERTEX_FLOAT>,
};

static const TNL::PfnDecodeVertices g_processFog[] = {
    &TNL::processFog<FogLinear>,
    &TNL::processFog<FogExp>,
    &TNL::processFog<FogExp2>,
};

GLenum TNL::setupTNLStates(GLenum mode, int first, uint32_t count) {
  GLenum err;

  auto rasterFlags = rasterID_.Flags;

  TNLFlags_.Value = 0;

  if (vertexStates_.Position) {
    err = positionArray_.prepare(&positionDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    __glLogError("The vertex array input is disabled.\r\n");
    return GL_INVALID_OPERATION;
  }

  auto fmtType = (positionArray_.Format - VERTEX_BYTE2) / 4;
  auto fmtSize = positionArray_.Format - 1 - fmtType * 4;
  auto func = fmtType * 3 + fmtSize - 1;
  assert(func < __countof(g_decodePosition));
  pfnDecodePosition_ = g_decodePosition[func];

  auto numVertives = count + CLIP_BUFFER_SIZE;

  clipVerticesBaseIndex_ = count;

  uint8_t *pbVertexData = nullptr;

  pbVertexData_[VERTEX_FLAGS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(uint16_t), 4);

  pbVertexData_[VERTEX_WORLDPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(VECTOR4), 4);

  pbVertexData_[VERTEX_CLIPPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(VECTOR4), 4);

  pbVertexData_[VERTEX_SCREENPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(RDVECTOR), 4);

  if (dirtyFlags_.ModelViewProj) {
    this->updateModelViewProj();
  }

  if (dirtyFlags_.ScreenXform) {
    this->updateScreenXform();
  }

  if (caps_.ClipPlanes) {
    this->updateClipPlanes();
  }

  if (GL_POINTS == mode) {
    err = this->updatePoints(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("TNL::updatePoints() failed, err = %d.\r\n", err);
      return err;
    }
  }

  if (rasterFlags.Color) {
    err = this->updateColor(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("TNL::updateColor() failed, err = %d.\r\n", err);
      return err;
    }
  }

  if (rasterFlags.NumTextures) {
    err = this->updateTexcoords(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("TNL::updateTexcoords() failed, err = %d.\r\n", err);
      return err;
    }
  }

  if (rasterFlags.Fog) {
    this->updateFog(&pbVertexData, first, count);
  }

  auto buffSize = reinterpret_cast<uintptr_t>(pbVertexData);
  vertexBuffer_.resize(buffSize);

  // Update vertex buffer offsets
  {
    auto offset = reinterpret_cast<uintptr_t>(vertexBuffer_.data());
    for (uint32_t i = 0; i < VERTEX_SIZE; ++i) {
      pbVertexData_[i] += offset;
    }
    pbVertexColor_ += offset;
  }

  return GL_NO_ERROR;
}

void TNL::processVertices(uint32_t count) {
  auto flags = TNLFlags_;

  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEX_FLAGS]);
  auto pvWorldPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEX_WORLDPOS]);
  auto pvClipPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEX_CLIPPOS]);
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEX_SCREENPOS]);

  // Decode vertices
  (pfnDecodePosition_)(pvWorldPos, positionDecode_.pBits,
                       positionDecode_.Stride, count);

  uint32_t clipUnion = 0;

  for (uint32_t i = 0; i < count; ++i) {
    // Tranform vertex position to clip space
    Math::Mul(&pvClipPos[i], pvWorldPos[i], mModelViewProj_);

    // Compute frustum clipping flags
    auto clipFlags = this->CalcClipFlags(pvClipPos[i]);
    clipUnion |= clipFlags;
    pwFlags[i] = static_cast<uint16_t>(clipFlags);
  }

  if (flags.UserClipPlanes) {
    // Compute user clipping flags
    clipUnion |= this->calcUserClipFlags(count);
  }

  // Transform to screen space
  if (0 == clipUnion) {
    this->transformScreenSpace(pvScreenPos, pvClipPos, count);
  } else {
    for (uint32_t i = 0; i < count; ++i) {
      if (0 == pwFlags[i]) {
        this->transformScreenSpace(&pvScreenPos[i], &pvClipPos[i], 1);
      }
    }
  }

  if (flags.EyeSpace || flags.EyeSpaceZ) {
    this->transformEyeSpace(count);
  }

  if (flags.PointSize) {
    (this->*pfnPointSize_)(count);
  }

  if (flags.Color) {
    (this->*pfnColor_)(count);
  }

  if (flags.TexCoords) {
    for (uint32_t i = 0, j = 0, mask = caps_.Texture2D; mask; mask >>= 1, ++i) {
      if (mask & 0x1) {
        (this->*pfnTexCoords_[i])(j++, i, count);
      }
    }
  }

  if (flags.Fog) {
    (this->*pfnFog_)(count);
  }
}

uint32_t TNL::CalcClipFlags(const VECTOR4 &vPosition) {
  auto cx = vPosition.x;
  auto cy = vPosition.y;
  auto cz = vPosition.z;
  auto cw = vPosition.w;

  auto dl = cw + cx;
  auto dr = cw - cx;
  auto db = cw + cy;
  auto dt = cw - cy;
  auto df = cw + cz;
  auto dk = cw - cz;

  uint32_t clipFlags;
  clipFlags =  (dl < Math::Zero<floatf>()) << CLIP_LEFT;
  clipFlags |= (dr < Math::Zero<floatf>()) << CLIP_RIGHT;
  clipFlags |= (db < Math::Zero<floatf>()) << CLIP_BOTTOM;
  clipFlags |= (dt < Math::Zero<floatf>()) << CLIP_TOP;
  clipFlags |= (df < Math::Zero<floatf>()) << CLIP_FRONT;
  clipFlags |= (dk < Math::Zero<floatf>()) << CLIP_BACK;

  return clipFlags;
}

uint32_t TNL::calcUserClipFlags(uint32_t count) {
  uint32_t clipUnion = 0;

  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEX_FLAGS]);
  auto pvClipPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEX_CLIPPOS]);

  for (uint32_t i = 0; i < count; ++i) {
    uint32_t clipFlags = 0;
    auto &vClipPos = pvClipPos[i];

    for (uint32_t j = 0, m = caps_.ClipPlanes; m; ++j, m >>= 1) {
      if (m & 1) {
        assert(j < vClipPlanesCS_.size());
        auto fDist = Math::Dot<floatf>(vClipPos, vClipPlanesCS_[j]);
        clipFlags |= (fDist < Math::Zero<floatf>()) << (CLIP_PLANE0 + j);
      }
    }

    pwFlags[i] |= clipFlags;
    clipUnion |= clipFlags;
  }

  return clipUnion;
}

void TNL::transformScreenSpace(RDVECTOR *pRDVertex, 
                               const VECTOR4 *pvClipPos,
                               uint32_t count) {
  assert(pRDVertex);
  assert(pvClipPos);

  auto fScaleX = screenXform_.fScaleX;
  auto fScaleY = screenXform_.fScaleY;
  auto fScaleZ = screenXform_.fScaleZ;

  auto fMinX = screenXform_.fMinX;
  auto fMinY = screenXform_.fMinY;
  auto fMinZ = screenXform_.fMinZ;

  for (uint32_t i = 0; i < count; ++i) {
    if (!Math::IsAlmostZero(pvClipPos[i].w - Math::One<floatf>())) {
      // Perspective screen space transform
      auto fRhw = Math::Inverse<floatRW>(pvClipPos[i].w);
      pRDVertex[i].x = fMinX + Math::MulRnd<fixed4>(pvClipPos[i].x, fRhw, fScaleX);
      pRDVertex[i].y = fMinY + Math::MulRnd<fixed4>(pvClipPos[i].y, fRhw, fScaleY);
      pRDVertex[i].z = fMinZ + Math::MulRnd<float20>(pvClipPos[i].z, fRhw, fScaleZ);
      pRDVertex[i].rhw = fRhw;
    } else {
      // Affine screen space transform
      pRDVertex[i].x = fMinX + Math::MulRnd<fixed4>(pvClipPos[i].x, fScaleX);
      pRDVertex[i].y = fMinY + Math::MulRnd<fixed4>(pvClipPos[i].y, fScaleY);
      pRDVertex[i].z = fMinZ + Math::MulRnd<float20>(pvClipPos[i].z, fScaleZ);
      pRDVertex[i].rhw = Math::One<floatRW>();
    }
  }
}

void TNL::transformEyeSpace(uint32_t count) {
  auto &matEyeXform = pMsModelView_->getMatrix();
  auto pvWorldPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEX_WORLDPOS]);
  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEX_EYEPOS]);

  if (TNLFlags_.EyeSpace) {
    for (uint32_t i = 0; i < count; ++i) {
      Math::Mul(&pvEyePos[i], pvWorldPos[i], matEyeXform);
    }
  } else {
    for (uint32_t i = 0; i < count; ++i) {
      pvEyePos[i].z = Math::MulAdd(
          pvWorldPos[i].x, matEyeXform._31, 
          pvWorldPos[i].y, matEyeXform._32,
          pvWorldPos[i].z, matEyeXform._33, 
          pvWorldPos[i].w, matEyeXform._34);
    }
  }
}

void TNL::processPointSize(uint32_t count) {
  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEX_EYEPOS]);
  auto pfPointSizes = reinterpret_cast<fixed4 *>(pbVertexData_[VERTEX_POINTSIZE]);
  auto &vAttenuation = pointParams_.vAttenuation;

  auto fPointSize = fPointSize_;

  if (!TNLFlags_.PointSizeQAttn) {
    if (!Math::IsAlmostZero(vAttenuation.x - Math::One<floatf>())) {
      fPointSize *= Math::RSqrt(vAttenuation.x);
    } else {
      fPointSize *= vAttenuation.x;
    }
  }

  for (uint32_t i = 0; i < count; ++i) {
    if (TNLFlags_.PointSizeQAttn) {
      auto fEyeDist = std::abs(pvEyePos[i].z);
      auto fInvAtt = vAttenuation.z * fEyeDist * fEyeDist +
                     vAttenuation.y * fEyeDist + vAttenuation.x;
      if (!Math::IsAlmostZero(fInvAtt - Math::One<floatf>())) {
        fPointSize *= Math::RSqrt(fInvAtt);
      }
    }

    pfPointSizes[i] = std::max(static_cast<fixed4>(fPointSize), Math::One<fixed4>());
  }
}

void TNL::processColor(uint32_t count) {
  auto pcFrontColors =
      reinterpret_cast<ColorARGB *>(pbVertexData_[VERTEX_FRONTCOLOR]);

  // Clamp the color
  ColorARGB cColor(Math::ToUNORM8(Math::Sat(vColor_.w)),
                   Math::ToUNORM8(Math::Sat(vColor_.x)),
                   Math::ToUNORM8(Math::Sat(vColor_.y)),
                   Math::ToUNORM8(Math::Sat(vColor_.z)));

  for (uint32_t i = 0; i < count; ++i) {
    pcFrontColors[i] = cColor;
  }
}

void TNL::processLightsOneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                const VECTOR3 &vNormal,
                                const VECTOR4 &vVertexColor) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;                      // Light vector
    auto fAtt = Math::One<floatf>(); // Attenuation distance

    if (pLight->Flags.DirectionalLight) {
      // Get the light direction
      vL.x = pLight->vPosition.x;
      vL.y = pLight->vPosition.y;
      vL.z = pLight->vPosition.z;
    } else {
      // Compute the light position
      vL.x = pLight->vPosition.x - vEyePos.x;
      vL.y = pLight->vPosition.y - vEyePos.y;
      vL.z = pLight->vPosition.z - vEyePos.z;

      // Normalize the light position
      auto fDistSq = Math::Dot<floatf>(vL, vL);
      if (!Math::IsAlmostZero(fDistSq - Math::One<floatf>())) {
        auto fInvDist = Math::RSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = std::sqrt(fDistSq);
        auto fInvAtt =
            pLight->getAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->getAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->getAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::IsAlmostZero(fInvAtt - Math::One<floatf>())) {
          fAtt = Math::Inverse<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::Dot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= std::pow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vFrontDS = pLight->vScaledAmbient;

    auto fDotNL = Math::Dot<floatf>(vNormal, vL);
    if (fDotNL > Math::Zero<floatf>()) {
      auto &vLightDiffuse = pLight->getColor(GL_DIFFUSE);
      vFrontDS.x += vVertexColor.x * vLightDiffuse.x * fDotNL;
      vFrontDS.y += vVertexColor.y * vLightDiffuse.y * fDotNL;
      vFrontDS.z += vVertexColor.z * vLightDiffuse.z * fDotNL;

      VECTOR3 vH;

      // Compute the Halfway vector
      if (pLight->Flags.DirectionalLight) {
        vH = pLight->vHalfway;
      } else {
        VECTOR3 vDir(Math::Zero<floatf>(), Math::Zero<floatf>(), Math::One<floatf>());
        Math::Add(&vH, vL, vDir);
        Math::Normalize(&vH);
      }

      auto fDotNH = Math::Dot<floatf>(vNormal, vH);
      if (fDotNH > Math::Zero<floatf>()) {
        // Compute the specular coefficient
        auto fCoeff = std::pow(fDotNH, fMatShininess_);
        if (!Math::IsAlmostZero(fCoeff)) {
          vFrontDS.x += pLight->vScaledSpecular.x * fCoeff;
          vFrontDS.y += pLight->vScaledSpecular.y * fCoeff;
          vFrontDS.z += pLight->vScaledSpecular.z * fCoeff;
        }
      }
    }

    pvOut->x += vFrontDS.x * fAtt;
    pvOut->y += vFrontDS.y * fAtt;
    pvOut->z += vFrontDS.z * fAtt;
  }
}

void TNL::processLightsOneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                const VECTOR3 &vNormal) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;                      // Light vector
    auto fAtt = Math::One<floatf>(); // Attenuation distance

    if (pLight->Flags.DirectionalLight) {
      // Get the light direction
      vL.x = pLight->vPosition.x;
      vL.y = pLight->vPosition.y;
      vL.z = pLight->vPosition.z;
    } else {
      // Compute the light position
      vL.x = pLight->vPosition.x - vEyePos.x;
      vL.y = pLight->vPosition.y - vEyePos.y;
      vL.z = pLight->vPosition.z - vEyePos.z;

      // Normalize the light position
      auto fDistSq = Math::Dot<floatf>(vL, vL);
      if (!Math::IsAlmostZero(fDistSq - Math::One<floatf>())) {
        auto fInvDist = Math::RSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = std::sqrt(fDistSq);
        auto fInvAtt =
            pLight->getAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->getAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->getAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::IsAlmostZero(fInvAtt - Math::One<floatf>())) {
          fAtt = Math::Inverse<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::Dot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= std::pow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    auto vFrontDS = pLight->vScaledAmbient;

    auto fDotNL = Math::Dot<floatf>(vNormal, vL);
    if (fDotNL > Math::Zero<floatf>()) {
      vFrontDS.x += pLight->vScaledDiffuse.x * fDotNL;
      vFrontDS.y += pLight->vScaledDiffuse.y * fDotNL;
      vFrontDS.z += pLight->vScaledDiffuse.z * fDotNL;

      VECTOR3 vH;

      // Compute the Halfway vector
      if (pLight->Flags.DirectionalLight) {
        vH = pLight->vHalfway;
      } else {
        VECTOR3 vDir(Math::Zero<floatf>(), Math::Zero<floatf>(), Math::One<floatf>());
        Math::Add(&vH, vL, vDir);
        Math::Normalize(&vH);
      }

      auto fDotNH = Math::Dot<floatf>(vNormal, vH);
      if (fDotNH > Math::Zero<floatf>()) {
        // Compute the specular coefficient
        auto fCoeff = std::pow(fDotNH, fMatShininess_);
        if (!Math::IsAlmostZero(fCoeff)) {
          vFrontDS.x += pLight->vScaledSpecular.x * fCoeff;
          vFrontDS.y += pLight->vScaledSpecular.y * fCoeff;
          vFrontDS.z += pLight->vScaledSpecular.z * fCoeff;
        }
      }
    }

    pvOut->x += vFrontDS.x * fAtt;
    pvOut->y += vFrontDS.y * fAtt;
    pvOut->z += vFrontDS.z * fAtt;
  }
}

void TNL::processLightsTwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                const VECTOR3 &vNormal,
                                const VECTOR4 &vVertexColor) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;                      // Light vector
    VECTOR3 vH;                      // Halfway vector
    auto fAtt = Math::One<floatf>(); // Attenuation distance

    if (pLight->Flags.DirectionalLight) {
      // Get the light direction
      vL.x = pLight->vPosition.x;
      vL.y = pLight->vPosition.y;
      vL.z = pLight->vPosition.z;

      // Get the halfway vector
      vH = pLight->vHalfway;
    } else {
      // Compute the light position
      vL.x = pLight->vPosition.x - vEyePos.x;
      vL.y = pLight->vPosition.y - vEyePos.y;
      vL.z = pLight->vPosition.z - vEyePos.z;

      // Normalize the light position
      auto fDistSq = Math::Dot<floatf>(vL, vL);
      if (!Math::IsAlmostZero(fDistSq - Math::One<floatf>())) {
        auto fInvDist = Math::RSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the halfway vector
      VECTOR3 vDir(Math::Zero<floatf>(), Math::Zero<floatf>(), Math::One<floatf>());
      Math::Add(&vH, vL, vDir);
      Math::Normalize(&vH);

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = std::sqrt(fDistSq);
        auto fInvAtt =
            pLight->getAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->getAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->getAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::IsAlmostZero(fInvAtt - Math::One<floatf>())) {
          fAtt = Math::Inverse<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::Dot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= std::pow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vColor;
    auto vFrontDS = pLight->vScaledAmbient;
    auto vBackDS = vFrontDS;

    auto fDotNL = Math::Dot<floatf>(vNormal, vL);
    auto fDiffFactor = (fDotNL > Math::Zero<floatf>()) ? fDotNL : -fDotNL;

    auto &vLightDiffuse = pLight->getColor(GL_DIFFUSE);
    vColor.x = vVertexColor.x * vLightDiffuse.x * fDiffFactor;
    vColor.y = vVertexColor.y * vLightDiffuse.y * fDiffFactor;
    vColor.z = vVertexColor.z * vLightDiffuse.z * fDiffFactor;

    auto fDotNH = Math::Dot<floatf>(vNormal, vH);
    if (fDotNL <= Math::Zero<floatf>()) {
      fDotNH = -fDotNH;
    }

    if (fDotNH > Math::Zero<floatf>()) {
      // Compute the specular coefficient
      auto fCoeff = std::pow(fDotNH, fMatShininess_);
      if (!Math::IsAlmostZero(fCoeff)) {
        vColor.x += pLight->vScaledSpecular.x * fCoeff;
        vColor.y += pLight->vScaledSpecular.y * fCoeff;
        vColor.z += pLight->vScaledSpecular.z * fCoeff;
      }
    }

    if (fDotNL > Math::Zero<floatf>()) {
      vFrontDS.x += vColor.x;
      vFrontDS.y += vColor.y;
      vFrontDS.z += vColor.z;
    } else {
      vBackDS.x += vColor.x;
      vBackDS.y += vColor.y;
      vBackDS.z += vColor.z;
    }

    pvOut[0].x += vFrontDS.x * fAtt;
    pvOut[0].y += vFrontDS.y * fAtt;
    pvOut[0].z += vFrontDS.z * fAtt;

    pvOut[1].x += vBackDS.x * fAtt;
    pvOut[1].y += vBackDS.y * fAtt;
    pvOut[1].z += vBackDS.z * fAtt;
  }
}

void TNL::processLightsTwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                const VECTOR3 &vNormal) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;                      // Light vector
    VECTOR3 vH;                      // Halfway vector
    auto fAtt = Math::One<floatf>(); // Attenuation distance

    if (pLight->Flags.DirectionalLight) {
      // Get the light direction
      vL.x = pLight->vPosition.x;
      vL.y = pLight->vPosition.y;
      vL.z = pLight->vPosition.z;

      // Get the halfway vector
      vH = pLight->vHalfway;
    } else {
      // Compute the light position
      vL.x = pLight->vPosition.x - vEyePos.x;
      vL.y = pLight->vPosition.y - vEyePos.y;
      vL.z = pLight->vPosition.z - vEyePos.z;

      // Normalize the light position
      auto fDistSq = Math::Dot<floatf>(vL, vL);
      if (!Math::IsAlmostZero(fDistSq - Math::One<floatf>())) {
        auto fInvDist = Math::RSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the halfway vector
      VECTOR3 vDir(Math::Zero<floatf>(), Math::Zero<floatf>(), Math::One<floatf>());
      Math::Add(&vH, vL, vDir);
      Math::Normalize(&vH);

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = std::sqrt(fDistSq);
        auto fInvAtt =
            pLight->getAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->getAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->getAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::IsAlmostZero(fInvAtt - Math::One<floatf>())) {
          fAtt = Math::Inverse<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::Dot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= std::pow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vColor;
    auto vFrontDS = pLight->vScaledAmbient;
    auto vBackDS = vFrontDS;

    auto fDotNL = Math::Dot<floatf>(vNormal, vL);
    auto fDiffFactor = (fDotNL > Math::Zero<floatf>()) ? fDotNL : -fDotNL;

    vColor.x = pLight->vScaledDiffuse.x * fDiffFactor;
    vColor.y = pLight->vScaledDiffuse.y * fDiffFactor;
    vColor.z = pLight->vScaledDiffuse.z * fDiffFactor;

    auto fDotNH = Math::Dot<floatf>(vNormal, vH);
    if (fDotNL <= Math::Zero<floatf>()) {
      fDotNH = -fDotNH;
    }

    if (fDotNH > Math::Zero<floatf>()) {
      // Compute the specular coefficient
      auto fCoeff = std::pow(fDotNH, fMatShininess_);
      if (!Math::IsAlmostZero(fCoeff)) {
        vColor.x += pLight->vScaledSpecular.x * fCoeff;
        vColor.y += pLight->vScaledSpecular.y * fCoeff;
        vColor.z += pLight->vScaledSpecular.z * fCoeff;
      }
    }

    if (fDotNL > Math::Zero<floatf>()) {
      vFrontDS.x += vColor.x;
      vFrontDS.y += vColor.y;
      vFrontDS.z += vColor.z;
    } else {
      vBackDS.x += vColor.x;
      vBackDS.y += vColor.y;
      vBackDS.z += vColor.z;
    }

    pvOut[0].x += vFrontDS.x * fAtt;
    pvOut[0].y += vFrontDS.y * fAtt;
    pvOut[0].z += vFrontDS.z * fAtt;

    pvOut[1].x += vBackDS.x * fAtt;
    pvOut[1].y += vBackDS.y * fAtt;
    pvOut[1].z += vBackDS.z * fAtt;
  }
}

void TNL::processTexCoords(uint32_t dstIndex, uint32_t srcIndex,
                           uint32_t count) {
  auto pvTexCoords = reinterpret_cast<TEXCOORD2 *>(
      pbVertexData_[VERTEX_TEXCOORD0 + dstIndex]);

  auto vIn = vTexCoords_[srcIndex];

  if (!pMsTexCoords_[srcIndex]->isIdentity()) {
    Math::Mul(&vIn, vIn, pMsTexCoords_[srcIndex]->getMatrix());
  }

  for (uint32_t k = 0; k < count; ++k) {
    pvTexCoords[k].m[0] = static_cast<float20>(vIn.x);
    pvTexCoords[k].m[1] = static_cast<float20>(vIn.y);
  }
}

GLenum TNL::updatePoints(uint8_t **ppbVertexData, int first, uint32_t count) {
  GLenum err;

  if (vertexStates_.PointSize) {
    err = pointSizeArray_.prepare(&pointSizeDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    pointSizeDecode_.pBits = nullptr;
  }

  if (!Math::IsAlmostZero(pointParams_.vAttenuation.y) ||
      !Math::IsAlmostZero(pointParams_.vAttenuation.z)) {
    TNLFlags_.PointSizeQAttn = 1;
    TNLFlags_.EyeSpaceZ = 1;
  }

  pbVertexData_[VERTEX_POINTSIZE] = *ppbVertexData;
  *ppbVertexData = __alignPtr(*ppbVertexData + 
                        (count + CLIP_BUFFER_SIZE) * sizeof(fixed4), 4);

  if (pointSizeDecode_.pBits) {
    auto func = (pointSizeArray_.Format - VERTEX_FIXED) / 4;

    if (TNLFlags_.PointSizeQAttn) {
      func += 2;
    }

    assert(func < __countof(g_processPointSize));
    pfnPointSize_ = g_processPointSize[func];
  } else {
    pfnPointSize_ = &TNL::processPointSize;
  }

  TNLFlags_.PointSize = 1;

  return GL_NO_ERROR;
}

GLenum TNL::updateColor(uint8_t **ppbVertexData, int first, uint32_t count) {
  GLenum err;

  if (vertexStates_.Color) {
    err = colorArray_.prepare(&colorDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    colorDecode_.pBits = nullptr;
  }

  pbVertexData_[VERTEX_FRONTCOLOR] = *ppbVertexData;
  pbVertexColor_ = *ppbVertexData;
  *ppbVertexData = __alignPtr(*ppbVertexData + 
                      (count + CLIP_BUFFER_SIZE) * sizeof(ColorARGB), 4);

  cullStates_.bTwoSidedLighting = false;

  if (caps_.Lighting) {
    err = this->updateLighting(ppbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("TNL::updateLighting() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    if (colorDecode_.pBits) {
      auto func = (colorArray_.Format - VERTEX_FIXED4) / 4;
      assert(func < __countof(g_processVertexColor));
      pfnColor_ = g_processVertexColor[func];
    } else {
      pfnColor_ = &TNL::processColor;
    }
  }

  TNLFlags_.Color = 1;

  return GL_NO_ERROR;
}

GLenum TNL::updateLighting(uint8_t **ppbVertexData, int first, uint32_t count) {
  GLenum err;

  if (dirtyFlags_.ModelViewInvT33) {
    this->updateModelViewInvT33();
  }

  if (vertexStates_.Normal) {
    err = normalArray_.prepare(&normalDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    normalDecode_.pBits = nullptr;
  }

  if (normalDecode_.pBits) {
    TNLFlags_.Normalize = caps_.Normalize;
  } else {
    if (dirtyFlags_.NormalizeNormal) {
      this->updateNormal();
    }
  }

  if (dirtyFlags_.ScaledAmbient) {
    this->updateMaterial();
  }

  if (dirtyFlags_.Lights) {
    this->setupLights();
  }

  if (pActiveLights_) {
    TNLFlags_.EyeSpace = 1;

    if (dirtyLights_.Ambient 
     || dirtyLights_.Diffuse 
     || dirtyLights_.Specular) {
      this->updateLights();
    }
  }

  int func = 0;

  if (normalDecode_.pBits) {
    func = 1 + (normalArray_.Format - VERTEX_BYTE3) / 4;
  }

  if (caps_.ColorMaterial) {
    uint32_t _func = 1;

    if (colorDecode_.pBits) {
      _func += 1 + (normalArray_.Format - VERTEX_FIXED4) / 4;
    }

    func += _func * 5;
  }

  if (caps_.TwoSidedLighting) {
    cullStates_.bTwoSidedLighting = true;

    pbVertexData_[VERTEX_BACKCOLOR] = *ppbVertexData;
    *ppbVertexData = __alignPtr(*ppbVertexData + 
                        (count + CLIP_BUFFER_SIZE) * sizeof(ColorARGB), 4);

    func += 5 * 5;
  }

  assert(func < __countof(g_processLighting));
  pfnColor_ = g_processLighting[func];

  return GL_NO_ERROR;
}

GLenum TNL::updateTexcoords(uint8_t **ppbVertexData, int first,
                            uint32_t count) {
  GLenum err;

  for (uint32_t i = 0, j = 0, mask = caps_.Texture2D; mask; mask >>= 1, ++i) {
    if (mask & 0x1) {
      if ((vertexStates_.TexCoords >> i) & 0x1) {
        err = texCoordArrays_[i].prepare(&texCoordDecodes_[i], first, count);
        if (__glFailed(err)) {
          __glLogError("VertexArray::prepare() failed, err = %d.\r\n", err);
          return err;
        }
      } else {
        texCoordDecodes_[i].pBits = nullptr;
      }

      pbVertexData_[VERTEX_TEXCOORD0 + j++] = *ppbVertexData;
      *ppbVertexData = __alignPtr(*ppbVertexData + 
                          (count + CLIP_BUFFER_SIZE) * sizeof(TEXCOORD2), 4);

      if (texCoordDecodes_[i].pBits) {
        auto fmtType = (texCoordArrays_[i].Format - VERTEX_BYTE2) / 4;
        auto fmtSize = texCoordArrays_[i].Format - 1 - fmtType * 4;
        auto func = fmtType * 3 + fmtSize - 1;

        if (!pMsTexCoords_[i]->isIdentity()) {
          func += 12;
        }

        assert(func < __countof(g_processTexcoords));
        pfnTexCoords_[i] = g_processTexcoords[func];
      } else {
        pfnTexCoords_[i] = &TNL::processTexCoords;
      }
    }
  }

  TNLFlags_.TexCoords = caps_.Texture2D;

  return GL_NO_ERROR;
}

void TNL::updateFog(uint8_t **ppbVertexData, int /*first*/, uint32_t count) {
  TNLFlags_.Fog = 1;
  TNLFlags_.EyeSpaceZ = 1;

  if (dirtyFlags_.FogRatio) {
    auto fFogStart = fog_.getFactor(GL_FOG_START);
    auto fFogEnd = fog_.getFactor(GL_FOG_END);

    if (fFogStart != fFogEnd) {
      fog_.fRatio = Math::Inverse<fixedRF>(fFogEnd - fFogStart);
    } else {
      fog_.fRatio = Math::Zero<fixedRF>();
    }

    dirtyFlags_.FogRatio = 0;
  }

  pbVertexData_[VERTEX_FOG] = *ppbVertexData;
  *ppbVertexData = __alignPtr(*ppbVertexData + 
                      (count + CLIP_BUFFER_SIZE) * sizeof(float20), 4);

  int func = fog_.Mode;
  assert(func < __countof(g_processFog));
  pfnFog_ = g_processFog[func];
}

void TNL::updateModelViewInvT44() {
  MATRIX44 matTmp;
  Math::Inverse(&matTmp, pMsModelView_->getMatrix());
  Math::Transpose(&mModelViewInvT_, matTmp);
  dirtyFlags_.ModelViewInvT44 = 0;

  if (!caps_.RescaleNormal) {
    dirtyFlags_.ModelViewInvT33 = 0;
    dirtyFlags_.NormalizeNormal = 1;
  }
}

void TNL::updateModelViewProj() {
  Math::Mul(&mModelViewProj_, pMsProjection_->getMatrix(), pMsModelView_->getMatrix());
  dirtyFlags_.ModelViewProj = 0;
}

void TNL::updateScreenXform() {
  screenXform_.fMinX = static_cast<fixed4>((viewport_.left + viewport_.right) / 2);
  screenXform_.fScaleX = float4(viewport_.right - viewport_.left) / 2;

  screenXform_.fMinY = static_cast<fixed4>((viewport_.top + viewport_.bottom) / 2);
  screenXform_.fScaleY = float4(viewport_.bottom - viewport_.top) / 2;

  screenXform_.fMinZ = static_cast<float20>((depthRange_.fNear + depthRange_.fFar) / 2);
  screenXform_.fScaleZ = (depthRange_.fFar - depthRange_.fNear) / 2;

  dirtyFlags_.ScreenXform = 0;
}

void TNL::updateClipPlanes() {
  TNLFlags_.UserClipPlanes = 1;

  if (dirtyFlags_.ClipPlanesCS) {
    if (dirtyFlags_.ProjectionInvT) {
      this->updateProjectionInvT();
    }

    auto updateMask = dirtyFlags_.ClipPlanesCS & caps_.ClipPlanes;
    for (uint32_t i = 0; updateMask; ++i, updateMask >>= 1) {
      if (updateMask & 1) {
        assert(i < vClipPlanesES_.size());
        assert(i < vClipPlanesCS_.size());
        Math::Mul(&vClipPlanesCS_[i], vClipPlanesES_[i], mProjectionInvT_);
      }
    }

    dirtyFlags_.ClipPlanesCS = 0;
  }
}

void TNL::updateProjectionInvT() {
  MATRIX44 matTmp;
  Math::Inverse(&matTmp, pMsProjection_->getMatrix());
  Math::Transpose(&mProjectionInvT_, matTmp);

  dirtyFlags_.ProjectionInvT = 0;
}

void TNL::updateModelViewInvT33() {
  MATRIX44 matTmp;
  Math::Inverse33(&matTmp, pMsModelView_->getMatrix());

  if (caps_.RescaleNormal) {
    auto fSum = (matTmp._31 * matTmp._31) + 
                (matTmp._32 * matTmp._32) +
                (matTmp._33 * matTmp._33);

    if (!Math::IsAlmostZero(fSum - Math::One<floatf>())) {
      auto fFactor = Math::RSqrt(fSum);

      mModelViewInvT_._11 = matTmp._11 * fFactor;
      mModelViewInvT_._21 = matTmp._21 * fFactor;
      mModelViewInvT_._31 = matTmp._31 * fFactor;

      mModelViewInvT_._12 = matTmp._12 * fFactor;
      mModelViewInvT_._22 = matTmp._22 * fFactor;
      mModelViewInvT_._32 = matTmp._32 * fFactor;

      mModelViewInvT_._13 = matTmp._13 * fFactor;
      mModelViewInvT_._23 = matTmp._23 * fFactor;
      mModelViewInvT_._33 = matTmp._33 * fFactor;
    }
  } else {
    mModelViewInvT_ = matTmp;
  }

  dirtyFlags_.ModelViewInvT33 = 0;
  dirtyFlags_.NormalizeNormal = 1;
}

void TNL::updateNormal() {
  // Transform the normal to world space
  Math::Mul(&vNormNormal_, vNormal_, mModelViewInvT_);

  // Normalize the normal
  Math::Normalize(&vNormNormal_);

  dirtyFlags_.NormalizeNormal = 0;
}

void TNL::updateMaterial() {
  auto &vMaterialAmbient = material_.getColor(GL_AMBIENT);
  auto &vMaterialDiffuse = material_.getColor(GL_DIFFUSE);

  vScaledAmbient_.x = vMaterialAmbient.x * vLightModelAmbient_.x;
  vScaledAmbient_.y = vMaterialAmbient.y * vLightModelAmbient_.y;
  vScaledAmbient_.z = vMaterialAmbient.z * vLightModelAmbient_.z;
  vScaledAmbient_.w = vMaterialDiffuse.w;

  dirtyFlags_.ScaledAmbient = 0;
}

void TNL::setupLights() {
  pActiveLights_ = nullptr;
  Light *pLight = nullptr;

  for (uint32_t i = 0, activeMask = caps_.Lights; activeMask;
       ++i, activeMask >>= 1) {
    if (activeMask & 1) {
      if (nullptr == pLight) {
        pActiveLights_ = pLight = &lights_[i];
      } else {
        pLight->pNext = &lights_[i];
        pLight = pLight->pNext;
      }

      pLight->pNext = nullptr;
    }
  }

  dirtyFlags_.Lights = 0;
}

void TNL::updateLights() {
  auto &vMaterialAmbient = material_.getColor(GL_AMBIENT);
  auto &vMaterialDiffuse = material_.getColor(GL_DIFFUSE);
  auto &vMaterialSpecular = material_.getColor(GL_SPECULAR);

  auto dirtyLights = dirtyLights_;

  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    uint32_t mask = 1 << (pLight - lights_.data());

    if (dirtyLights.Ambient & mask) {
      auto &vLightAmbient = pLight->getColor(GL_AMBIENT);
      pLight->vScaledAmbient.x = vLightAmbient.x * vMaterialAmbient.x;
      pLight->vScaledAmbient.y = vLightAmbient.y * vMaterialAmbient.y;
      pLight->vScaledAmbient.z = vLightAmbient.z * vMaterialAmbient.z;
    }

    if (dirtyLights.Diffuse & mask) {
      auto &vLightDiffuse = pLight->getColor(GL_DIFFUSE);
      pLight->vScaledDiffuse.x = vMaterialDiffuse.x * vLightDiffuse.x;
      pLight->vScaledDiffuse.y = vMaterialDiffuse.y * vLightDiffuse.y;
      pLight->vScaledDiffuse.z = vMaterialDiffuse.z * vLightDiffuse.z;
    }

    if (dirtyLights.Specular & mask) {
      auto &vLightSpecular = pLight->getColor(GL_SPECULAR);
      pLight->vScaledSpecular.x = vMaterialSpecular.x * vLightSpecular.x;
      pLight->vScaledSpecular.y = vMaterialSpecular.y * vLightSpecular.y;
      pLight->vScaledSpecular.z = vMaterialSpecular.z * vLightSpecular.z;
    }

    if (dirtyLights.Position & mask) {
      if (pLight->Flags.DirectionalLight) {
        // Normalize the position
        Math::Normalize(reinterpret_cast<VECTOR3 *>(&pLight->vPosition));

        // Compute the halfway vector
        VECTOR3 vDir(Math::Zero<floatf>(), Math::Zero<floatf>(), Math::One<floatf>());
        Math::Add(&pLight->vHalfway, reinterpret_cast<const VECTOR3 &>(pLight->vPosition), vDir);

        // Normalize the halfway vector
        Math::Normalize(&pLight->vHalfway);
      } else {
        if (Math::IsAlmostZero(pLight->getAttenuation(GL_CONSTANT_ATTENUATION) - Math::One<floatf>())
         && Math::IsAlmostZero(pLight->getAttenuation(GL_LINEAR_ATTENUATION)) 
         && Math::IsAlmostZero(pLight->getAttenuation(GL_QUADRATIC_ATTENUATION))) {
          pLight->Flags.Attenuation = 0;
        } else {
          pLight->Flags.Attenuation = 1;
        }

        if (Math::IsAlmostZero(pLight->fSpotCutOff - Math::F180<floatf>())) {
          pLight->Flags.SpotLight = 0;
        } else {
          pLight->Flags.SpotLight = 1;
        }
      }
    }
  }

  dirtyLights_.Value = 0;
}

void TNL::updateMatrixDirtyFlags() {
  if (pMatrixStack_ == pMsModelView_) {
    dirtyFlags_.ModelViewInvT44 = 1;
    dirtyFlags_.ModelViewInvT33 = 1;
    dirtyFlags_.ModelViewProj = 1;
  } else if (pMatrixStack_ == pMsProjection_) {
    dirtyFlags_.ModelViewProj = 1;
    dirtyFlags_.ProjectionInvT = 1;
    dirtyFlags_.ClipPlanesCS = CLIPPLANES_MASK;
  }
}
