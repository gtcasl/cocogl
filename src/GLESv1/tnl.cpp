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
    TDecodePosition<VERTEX_BYTE2>,  TDecodePosition<VERTEX_BYTE3>,
    TDecodePosition<VERTEX_BYTE4>,  TDecodePosition<VERTEX_SHORT2>,
    TDecodePosition<VERTEX_SHORT3>, TDecodePosition<VERTEX_SHORT4>,
    TDecodePosition<VERTEX_FIXED2>, TDecodePosition<VERTEX_FIXED3>,
    TDecodePosition<VERTEX_FIXED4>, TDecodePosition<VERTEX_FLOAT2>,
    TDecodePosition<VERTEX_FLOAT3>, TDecodePosition<VERTEX_FLOAT4>,
};

static const CTNL::PfnDecodeVertices g_processLighting[] = {
    &CTNL::TProcessLighting_OneSided<false, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_OneSided<false, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_OneSided<false, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_OneSided<false, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_OneSided<false, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FIXED4, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FIXED4, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FIXED4, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FIXED4, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FIXED4, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FLOAT4, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FLOAT4, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FLOAT4, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FLOAT4, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_FLOAT4, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_RGBA, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_RGBA, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_RGBA, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_RGBA, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_OneSided<true, VERTEX_RGBA, VERTEX_FLOAT3>,

    &CTNL::TProcessLighting_TwoSided<false, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_TwoSided<false, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_TwoSided<false, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_TwoSided<false, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_TwoSided<false, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_UNKNOWN, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_UNKNOWN, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_UNKNOWN, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_UNKNOWN, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_UNKNOWN, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FIXED4, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FIXED4, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FIXED4, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FIXED4, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FIXED4, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FLOAT4, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FLOAT4, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FLOAT4, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FLOAT4, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_FLOAT4, VERTEX_FLOAT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_RGBA, VERTEX_UNKNOWN>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_RGBA, VERTEX_BYTE3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_RGBA, VERTEX_SHORT3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_RGBA, VERTEX_FIXED3>,
    &CTNL::TProcessLighting_TwoSided<true, VERTEX_RGBA, VERTEX_FLOAT3>,
};

static const CTNL::PfnDecodeVertices g_processVertexColor[] = {
    &CTNL::TProcessVertexColor<VERTEX_FIXED4>,
    &CTNL::TProcessVertexColor<VERTEX_FLOAT4>,
    &CTNL::TProcessVertexColor<VERTEX_RGBA>,
};

static const CTNL::PfnDecodeTexCoord g_processTexcoords[] = {
    &CTNL::TProcessTexCoords<false, VERTEX_BYTE2>,
    &CTNL::TProcessTexCoords<false, VERTEX_BYTE3>,
    &CTNL::TProcessTexCoords<false, VERTEX_BYTE4>,
    &CTNL::TProcessTexCoords<false, VERTEX_SHORT2>,
    &CTNL::TProcessTexCoords<false, VERTEX_SHORT3>,
    &CTNL::TProcessTexCoords<false, VERTEX_SHORT4>,
    &CTNL::TProcessTexCoords<false, VERTEX_FIXED2>,
    &CTNL::TProcessTexCoords<false, VERTEX_FIXED3>,
    &CTNL::TProcessTexCoords<false, VERTEX_FIXED4>,
    &CTNL::TProcessTexCoords<false, VERTEX_FLOAT2>,
    &CTNL::TProcessTexCoords<false, VERTEX_FLOAT3>,
    &CTNL::TProcessTexCoords<false, VERTEX_FLOAT4>,

    &CTNL::TProcessTexCoords<true, VERTEX_BYTE2>,
    &CTNL::TProcessTexCoords<true, VERTEX_BYTE3>,
    &CTNL::TProcessTexCoords<true, VERTEX_BYTE4>,
    &CTNL::TProcessTexCoords<true, VERTEX_SHORT2>,
    &CTNL::TProcessTexCoords<true, VERTEX_SHORT3>,
    &CTNL::TProcessTexCoords<true, VERTEX_SHORT4>,
    &CTNL::TProcessTexCoords<true, VERTEX_FIXED2>,
    &CTNL::TProcessTexCoords<true, VERTEX_FIXED3>,
    &CTNL::TProcessTexCoords<true, VERTEX_FIXED4>,
    &CTNL::TProcessTexCoords<true, VERTEX_FLOAT2>,
    &CTNL::TProcessTexCoords<true, VERTEX_FLOAT3>,
    &CTNL::TProcessTexCoords<true, VERTEX_FLOAT4>,
};

static const CTNL::PfnDecodeVertices g_processPointSize[] = {
    &CTNL::TProcessPointSize<false, VERTEX_FIXED>,
    &CTNL::TProcessPointSize<false, VERTEX_FLOAT>,
    &CTNL::TProcessPointSize<true, VERTEX_FIXED>,
    &CTNL::TProcessPointSize<true, VERTEX_FLOAT>,
};

static const CTNL::PfnDecodeVertices g_processFog[] = {
    &CTNL::TProcessFog<FogLinear>,
    &CTNL::TProcessFog<FogExp>,
    &CTNL::TProcessFog<FogExp2>,
};

GLenum CTNL::SetupTNLStates(GLenum mode, int first, uint32_t count) {
  GLenum err;

  auto rasterFlags = rasterID_.Flags;

  TNLFlags_.Value = 0;

  if (vertexStates_.Position) {
    err = positionArray_.Prepare(&positionDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::Prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    __glLogError("The vertex array input is disabled.\r\n");
    return GL_INVALID_OPERATION;
  }

  uint32_t uiFormatType = (positionArray_.Format - VERTEX_BYTE2) / 4;
  uint32_t uiFormatSize = positionArray_.Format - 1 - uiFormatType * 4;
  uint32_t uiFunc = uiFormatType * 3 + uiFormatSize - 1;
  assert(uiFunc < __countof(g_decodePosition));
  pfnDecodePosition_ = g_decodePosition[uiFunc];

  uint32_t numVertives = count + CLIP_BUFFER_SIZE;

  clipVerticesBaseIndex_ = count;

  uint8_t *pbVertexData = nullptr;

  pbVertexData_[VERTEXDATA_FLAGS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(uint16_t), 4);

  pbVertexData_[VERTEXDATA_WORLDPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(VECTOR4), 4);

  pbVertexData_[VERTEXDATA_CLIPPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(VECTOR4), 4);

  pbVertexData_[VERTEXDATA_SCREENPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(RDVECTOR), 4);

  if (dirtyFlags_.ModelViewProj) {
    this->UpdateModelViewProj();
  }

  if (dirtyFlags_.ScreenXform) {
    this->UpdateScreenXform();
  }

  if (caps_.ClipPlanes) {
    this->UpdateClipPlanes();
  }

  if (GL_POINTS == mode) {
    err = this->UpdatePoints(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("CTNL::UpdatePoints() failed, err = %d.\r\n", err);
      return err;
    }
  }

  if (rasterFlags.Color) {
    err = this->UpdateColor(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("CTNL::UpdateColor() failed, err = %d.\r\n", err);
      return err;
    }
  }

  if (rasterFlags.NumTextures) {
    err = this->UpdateTexcoords(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("CTNL::UpdateTexcoords() failed, err = %d.\r\n", err);
      return err;
    }
  }

  if (rasterFlags.Fog) {
    this->UpdateFog(&pbVertexData, first, count);
  }

  uint32_t buffSize = pbVertexData - (uint8_t *)nullptr;

  vertexBuffer_.resize(buffSize);

  // Update vertex buffer offsets
  {
    uint32_t offset = vertexBuffer_.data() - (uint8_t *)nullptr;
    for (uint32_t i = 0; i < VERTEXDATA_SIZE; ++i) {
      pbVertexData_[i] += offset;
    }
    pbVertexColor_ += offset;
  }

  return GL_NO_ERROR;
}

void CTNL::ProcessVertices(uint32_t count) {
  auto flags = TNLFlags_;

  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEXDATA_FLAGS]);
  auto pvWorldPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEXDATA_WORLDPOS]);
  auto pvClipPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEXDATA_CLIPPOS]);
  auto pvScreenPos = reinterpret_cast<RDVECTOR *>(pbVertexData_[VERTEXDATA_SCREENPOS]);

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
    pwFlags[i] = (uint16_t)clipFlags;
  }

  if (flags.UserClipPlanes) {
    // Compute user clipping flags
    clipUnion |= this->CalcUserClipFlags(count);
  }

  // Transform to screen space
  if (0 == clipUnion) {
    this->XformScreenSpace(pvScreenPos, pvClipPos, count);
  } else {
    for (uint32_t i = 0; i < count; ++i) {
      if (0 == pwFlags[i]) {
        this->XformScreenSpace(&pvScreenPos[i], &pvClipPos[i], 1);
      }
    }
  }

  if (flags.EyeSpace || flags.EyeSpaceZ) {
    this->XformEyeSpace(count);
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

uint32_t CTNL::CalcClipFlags(const VECTOR4 &vPosition) {
  auto cx = vPosition.x;
  auto cy = vPosition.y;
  auto cz = vPosition.z;
  auto cw = vPosition.w;

  uint32_t clipFlags;

  auto dl = cw + cx;
  auto dr = cw - cx;
  auto db = cw + cy;
  auto dt = cw - cy;
  auto df = cw + cz;
  auto dk = cw - cz;

  clipFlags = (*(const uint32_t *)&dl >> 31) << CLIP_LEFT;
  clipFlags |= (*(const uint32_t *)&dr >> 31) << CLIP_RIGHT;
  clipFlags |= (*(const uint32_t *)&db >> 31) << CLIP_BOTTOM;
  clipFlags |= (*(const uint32_t *)&dt >> 31) << CLIP_TOP;
  clipFlags |= (*(const uint32_t *)&df >> 31) << CLIP_FRONT;
  clipFlags |= (*(const uint32_t *)&dk >> 31) << CLIP_BACK;

  return clipFlags;
}

uint32_t CTNL::CalcUserClipFlags(uint32_t count) {
  uint32_t clipUnion = 0;

  auto pwFlags = reinterpret_cast<uint16_t *>(pbVertexData_[VERTEXDATA_FLAGS]);
  auto pvClipPos = reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEXDATA_CLIPPOS]);

  for (uint32_t i = 0; i < count; ++i) {
    uint32_t clipFlags = 0;

    auto &vClipPos = pvClipPos[i];

    for (uint32_t j = 0, activeMask = caps_.ClipPlanes; activeMask;
         ++j, activeMask >>= 1) {
      if (activeMask & 1) {
        assert(j < vClipPlanesCS_.size());
        auto fDist = Math::TDot<floatf>(vClipPos, vClipPlanesCS_[j]);
        clipFlags |= (*(const uint32_t *)&fDist >> 31) << (CLIP_PLANE0 + j);
      }
    }

    pwFlags[i] |= clipFlags;
    clipUnion |= clipFlags;
  }

  return clipUnion;
}

void CTNL::XformScreenSpace(RDVECTOR *pRDVertex, const VECTOR4 *pvClipPos,
                            uint32_t count) {
  assert(pRDVertex);
  assert(pvClipPos);

  auto iScaleX = screenXform_.iScaleX;
  auto iScaleY = screenXform_.iScaleY;
  auto fScaleZ = screenXform_.fScaleZ;

  auto fMinX = screenXform_.fMinX;
  auto fMinY = screenXform_.fMinY;
  auto fMinZ = screenXform_.fMinZ;

  for (uint32_t i = 0; i < count; ++i) {
    if (!Math::TIsZero(pvClipPos[i].w - fONE)) {
      // Perspective screen space transform
      auto fRhw = Math::TInv<floatRW>(pvClipPos[i].w);
      pRDVertex[i].x = fMinX + Math::TMulRnd<fixed4>(pvClipPos[i].x, fRhw, iScaleX);
      pRDVertex[i].y = fMinY + Math::TMulRnd<fixed4>(pvClipPos[i].y, fRhw, iScaleY);
      pRDVertex[i].z = fMinZ + Math::TMulRnd<float20>(pvClipPos[i].z, fRhw, fScaleZ);
      pRDVertex[i].rhw = fRhw;
    } else {
      // Affine screen space transform
      pRDVertex[i].x = fMinX + Math::TMulRnd<fixed4>(pvClipPos[i].x, iScaleX);
      pRDVertex[i].y = fMinY + Math::TMulRnd<fixed4>(pvClipPos[i].y, iScaleY);
      pRDVertex[i].z = fMinZ + Math::TMulRnd<float20>(pvClipPos[i].z, fScaleZ);
      pRDVertex[i].rhw = TConst<floatRW>::One();
    }
  }
}

void CTNL::XformEyeSpace(uint32_t count) {
  auto &matEyeXform = pMsModelView_->GetMatrix();

  auto pvWorldPos =
      reinterpret_cast<VECTOR4 *>(pbVertexData_[VERTEXDATA_WORLDPOS]);
  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEXDATA_EYEPOS]);

  if (TNLFlags_.EyeSpace) {
    for (uint32_t i = 0; i < count; ++i) {
      Math::Mul(&pvEyePos[i], pvWorldPos[i], matEyeXform);
    }
  } else {
    for (uint32_t i = 0; i < count; ++i) {
      pvEyePos[i].z = Math::MulAdd(
          pvWorldPos[i].x, matEyeXform._31, pvWorldPos[i].y, matEyeXform._32,
          pvWorldPos[i].z, matEyeXform._33, pvWorldPos[i].w, matEyeXform._34);
    }
  }
}

void CTNL::ProcessPointSize(uint32_t count) {
  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEXDATA_EYEPOS]);
  auto pfPointSizes =
      reinterpret_cast<fixed4 *>(pbVertexData_[VERTEXDATA_POINTSIZE]);

  auto &vAttenuation = pointParams_.vAttenuation;

  auto fPointSize = fPointSize_;

  if (!TNLFlags_.PointSizeQAttn) {
    if (!Math::TIsZero(vAttenuation.x - fONE)) {
      fPointSize *= Math::TInvSqrt(vAttenuation.x);
    } else {
      fPointSize *= vAttenuation.x;
    }
  }

  for (uint32_t i = 0; i < count; ++i) {
    if (TNLFlags_.PointSizeQAttn) {
      auto fEyeDist = Math::TAbs(pvEyePos[i].z);
      auto fInvAtt = vAttenuation.z * fEyeDist * fEyeDist +
                     vAttenuation.y * fEyeDist + vAttenuation.x;
      if (!Math::TIsZero(fInvAtt - fONE)) {
        fPointSize *= Math::TInvSqrt(fInvAtt);
      }
    }

    pfPointSizes[i] = static_cast<fixed4>(Math::TMax<floatf>(fPointSize, fONE));
  }
}

void CTNL::ProcessColor(uint32_t count) {
  auto pcFrontColors =
      reinterpret_cast<ColorARGB *>(pbVertexData_[VERTEXDATA_FRONTCOLOR]);

  // Clamp the color
  ColorARGB cColor(Math::TToUNORM8(Math::TSat(vColor_.w)),
                   Math::TToUNORM8(Math::TSat(vColor_.x)),
                   Math::TToUNORM8(Math::TSat(vColor_.y)),
                   Math::TToUNORM8(Math::TSat(vColor_.z)));

  for (uint32_t i = 0; i < count; ++i) {
    pcFrontColors[i] = cColor;
  }
}

void CTNL::ProcessLights_OneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                  const VECTOR3 &vNormal,
                                  const VECTOR4 &vVertexColor) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;       // Light vector
    auto fAtt = fONE; // Attenuation distance

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
      auto fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        auto fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = Math::TSqrt(fDistSq);
        auto fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vFrontDS = pLight->vScaledAmbient;

    auto fDotNL = Math::TDot<floatf>(vNormal, vL);
    if (fDotNL > fZERO) {
      auto &vLightDiffuse = pLight->GetColor(GL_DIFFUSE);
      vFrontDS.x += vVertexColor.x * vLightDiffuse.x * fDotNL;
      vFrontDS.y += vVertexColor.y * vLightDiffuse.y * fDotNL;
      vFrontDS.z += vVertexColor.z * vLightDiffuse.z * fDotNL;

      VECTOR3 vH;

      // Compute the Halfway vector
      if (pLight->Flags.DirectionalLight) {
        vH = pLight->vHalfway;
      } else {
        VECTOR3 vDir(fZERO, fZERO, fONE);
        Math::Add(&vH, vL, vDir);
        Math::Normalize(&vH);
      }

      auto fDotNH = Math::TDot<floatf>(vNormal, vH);
      if (fDotNH > fZERO) {
        // Compute the specular coefficient
        auto fCoeff = Math::TPow(fDotNH, fMatShininess_);
        if (!Math::TIsZero(fCoeff)) {
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

void CTNL::ProcessLights_OneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                  const VECTOR3 &vNormal) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;       // Light vector
    auto fAtt = fONE; // Attenuation distance

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
      auto fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        auto fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = Math::TSqrt(fDistSq);
        auto fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    auto vFrontDS = pLight->vScaledAmbient;

    auto fDotNL = Math::TDot<floatf>(vNormal, vL);
    if (fDotNL > fZERO) {
      vFrontDS.x += pLight->vScaledDiffuse.x * fDotNL;
      vFrontDS.y += pLight->vScaledDiffuse.y * fDotNL;
      vFrontDS.z += pLight->vScaledDiffuse.z * fDotNL;

      VECTOR3 vH;

      // Compute the Halfway vector
      if (pLight->Flags.DirectionalLight) {
        vH = pLight->vHalfway;
      } else {
        VECTOR3 vDir(fZERO, fZERO, fONE);
        Math::Add(&vH, vL, vDir);
        Math::Normalize(&vH);
      }

      auto fDotNH = Math::TDot<floatf>(vNormal, vH);
      if (fDotNH > fZERO) {
        // Compute the specular coefficient
        auto fCoeff = Math::TPow(fDotNH, fMatShininess_);
        if (!Math::TIsZero(fCoeff)) {
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

void CTNL::ProcessLights_TwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                  const VECTOR3 &vNormal,
                                  const VECTOR4 &vVertexColor) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;       // Light vector
    VECTOR3 vH;       // Halfway vector
    auto fAtt = fONE; // Attenuation distance

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
      auto fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        auto fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the halfway vector
      VECTOR3 vDir(fZERO, fZERO, fONE);
      Math::Add(&vH, vL, vDir);
      Math::Normalize(&vH);

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = Math::TSqrt(fDistSq);
        auto fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vColor;
    auto vFrontDS = pLight->vScaledAmbient;
    auto vBackDS = vFrontDS;

    auto fDotNL = Math::TDot<floatf>(vNormal, vL);
    auto fDiffFactor = (fDotNL > fZERO) ? fDotNL : -fDotNL;

    auto &vLightDiffuse = pLight->GetColor(GL_DIFFUSE);
    vColor.x = vVertexColor.x * vLightDiffuse.x * fDiffFactor;
    vColor.y = vVertexColor.y * vLightDiffuse.y * fDiffFactor;
    vColor.z = vVertexColor.z * vLightDiffuse.z * fDiffFactor;

    auto fDotNH = Math::TDot<floatf>(vNormal, vH);
    if (fDotNL <= fZERO) {
      fDotNH = -fDotNH;
    }

    if (fDotNH > fZERO) {
      // Compute the specular coefficient
      auto fCoeff = Math::TPow(fDotNH, fMatShininess_);
      if (!Math::TIsZero(fCoeff)) {
        vColor.x += pLight->vScaledSpecular.x * fCoeff;
        vColor.y += pLight->vScaledSpecular.y * fCoeff;
        vColor.z += pLight->vScaledSpecular.z * fCoeff;
      }
    }

    if (fDotNL > fZERO) {
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

void CTNL::ProcessLights_TwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                  const VECTOR3 &vNormal) {
  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;       // Light vector
    VECTOR3 vH;       // Halfway vector
    auto fAtt = fONE; // Attenuation distance

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
      auto fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        auto fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the halfway vector
      VECTOR3 vDir(fZERO, fZERO, fONE);
      Math::Add(&vH, vL, vDir);
      Math::Normalize(&vH);

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        auto fDist = Math::TSqrt(fDistSq);
        auto fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        auto fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vColor;
    auto vFrontDS = pLight->vScaledAmbient;
    auto vBackDS = vFrontDS;

    auto fDotNL = Math::TDot<floatf>(vNormal, vL);
    auto fDiffFactor = (fDotNL > fZERO) ? fDotNL : -fDotNL;

    vColor.x = pLight->vScaledDiffuse.x * fDiffFactor;
    vColor.y = pLight->vScaledDiffuse.y * fDiffFactor;
    vColor.z = pLight->vScaledDiffuse.z * fDiffFactor;

    auto fDotNH = Math::TDot<floatf>(vNormal, vH);
    if (fDotNL <= fZERO) {
      fDotNH = -fDotNH;
    }

    if (fDotNH > fZERO) {
      // Compute the specular coefficient
      auto fCoeff = Math::TPow(fDotNH, fMatShininess_);
      if (!Math::TIsZero(fCoeff)) {
        vColor.x += pLight->vScaledSpecular.x * fCoeff;
        vColor.y += pLight->vScaledSpecular.y * fCoeff;
        vColor.z += pLight->vScaledSpecular.z * fCoeff;
      }
    }

    if (fDotNL > fZERO) {
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

void CTNL::ProcessTexCoords(uint32_t dstIndex, uint32_t srcIndex,
                            uint32_t count) {
  auto pvTexCoords = reinterpret_cast<TEXCOORD2 *>(
      pbVertexData_[VERTEXDATA_TEXCOORD0 + dstIndex]);

  auto vIn = vTexCoords_[srcIndex];

  if (!pMsTexCoords_[srcIndex]->IsIdentity()) {
    Math::Mul(&vIn, vIn, pMsTexCoords_[srcIndex]->GetMatrix());
  }

  for (uint32_t k = 0; k < count; ++k) {
    pvTexCoords[k].m[0] = Math::TCast<float20>(vIn.x);
    pvTexCoords[k].m[1] = Math::TCast<float20>(vIn.y);
  }
}

GLenum CTNL::UpdatePoints(uint8_t **ppbVertexData, int first, uint32_t count) {
  GLenum err;

  if (vertexStates_.PointSize) {
    err = pointSizeArray_.Prepare(&pointSizeDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::Prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    pointSizeDecode_.pBits = nullptr;
  }

  if (!Math::TIsZero(pointParams_.vAttenuation.y) ||
      !Math::TIsZero(pointParams_.vAttenuation.z)) {
    TNLFlags_.PointSizeQAttn = 1;
    TNLFlags_.EyeSpaceZ = 1;
  }

  pbVertexData_[VERTEXDATA_POINTSIZE] = *ppbVertexData;
  *ppbVertexData = __alignPtr(
      *ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(fixed4), 4);

  if (pointSizeDecode_.pBits) {
    uint32_t uiFunc = (pointSizeArray_.Format - VERTEX_FIXED) / 4;

    if (TNLFlags_.PointSizeQAttn) {
      uiFunc += 2;
    }

    assert(uiFunc < __countof(g_processPointSize));
    pfnPointSize_ = g_processPointSize[uiFunc];
  } else {
    pfnPointSize_ = &CTNL::ProcessPointSize;
  }

  TNLFlags_.PointSize = 1;

  return GL_NO_ERROR;
}

GLenum CTNL::UpdateColor(uint8_t **ppbVertexData, int first, uint32_t count) {
  GLenum err;

  if (vertexStates_.Color) {
    err = colorArray_.Prepare(&colorDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::Prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    colorDecode_.pBits = nullptr;
  }

  pbVertexData_[VERTEXDATA_FRONTCOLOR] = *ppbVertexData;
  pbVertexColor_ = *ppbVertexData;
  *ppbVertexData = __alignPtr(*ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(ColorARGB), 4);

  cullStates_.bTwoSidedLighting = false;

  if (caps_.Lighting) {
    err = this->UpdateLighting(ppbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError("CTNL::UpdateLighting() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    if (colorDecode_.pBits) {
      uint32_t uiFunc = (colorArray_.Format - VERTEX_FIXED4) / 4;
      assert(uiFunc < __countof(g_processVertexColor));
      pfnColor_ = g_processVertexColor[uiFunc];
    } else {
      pfnColor_ = &CTNL::ProcessColor;
    }
  }

  TNLFlags_.Color = 1;

  return GL_NO_ERROR;
}

GLenum CTNL::UpdateLighting(uint8_t **ppbVertexData, int first,
                            uint32_t count) {
  GLenum err;

  if (dirtyFlags_.ModelViewInvT33) {
    this->UpdateModelViewInvT33();
  }

  if (vertexStates_.Normal) {
    err = normalArray_.Prepare(&normalDecode_, first, count);
    if (__glFailed(err)) {
      __glLogError("VertexArray::Prepare() failed, err = %d.\r\n", err);
      return err;
    }
  } else {
    normalDecode_.pBits = nullptr;
  }

  if (normalDecode_.pBits) {
    TNLFlags_.Normalize = caps_.Normalize;
  } else {
    if (dirtyFlags_.NormalizeNormal) {
      this->UpdateNormal();
    }
  }

  if (dirtyFlags_.ScaledAmbient) {
    this->UpdateMaterial();
  }

  if (dirtyFlags_.Lights) {
    this->SetupLights();
  }

  if (pActiveLights_) {
    TNLFlags_.EyeSpace = 1;

    if (dirtyLights_.Ambient || dirtyLights_.Diffuse || dirtyLights_.Specular) {
      this->UpdateLights();
    }
  }

  uint32_t uiFunc = 0;

  if (normalDecode_.pBits) {
    uiFunc = 1 + (normalArray_.Format - VERTEX_BYTE3) / 4;
  }

  if (caps_.ColorMaterial) {
    uint32_t _uiFunc = 1;

    if (colorDecode_.pBits) {
      _uiFunc += 1 + (normalArray_.Format - VERTEX_FIXED4) / 4;
    }

    uiFunc += _uiFunc * 5;
  }

  if (caps_.TwoSidedLighting) {
    cullStates_.bTwoSidedLighting = true;

    pbVertexData_[VERTEXDATA_BACKCOLOR] = *ppbVertexData;
    *ppbVertexData = __alignPtr(*ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(ColorARGB), 4);

    uiFunc += 5 * 5;
  }

  assert(uiFunc < __countof(g_processLighting));
  pfnColor_ = g_processLighting[uiFunc];

  return GL_NO_ERROR;
}

GLenum CTNL::UpdateTexcoords(uint8_t **ppbVertexData, int first,
                             uint32_t count) {
  GLenum err;

  for (uint32_t i = 0, j = 0, mask = caps_.Texture2D; mask; mask >>= 1, ++i) {
    if (mask & 0x1) {
      if ((vertexStates_.TexCoords >> i) & 0x1) {
        err = texCoordArrays_[i].Prepare(&texCoordDecodes_[i], first, count);
        if (__glFailed(err)) {
          __glLogError("VertexArray::Prepare() failed, err = %d.\r\n", err);
          return err;
        }
      } else {
        texCoordDecodes_[i].pBits = nullptr;
      }

      pbVertexData_[VERTEXDATA_TEXCOORD0 + j++] = *ppbVertexData;
      *ppbVertexData = __alignPtr(*ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(TEXCOORD2), 4);

      if (texCoordDecodes_[i].pBits) {
        uint32_t uiFormatType = (texCoordArrays_[i].Format - VERTEX_BYTE2) / 4;
        uint32_t uiFormatSize =
            texCoordArrays_[i].Format - 1 - uiFormatType * 4;
        uint32_t uiFunc = uiFormatType * 3 + uiFormatSize - 1;

        if (!pMsTexCoords_[i]->IsIdentity()) {
          uiFunc += 12;
        }

        assert(uiFunc < __countof(g_processTexcoords));
        pfnTexCoords_[i] = g_processTexcoords[uiFunc];
      } else {
        pfnTexCoords_[i] = &CTNL::ProcessTexCoords;
      }
    }
  }

  TNLFlags_.TexCoords = caps_.Texture2D;

  return GL_NO_ERROR;
}

void CTNL::UpdateFog(uint8_t **ppbVertexData, int /*first*/, uint32_t count) {
  TNLFlags_.Fog = 1;
  TNLFlags_.EyeSpaceZ = 1;

  if (dirtyFlags_.FogRatio) {
    auto fFogStart = fog_.GetFactor(GL_FOG_START);
    auto fFogEnd = fog_.GetFactor(GL_FOG_END);

    if (fFogStart != fFogEnd) {
      fog_.fRatio = Math::TInv<fixedRF>(fFogEnd - fFogStart);
    } else {
      fog_.fRatio = TConst<fixedRF>::Zero();
    }

    dirtyFlags_.FogRatio = 0;
  }

  pbVertexData_[VERTEXDATA_FOG] = *ppbVertexData;
  *ppbVertexData = __alignPtr(*ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(float20), 4);

  uint32_t uiFunc = fog_.Mode;
  assert(uiFunc < __countof(g_processFog));
  pfnFog_ = g_processFog[uiFunc];
}

void CTNL::UpdateModelViewInvT44() {
  MATRIX44 matTmp;
  Math::Inverse(&matTmp, pMsModelView_->GetMatrix());
  Math::Transpose(&mModelViewInvT_, matTmp);
  dirtyFlags_.ModelViewInvT44 = 0;

  if (!caps_.RescaleNormal) {
    dirtyFlags_.ModelViewInvT33 = 0;
    dirtyFlags_.NormalizeNormal = 1;
  }
}

void CTNL::UpdateModelViewProj() {
  Math::Mul(&mModelViewProj_, pMsProjection_->GetMatrix(), pMsModelView_->GetMatrix());
  dirtyFlags_.ModelViewProj = 0;
}

void CTNL::UpdateScreenXform() {
  screenXform_.fMinX = static_cast<fixed4>((viewport_.left + viewport_.right) / 2);
  screenXform_.iScaleX = (viewport_.right - viewport_.left) / 2;

  screenXform_.fMinY = static_cast<fixed4>((viewport_.top + viewport_.bottom) / 2);
  screenXform_.iScaleY = (viewport_.bottom - viewport_.top) / 2;

  screenXform_.fMinZ = static_cast<float20>((depthRange_.fNear + depthRange_.fFar) / 2);
  screenXform_.fScaleZ = (depthRange_.fFar - depthRange_.fNear) / 2;

  dirtyFlags_.ScreenXform = 0;
}

void CTNL::UpdateClipPlanes() {
  TNLFlags_.UserClipPlanes = 1;

  if (dirtyFlags_.ClipPlanesCS) {
    if (dirtyFlags_.ProjectionInvT) {
      this->UpdateProjectionInvT();
    }

    uint32_t updateMask = dirtyFlags_.ClipPlanesCS & caps_.ClipPlanes;
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

void CTNL::UpdateProjectionInvT() {
  MATRIX44 matTmp;
  Math::Inverse(&matTmp, pMsProjection_->GetMatrix());
  Math::Transpose(&mProjectionInvT_, matTmp);

  dirtyFlags_.ProjectionInvT = 0;
}

void CTNL::UpdateModelViewInvT33() {
  MATRIX44 matTmp;
  Math::Inverse33(&matTmp, pMsModelView_->GetMatrix());

  if (caps_.RescaleNormal) {
    auto fSum = (matTmp._31 * matTmp._31) + 
                (matTmp._32 * matTmp._32) +
                (matTmp._33 * matTmp._33);

    if (!Math::TIsZero(fSum - fONE)) {
      auto fFactor = Math::TInvSqrt(fSum);

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

void CTNL::UpdateNormal() {
  // Transform the normal to world space
  Math::Mul(&vNormNormal_, vNormal_, mModelViewInvT_);

  // Normalize the normal
  Math::Normalize(&vNormNormal_);

  dirtyFlags_.NormalizeNormal = 0;
}

void CTNL::UpdateMaterial() {
  auto &vMaterialAmbient = material_.GetColor(GL_AMBIENT);
  auto &vMaterialDiffuse = material_.GetColor(GL_DIFFUSE);

  vScaledAmbient_.x = vMaterialAmbient.x * vLightModelAmbient_.x;
  vScaledAmbient_.y = vMaterialAmbient.y * vLightModelAmbient_.y;
  vScaledAmbient_.z = vMaterialAmbient.z * vLightModelAmbient_.z;
  vScaledAmbient_.w = vMaterialDiffuse.w;

  dirtyFlags_.ScaledAmbient = 0;
}

void CTNL::SetupLights() {
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

void CTNL::UpdateLights() {
  auto &vMaterialAmbient = material_.GetColor(GL_AMBIENT);
  auto &vMaterialDiffuse = material_.GetColor(GL_DIFFUSE);
  auto &vMaterialSpecular = material_.GetColor(GL_SPECULAR);

  auto dirtyLights = dirtyLights_;

  for (auto *pLight = pActiveLights_; pLight; pLight = pLight->pNext) {
    uint32_t mask = 1 << (pLight - lights_.data());

    if (dirtyLights.Ambient & mask) {
      auto &vLightAmbient = pLight->GetColor(GL_AMBIENT);
      pLight->vScaledAmbient.x = vLightAmbient.x * vMaterialAmbient.x;
      pLight->vScaledAmbient.y = vLightAmbient.y * vMaterialAmbient.y;
      pLight->vScaledAmbient.z = vLightAmbient.z * vMaterialAmbient.z;
    }

    if (dirtyLights.Diffuse & mask) {
      auto &vLightDiffuse = pLight->GetColor(GL_DIFFUSE);
      pLight->vScaledDiffuse.x = vMaterialDiffuse.x * vLightDiffuse.x;
      pLight->vScaledDiffuse.y = vMaterialDiffuse.y * vLightDiffuse.y;
      pLight->vScaledDiffuse.z = vMaterialDiffuse.z * vLightDiffuse.z;
    }

    if (dirtyLights.Specular & mask) {
      auto &vLightSpecular = pLight->GetColor(GL_SPECULAR);
      pLight->vScaledSpecular.x = vMaterialSpecular.x * vLightSpecular.x;
      pLight->vScaledSpecular.y = vMaterialSpecular.y * vLightSpecular.y;
      pLight->vScaledSpecular.z = vMaterialSpecular.z * vLightSpecular.z;
    }

    if (dirtyLights.Position & mask) {
      if (pLight->Flags.DirectionalLight) {
        // Normalize the position
        Math::Normalize(reinterpret_cast<VECTOR3 *>(&pLight->vPosition));

        // Compute the halfway vector
        VECTOR3 vDir(fZERO, fZERO, fONE);
        Math::Add(&pLight->vHalfway, reinterpret_cast<const VECTOR3 &>(pLight->vPosition), vDir);

        // Normalize the halfway vector
        Math::Normalize(&pLight->vHalfway);
      } else {
        if (Math::TIsZero(pLight->GetAttenuation(GL_CONSTANT_ATTENUATION) - fONE) &&
            Math::TIsZero(pLight->GetAttenuation(GL_LINEAR_ATTENUATION)) &&
            Math::TIsZero(pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION))) {
          pLight->Flags.Attenuation = 0;
        } else {
          pLight->Flags.Attenuation = 1;
        }

        if (Math::TIsZero(pLight->fSpotCutOff - f180)) {
          pLight->Flags.SpotLight = 0;
        } else {
          pLight->Flags.SpotLight = 1;
        }
      }
    }
  }

  dirtyLights_.Value = 0;
}

void CTNL::UpdateMatrixDirtyFlags() {
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
