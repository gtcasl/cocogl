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

static const PFN_DECODEPOSITION g_decodePosition[] = {
    TDecodePosition<VERTEX_BYTE2>,  TDecodePosition<VERTEX_BYTE3>,
    TDecodePosition<VERTEX_BYTE4>,  TDecodePosition<VERTEX_SHORT2>,
    TDecodePosition<VERTEX_SHORT3>, TDecodePosition<VERTEX_SHORT4>,
    TDecodePosition<VERTEX_FIXED2>, TDecodePosition<VERTEX_FIXED3>,
    TDecodePosition<VERTEX_FIXED4>, TDecodePosition<VERTEX_FLOAT2>,
    TDecodePosition<VERTEX_FLOAT3>, TDecodePosition<VERTEX_FLOAT4>,
};

static const CTNL::PFN_VERTEXDECODE g_processLighting[] = {
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

static const CTNL::PFN_VERTEXDECODE g_processVertexColor[] = {
    &CTNL::TProcessVertexColor<VERTEX_FIXED4>,
    &CTNL::TProcessVertexColor<VERTEX_FLOAT4>,
    &CTNL::TProcessVertexColor<VERTEX_RGBA>,
};

static const CTNL::PFN_DECODETEXCOORD g_processTexcoords[] = {
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

static const CTNL::PFN_VERTEXDECODE g_processPointSize[] = {
    &CTNL::TProcessPointSize<false, VERTEX_FIXED>,
    &CTNL::TProcessPointSize<false, VERTEX_FLOAT>,
    &CTNL::TProcessPointSize<true, VERTEX_FIXED>,
    &CTNL::TProcessPointSize<true, VERTEX_FLOAT>,
};

static const CTNL::PFN_VERTEXDECODE g_processFog[] = {
    &CTNL::TProcessFog<FogLinear>, &CTNL::TProcessFog<FogExp>,
    &CTNL::TProcessFog<FogExp2>,
};

GLenum CTNL::SetupTNLStates(GLenum mode, int first, unsigned count) {
  GLenum err;

  const RASTERFLAGS rasterFlags = m_rasterID.Flags;

  m_TNLFlags.Value = 0;

  if (m_vertexStates.Position) {
    err = m_positionArray.Prepare(&m_positionDecode, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("VertexArray::Prepare() failed, err = %d.\r\n"), err);
      return err;
    }
  } else {
    __glLogError(_T("The vertex array input is disabled.\r\n"));
    return GL_INVALID_OPERATION;
  }

  const unsigned uiFormatType = (m_positionArray.Format - VERTEX_BYTE2) / 4;
  const unsigned uiFormatSize = m_positionArray.Format - 1 - uiFormatType * 4;
  const unsigned uiFunc = uiFormatType * 3 + uiFormatSize - 1;
  ASSERT(uiFunc < __countof(g_decodePosition));
  m_pfnDecodePosition = g_decodePosition[uiFunc];

  const unsigned numVertives = count + CLIP_BUFFER_SIZE;

  m_clipVerticesBaseIndex = count;

  uint8_t *pbVertexData = nullptr;

  m_pbVertexData[VERTEXDATA_FLAGS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(uint16_t), 4);

  m_pbVertexData[VERTEXDATA_WORLDPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(VECTOR4), 4);

  m_pbVertexData[VERTEXDATA_CLIPPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(VECTOR4), 4);

  m_pbVertexData[VERTEXDATA_SCREENPOS] = pbVertexData;
  pbVertexData = __alignPtr(pbVertexData + numVertives * sizeof(RDVECTOR), 4);

  if (m_dirtyFlags.ModelViewProj) {
    this->UpdateModelViewProj();
  }

  if (m_dirtyFlags.ScreenXform) {
    this->UpdateScreenXform();
  }

  if (m_caps.ClipPlanes) {
    this->UpdateClipPlanes();
  }

  if (GL_POINTS == mode) {
    err = this->UpdatePoints(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("CTNL::UpdatePoints() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  if (rasterFlags.Color) {
    err = this->UpdateColor(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("CTNL::UpdateColor() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  if (rasterFlags.NumTextures) {
    err = this->UpdateTexcoords(&pbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("CTNL::UpdateTexcoords() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  if (rasterFlags.Fog) {
    this->UpdateFog(&pbVertexData, first, count);
  }

  const unsigned buffSize = pbVertexData - (uint8_t *)nullptr;

  err = GLERROR_FROM_HRESULT(m_vertexBuffer.Resize(buffSize));
  if (__glFailed(err)) {
    __glLogError(_T("TArray::Resize() failed, err = %d.\r\n"), err);
    return err;
  }

  // Update vertex buffer offsets
  {
    const unsigned offset = m_vertexBuffer.GetBegin() - (uint8_t *)nullptr;
    for (unsigned i = 0; i < VERTEXDATA_SIZE; ++i) {
      m_pbVertexData[i] += offset;
    }
    m_pbVertexColor += offset;
  }

  return GL_NO_ERROR;
}

void CTNL::ProcessVertices(unsigned count) {
  const TNLFLAGS flags = m_TNLFlags;

  uint16_t *const pwFlags = (uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  VECTOR4 *const pvWorldPos = (VECTOR4 *)m_pbVertexData[VERTEXDATA_WORLDPOS];
  VECTOR4 *const pvClipPos = (VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];
  RDVECTOR *const pvScreenPos =
      (RDVECTOR *)m_pbVertexData[VERTEXDATA_SCREENPOS];

  // Decode vertices
  (m_pfnDecodePosition)(pvWorldPos, m_positionDecode.pBits,
                        m_positionDecode.Stride, count);

  unsigned clipUnion = 0;

  for (unsigned i = 0; i < count; ++i) {
    // Tranform vertex position to clip space
    Math::Mul(&pvClipPos[i], pvWorldPos[i], m_mModelViewProj);

    // Compute frustum clipping flags
    const unsigned clipFlags = this->CalcClipFlags(pvClipPos[i]);
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
    for (unsigned i = 0; i < count; ++i) {
      if (0 == pwFlags[i]) {
        this->XformScreenSpace(&pvScreenPos[i], &pvClipPos[i], 1);
      }
    }
  }

  if (flags.EyeSpace || flags.EyeSpaceZ) {
    this->XformEyeSpace(count);
  }

  if (flags.PointSize) {
    (this->*m_pfnPointSize)(count);
  }

  if (flags.Color) {
    (this->*m_pfnColor)(count);
  }

  if (flags.TexCoords) {
    for (unsigned i = 0, j = 0, mask = m_caps.Texture2D; mask;
         mask >>= 1, ++i) {
      if (mask & 0x1) {
        (this->*m_pfnTexCoords[i])(j++, i, count);
      }
    }
  }

  if (flags.Fog) {
    (this->*m_pfnFog)(count);
  }
}

unsigned CTNL::CalcClipFlags(const VECTOR4 &vPosition) {
  const floatf cx = vPosition.x;
  const floatf cy = vPosition.y;
  const floatf cz = vPosition.z;
  const floatf cw = vPosition.w;

  unsigned clipFlags;

  const floatf dl = cw + cx;
  const floatf dr = cw - cx;
  const floatf db = cw + cy;
  const floatf dt = cw - cy;
  const floatf df = cw + cz;
  const floatf dk = cw - cz;

  clipFlags = (*(const unsigned *)&dl >> 31) << CLIP_LEFT;
  clipFlags |= (*(const unsigned *)&dr >> 31) << CLIP_RIGHT;
  clipFlags |= (*(const unsigned *)&db >> 31) << CLIP_BOTTOM;
  clipFlags |= (*(const unsigned *)&dt >> 31) << CLIP_TOP;
  clipFlags |= (*(const unsigned *)&df >> 31) << CLIP_FRONT;
  clipFlags |= (*(const unsigned *)&dk >> 31) << CLIP_BACK;

  return clipFlags;
}

unsigned CTNL::CalcUserClipFlags(unsigned count) {
  unsigned clipUnion = 0;

  uint16_t *const pwFlags = (uint16_t *)m_pbVertexData[VERTEXDATA_FLAGS];
  VECTOR4 *const pvClipPos = (VECTOR4 *)m_pbVertexData[VERTEXDATA_CLIPPOS];

  for (unsigned i = 0; i < count; ++i) {
    unsigned clipFlags = 0;

    const VECTOR4 &vClipPos = pvClipPos[i];

    for (unsigned j = 0, activeMask = m_caps.ClipPlanes; activeMask;
         ++j, activeMask >>= 1) {
      if (activeMask & 1) {
        ASSERT(j < m_vClipPlanesCS.GetSize());
        const floatf fDist = Math::TDot<floatf>(vClipPos, m_vClipPlanesCS[j]);
        clipFlags |= (*(const unsigned *)&fDist >> 31) << (CLIP_PLANE0 + j);
      }
    }

    pwFlags[i] |= clipFlags;
    clipUnion |= clipFlags;
  }

  return clipUnion;
}

void CTNL::XformScreenSpace(RDVECTOR *pRDVertex, const VECTOR4 *pvClipPos,
                            unsigned count) {
  ASSERT(pRDVertex);
  ASSERT(pvClipPos);

  const int iScaleX = m_screenXform.iScaleX;
  const int iScaleY = m_screenXform.iScaleY;
  const floatf fScaleZ = m_screenXform.fScaleZ;

  const fixed4 fMinX = m_screenXform.fMinX;
  const fixed4 fMinY = m_screenXform.fMinY;
  const float20 fMinZ = m_screenXform.fMinZ;

  for (unsigned i = 0; i < count; ++i) {
    if (!Math::TIsZero(pvClipPos[i].w - fONE)) {
      // Perspective screen space transform
      const floatRW fRhw = Math::TInv<floatRW>(pvClipPos[i].w);
      pRDVertex[i].x =
          fMinX + Math::TMulRnd<fixed4>(pvClipPos[i].x, fRhw, iScaleX);
      pRDVertex[i].y =
          fMinY + Math::TMulRnd<fixed4>(pvClipPos[i].y, fRhw, iScaleY);
      pRDVertex[i].z =
          fMinZ + Math::TMulRnd<float20>(pvClipPos[i].z, fRhw, fScaleZ);
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

void CTNL::XformEyeSpace(unsigned count) {
  const MATRIX44 &matEyeXform = m_pMsModelView->GetMatrix();

  VECTOR4 *const pvWorldPos = (VECTOR4 *)m_pbVertexData[VERTEXDATA_WORLDPOS];
  VECTOR3 *const pvEyePos = (VECTOR3 *)m_pbVertexData[VERTEXDATA_EYEPOS];

  if (m_TNLFlags.EyeSpace) {
    for (unsigned i = 0; i < count; ++i) {
      Math::Mul(&pvEyePos[i], pvWorldPos[i], matEyeXform);
    }
  } else {
    for (unsigned i = 0; i < count; ++i) {
      pvEyePos[i].z = Math::MulAdd(
          pvWorldPos[i].x, matEyeXform._31, pvWorldPos[i].y, matEyeXform._32,
          pvWorldPos[i].z, matEyeXform._33, pvWorldPos[i].w, matEyeXform._34);
    }
  }
}

void CTNL::ProcessPointSize(unsigned count) {
  VECTOR3 *const pvEyePos = (VECTOR3 *)m_pbVertexData[VERTEXDATA_EYEPOS];
  fixed4 *const pfPointSizes = (fixed4 *)m_pbVertexData[VERTEXDATA_POINTSIZE];

  const VECTOR3 &vAttenuation = m_pointParams.vAttenuation;

  floatf fPointSize = m_fPointSize;

  if (!m_TNLFlags.PointSizeQAttn) {
    if (!Math::TIsZero(vAttenuation.x - fONE)) {
      fPointSize *= Math::TInvSqrt(vAttenuation.x);
    } else {
      fPointSize *= vAttenuation.x;
    }
  }

  for (unsigned i = 0; i < count; ++i) {
    if (m_TNLFlags.PointSizeQAttn) {
      const floatf fEyeDist = Math::TAbs(pvEyePos[i].z);
      const floatf fInvAtt = vAttenuation.z * fEyeDist * fEyeDist +
                             vAttenuation.y * fEyeDist + vAttenuation.x;
      if (!Math::TIsZero(fInvAtt - fONE)) {
        fPointSize *= Math::TInvSqrt(fInvAtt);
      }
    }

    pfPointSizes[i] = static_cast<fixed4>(Math::TMax<floatf>(fPointSize, fONE));
  }
}

void CTNL::ProcessColor(unsigned count) {
  ColorARGB *const pcFrontColors =
      (ColorARGB *)m_pbVertexData[VERTEXDATA_FRONTCOLOR];

  // Clamp the color
  const ColorARGB cColor(Math::TToUNORM8(Math::TSat(m_vColor.w)),
                         Math::TToUNORM8(Math::TSat(m_vColor.x)),
                         Math::TToUNORM8(Math::TSat(m_vColor.y)),
                         Math::TToUNORM8(Math::TSat(m_vColor.z)));

  for (unsigned i = 0; i < count; ++i) {
    pcFrontColors[i] = cColor;
  }
}

void CTNL::ProcessLights_OneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                                  const VECTOR3 &vNormal,
                                  const VECTOR4 &vVertexColor) {
  for (const Light *pLight = m_pActiveLights; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;         // Light vector
    floatf fAtt = fONE; // Attenuation distance

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
      const floatf fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        const floatf fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        const floatf fDist = Math::TSqrt(fDistSq);
        const floatf fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        const floatf fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vFrontDS = pLight->vScaledAmbient;

    const floatf fDotNL = Math::TDot<floatf>(vNormal, vL);
    if (fDotNL > fZERO) {
      const VECTOR4 &vLightDiffuse = pLight->GetColor(GL_DIFFUSE);
      vFrontDS.x += vVertexColor.x * vLightDiffuse.x * fDotNL;
      vFrontDS.y += vVertexColor.y * vLightDiffuse.y * fDotNL;
      vFrontDS.z += vVertexColor.z * vLightDiffuse.z * fDotNL;

      VECTOR3 vH;

      // Compute the Halfway vector
      if (pLight->Flags.DirectionalLight) {
        vH = pLight->vHalfway;
      } else {
        const VECTOR3 vDir(fZERO, fZERO, fONE);
        Math::Add(&vH, vL, vDir);
        Math::Normalize(&vH);
      }

      const floatf fDotNH = Math::TDot<floatf>(vNormal, vH);
      if (fDotNH > fZERO) {
        // Compute the specular coefficient
        const floatf fCoeff = Math::TPow(fDotNH, m_fMatShininess);
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
  for (const Light *pLight = m_pActiveLights; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;         // Light vector
    floatf fAtt = fONE; // Attenuation distance

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
      const floatf fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        const floatf fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        const floatf fDist = Math::TSqrt(fDistSq);
        const floatf fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        const floatf fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vFrontDS = pLight->vScaledAmbient;

    const floatf fDotNL = Math::TDot<floatf>(vNormal, vL);
    if (fDotNL > fZERO) {
      vFrontDS.x += pLight->vScaledDiffuse.x * fDotNL;
      vFrontDS.y += pLight->vScaledDiffuse.y * fDotNL;
      vFrontDS.z += pLight->vScaledDiffuse.z * fDotNL;

      VECTOR3 vH;

      // Compute the Halfway vector
      if (pLight->Flags.DirectionalLight) {
        vH = pLight->vHalfway;
      } else {
        const VECTOR3 vDir(fZERO, fZERO, fONE);
        Math::Add(&vH, vL, vDir);
        Math::Normalize(&vH);
      }

      const floatf fDotNH = Math::TDot<floatf>(vNormal, vH);
      if (fDotNH > fZERO) {
        // Compute the specular coefficient
        const floatf fCoeff = Math::TPow(fDotNH, m_fMatShininess);
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
  for (const Light *pLight = m_pActiveLights; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;         // Light vector
    VECTOR3 vH;         // Halfway vector
    floatf fAtt = fONE; // Attenuation distance

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
      const floatf fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        const floatf fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the halfway vector
      const VECTOR3 vDir(fZERO, fZERO, fONE);
      Math::Add(&vH, vL, vDir);
      Math::Normalize(&vH);

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        const floatf fDist = Math::TSqrt(fDistSq);
        const floatf fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        const floatf fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vColor;
    VECTOR3 vFrontDS = pLight->vScaledAmbient;
    VECTOR3 vBackDS = vFrontDS;

    const floatf fDotNL = Math::TDot<floatf>(vNormal, vL);
    const floatf fDiffFactor = (fDotNL > fZERO) ? fDotNL : -fDotNL;

    const VECTOR4 &vLightDiffuse = pLight->GetColor(GL_DIFFUSE);
    vColor.x = vVertexColor.x * vLightDiffuse.x * fDiffFactor;
    vColor.y = vVertexColor.y * vLightDiffuse.y * fDiffFactor;
    vColor.z = vVertexColor.z * vLightDiffuse.z * fDiffFactor;

    floatf fDotNH = Math::TDot<floatf>(vNormal, vH);
    if (fDotNL <= fZERO) {
      fDotNH = -fDotNH;
    }

    if (fDotNH > fZERO) {
      // Compute the specular coefficient
      const floatf fCoeff = Math::TPow(fDotNH, m_fMatShininess);
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
  for (const Light *pLight = m_pActiveLights; pLight; pLight = pLight->pNext) {
    VECTOR3 vL;         // Light vector
    VECTOR3 vH;         // Halfway vector
    floatf fAtt = fONE; // Attenuation distance

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
      const floatf fDistSq = Math::TDot<floatf>(vL, vL);
      if (!Math::TIsZero(fDistSq - fONE)) {
        const floatf fInvDist = Math::TInvSqrt(fDistSq);
        vL.x *= fInvDist;
        vL.y *= fInvDist;
        vL.z *= fInvDist;
      }

      // Compute the halfway vector
      const VECTOR3 vDir(fZERO, fZERO, fONE);
      Math::Add(&vH, vL, vDir);
      Math::Normalize(&vH);

      // Compute the distance attenuation
      if (pLight->Flags.Attenuation) {
        const floatf fDist = Math::TSqrt(fDistSq);
        const floatf fInvAtt =
            pLight->GetAttenuation(GL_QUADRATIC_ATTENUATION) * fDistSq +
            pLight->GetAttenuation(GL_LINEAR_ATTENUATION) * fDist +
            pLight->GetAttenuation(GL_CONSTANT_ATTENUATION);
        if (!Math::TIsZero(fInvAtt - fONE)) {
          fAtt = Math::TInv<floatf>(fInvAtt);
        }
      }

      // Compute the spot light attenuation
      if (pLight->Flags.SpotLight) {
        const floatf fCos = -Math::TDot<floatf>(vL, pLight->vSpotDirection);
        if (fCos >= pLight->fSpotCutOffCos) {
          fAtt *= Math::TPow(fCos, pLight->fSpotExponent);
        } else {
          continue;
        }
      }
    }

    VECTOR3 vColor;
    VECTOR3 vFrontDS = pLight->vScaledAmbient;
    VECTOR3 vBackDS = vFrontDS;

    const floatf fDotNL = Math::TDot<floatf>(vNormal, vL);
    const floatf fDiffFactor = (fDotNL > fZERO) ? fDotNL : -fDotNL;

    vColor.x = pLight->vScaledDiffuse.x * fDiffFactor;
    vColor.y = pLight->vScaledDiffuse.y * fDiffFactor;
    vColor.z = pLight->vScaledDiffuse.z * fDiffFactor;

    floatf fDotNH = Math::TDot<floatf>(vNormal, vH);
    if (fDotNL <= fZERO) {
      fDotNH = -fDotNH;
    }

    if (fDotNH > fZERO) {
      // Compute the specular coefficient
      const floatf fCoeff = Math::TPow(fDotNH, m_fMatShininess);
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

void CTNL::ProcessTexCoords(unsigned dstIndex, unsigned srcIndex,
                            unsigned count) {
  TEXCOORD2 *const pvTexCoords =
      (TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + dstIndex];

  VECTOR2 vIn = m_vTexCoords[srcIndex];

  if (!m_pMsTexCoords[srcIndex]->IsIdentity()) {
    Math::Mul(&vIn, vIn, m_pMsTexCoords[srcIndex]->GetMatrix());
  }

  for (unsigned k = 0; k < count; ++k) {
    pvTexCoords[k].m[0] = Math::TCast<float20>(vIn.x);
    pvTexCoords[k].m[1] = Math::TCast<float20>(vIn.y);
  }
}

GLenum CTNL::UpdatePoints(uint8_t **ppbVertexData, int first, unsigned count) {
  GLenum err;

  if (m_vertexStates.PointSize) {
    err = m_pointSizeArray.Prepare(&m_pointSizeDecode, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("VertexArray::Prepare() failed, err = %d.\r\n"), err);
      return err;
    }
  } else {
    m_pointSizeDecode.pBits = nullptr;
  }

  if (!Math::TIsZero(m_pointParams.vAttenuation.y) ||
      !Math::TIsZero(m_pointParams.vAttenuation.z)) {
    m_TNLFlags.PointSizeQAttn = 1;
    m_TNLFlags.EyeSpaceZ = 1;
  }

  m_pbVertexData[VERTEXDATA_POINTSIZE] = *ppbVertexData;
  *ppbVertexData = __alignPtr(
      *ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(fixed4), 4);

  if (m_pointSizeDecode.pBits) {
    unsigned uiFunc = (m_pointSizeArray.Format - VERTEX_FIXED) / 4;

    if (m_TNLFlags.PointSizeQAttn) {
      uiFunc += 2;
    }

    ASSERT(uiFunc < __countof(g_processPointSize));
    m_pfnPointSize = g_processPointSize[uiFunc];
  } else {
    m_pfnPointSize = &CTNL::ProcessPointSize;
  }

  m_TNLFlags.PointSize = 1;

  return GL_NO_ERROR;
}

GLenum CTNL::UpdateColor(uint8_t **ppbVertexData, int first, unsigned count) {
  GLenum err;

  if (m_vertexStates.Color) {
    err = m_colorArray.Prepare(&m_colorDecode, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("VertexArray::Prepare() failed, err = %d.\r\n"), err);
      return err;
    }
  } else {
    m_colorDecode.pBits = nullptr;
  }

  m_pbVertexData[VERTEXDATA_FRONTCOLOR] = *ppbVertexData;
  m_pbVertexColor = *ppbVertexData;
  *ppbVertexData = __alignPtr(
      *ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(ColorARGB), 4);

  m_cullStates.bTwoSidedLighting = false;

  if (m_caps.Lighting) {
    err = this->UpdateLighting(ppbVertexData, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("CTNL::UpdateLighting() failed, err = %d.\r\n"), err);
      return err;
    }
  } else {
    if (m_colorDecode.pBits) {
      const unsigned uiFunc = (m_colorArray.Format - VERTEX_FIXED4) / 4;
      ASSERT(uiFunc < __countof(g_processVertexColor));
      m_pfnColor = g_processVertexColor[uiFunc];
    } else {
      m_pfnColor = &CTNL::ProcessColor;
    }
  }

  m_TNLFlags.Color = 1;

  return GL_NO_ERROR;
}

GLenum CTNL::UpdateLighting(uint8_t **ppbVertexData, int first,
                            unsigned count) {
  GLenum err;

  if (m_dirtyFlags.ModelViewInvT33) {
    this->UpdateModelViewInvT33();
  }

  if (m_vertexStates.Normal) {
    err = m_normalArray.Prepare(&m_normalDecode, first, count);
    if (__glFailed(err)) {
      __glLogError(_T("VertexArray::Prepare() failed, err = %d.\r\n"), err);
      return err;
    }
  } else {
    m_normalDecode.pBits = nullptr;
  }

  if (m_normalDecode.pBits) {
    m_TNLFlags.Normalize = m_caps.Normalize;
  } else {
    if (m_dirtyFlags.NormalizeNormal) {
      this->UpdateNormal();
    }
  }

  if (m_dirtyFlags.ScaledAmbient) {
    this->UpdateMaterial();
  }

  if (m_dirtyFlags.Lights) {
    this->SetupLights();
  }

  if (m_pActiveLights) {
    m_TNLFlags.EyeSpace = 1;

    if (m_dirtyLights.Ambient || m_dirtyLights.Diffuse ||
        m_dirtyLights.Specular) {
      this->UpdateLights();
    }
  }

  unsigned uiFunc = 0;

  if (m_normalDecode.pBits) {
    uiFunc = 1 + (m_normalArray.Format - VERTEX_BYTE3) / 4;
  }

  if (m_caps.ColorMaterial) {
    unsigned _uiFunc = 1;

    if (m_colorDecode.pBits) {
      _uiFunc += 1 + (m_normalArray.Format - VERTEX_FIXED4) / 4;
    }

    uiFunc += _uiFunc * 5;
  }

  if (m_caps.TwoSidedLighting) {
    m_cullStates.bTwoSidedLighting = true;

    m_pbVertexData[VERTEXDATA_BACKCOLOR] = *ppbVertexData;
    *ppbVertexData = __alignPtr(
        *ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(ColorARGB), 4);

    uiFunc += 5 * 5;
  }

  ASSERT(uiFunc < __countof(g_processLighting));
  m_pfnColor = g_processLighting[uiFunc];

  return GL_NO_ERROR;
}

GLenum CTNL::UpdateTexcoords(uint8_t **ppbVertexData, int first,
                             unsigned count) {
  GLenum err;

  for (unsigned i = 0, j = 0, mask = m_caps.Texture2D; mask; mask >>= 1, ++i) {
    if (mask & 0x1) {
      if ((m_vertexStates.TexCoords >> i) & 0x1) {
        err = m_texCoordArrays[i].Prepare(&m_texCoordDecodes[i], first, count);
        if (__glFailed(err)) {
          __glLogError(_T("VertexArray::Prepare() failed, err = %d.\r\n"), err);
          return err;
        }
      } else {
        m_texCoordDecodes[i].pBits = nullptr;
      }

      m_pbVertexData[VERTEXDATA_TEXCOORD0 + j++] = *ppbVertexData;
      *ppbVertexData = __alignPtr(
          *ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(TEXCOORD2), 4);

      if (m_texCoordDecodes[i].pBits) {
        const unsigned uiFormatType =
            (m_texCoordArrays[i].Format - VERTEX_BYTE2) / 4;
        const unsigned uiFormatSize =
            m_texCoordArrays[i].Format - 1 - uiFormatType * 4;
        unsigned uiFunc = uiFormatType * 3 + uiFormatSize - 1;

        if (!m_pMsTexCoords[i]->IsIdentity()) {
          uiFunc += 12;
        }

        ASSERT(uiFunc < __countof(g_processTexcoords));
        m_pfnTexCoords[i] = g_processTexcoords[uiFunc];
      } else {
        m_pfnTexCoords[i] = &CTNL::ProcessTexCoords;
      }
    }
  }

  m_TNLFlags.TexCoords = m_caps.Texture2D;

  return GL_NO_ERROR;
}

void CTNL::UpdateFog(uint8_t **ppbVertexData, int /*first*/, unsigned count) {
  m_TNLFlags.Fog = 1;
  m_TNLFlags.EyeSpaceZ = 1;

  if (m_dirtyFlags.FogRatio) {
    const floatf fFogStart = m_fog.GetFactor(GL_FOG_START);
    const floatf fFogEnd = m_fog.GetFactor(GL_FOG_END);

    if (fFogStart != fFogEnd) {
      m_fog.fRatio = Math::TInv<fixedRF>(fFogEnd - fFogStart);
    } else {
      m_fog.fRatio = TConst<fixedRF>::Zero();
    }

    m_dirtyFlags.FogRatio = 0;
  }

  m_pbVertexData[VERTEXDATA_FOG] = *ppbVertexData;
  *ppbVertexData = __alignPtr(
      *ppbVertexData + (count + CLIP_BUFFER_SIZE) * sizeof(float20), 4);

  const unsigned uiFunc = m_fog.Mode;
  ASSERT(uiFunc < __countof(g_processFog));
  m_pfnFog = g_processFog[uiFunc];
}

void CTNL::UpdateModelViewInvT44() {
  MATRIX44 matTmp;
  Math::Inverse(&matTmp, m_pMsModelView->GetMatrix());
  Math::Transpose(&m_mModelViewInvT, matTmp);
  m_dirtyFlags.ModelViewInvT44 = 0;

  if (!m_caps.RescaleNormal) {
    m_dirtyFlags.ModelViewInvT33 = 0;
    m_dirtyFlags.NormalizeNormal = 1;
  }
}

void CTNL::UpdateModelViewProj() {
  Math::Mul(&m_mModelViewProj, m_pMsProjection->GetMatrix(),
            m_pMsModelView->GetMatrix());

  m_dirtyFlags.ModelViewProj = 0;
}

void CTNL::UpdateScreenXform() {
  m_screenXform.fMinX =
      static_cast<fixed4>((m_viewport.left + m_viewport.right) >> 1);
  m_screenXform.iScaleX = (m_viewport.right - m_viewport.left) >> 1;

  m_screenXform.fMinY =
      static_cast<fixed4>((m_viewport.top + m_viewport.bottom) >> 1);
  m_screenXform.iScaleY = (m_viewport.bottom - m_viewport.top) >> 1;

  m_screenXform.fMinZ =
      static_cast<float20>((m_depthRange.fNear + m_depthRange.fFar) / 2);
  m_screenXform.fScaleZ = (m_depthRange.fFar - m_depthRange.fNear) / 2;

  m_dirtyFlags.ScreenXform = 0;
}

void CTNL::UpdateClipPlanes() {
  m_TNLFlags.UserClipPlanes = 1;

  if (m_dirtyFlags.ClipPlanesCS) {
    if (m_dirtyFlags.ProjectionInvT) {
      this->UpdateProjectionInvT();
    }

    unsigned updateMask = m_dirtyFlags.ClipPlanesCS & m_caps.ClipPlanes;
    for (unsigned i = 0; updateMask; ++i, updateMask >>= 1) {
      if (updateMask & 1) {
        ASSERT(i < m_vClipPlanesES.GetSize());
        ASSERT(i < m_vClipPlanesCS.GetSize());
        Math::Mul(&m_vClipPlanesCS[i], m_vClipPlanesES[i], m_mProjectionInvT);
      }
    }

    m_dirtyFlags.ClipPlanesCS = 0;
  }
}

void CTNL::UpdateProjectionInvT() {
  MATRIX44 matTmp;
  Math::Inverse(&matTmp, m_pMsProjection->GetMatrix());
  Math::Transpose(&m_mProjectionInvT, matTmp);

  m_dirtyFlags.ProjectionInvT = 0;
}

void CTNL::UpdateModelViewInvT33() {
  MATRIX44 matTmp;
  Math::Inverse33(&matTmp, m_pMsModelView->GetMatrix());

  if (m_caps.RescaleNormal) {
    const floatf fSum = (matTmp._31 * matTmp._31) + (matTmp._32 * matTmp._32) +
                        (matTmp._33 * matTmp._33);

    if (!Math::TIsZero(fSum - fONE)) {
      const floatf fFactor = Math::TInvSqrt(fSum);

      m_mModelViewInvT._11 = matTmp._11 * fFactor;
      m_mModelViewInvT._21 = matTmp._21 * fFactor;
      m_mModelViewInvT._31 = matTmp._31 * fFactor;

      m_mModelViewInvT._12 = matTmp._12 * fFactor;
      m_mModelViewInvT._22 = matTmp._22 * fFactor;
      m_mModelViewInvT._32 = matTmp._32 * fFactor;

      m_mModelViewInvT._13 = matTmp._13 * fFactor;
      m_mModelViewInvT._23 = matTmp._23 * fFactor;
      m_mModelViewInvT._33 = matTmp._33 * fFactor;
    }
  } else {
    m_mModelViewInvT = matTmp;
  }

  m_dirtyFlags.ModelViewInvT33 = 0;
  m_dirtyFlags.NormalizeNormal = 1;
}

void CTNL::UpdateNormal() {
  // Transform the normal to world space
  Math::Mul(&m_vNormNormal, m_vNormal, m_mModelViewInvT);

  // Normalize the normal
  Math::Normalize(&m_vNormNormal);

  m_dirtyFlags.NormalizeNormal = 0;
}

void CTNL::UpdateMaterial() {
  const VECTOR4 &vMaterialAmbient = m_material.GetColor(GL_AMBIENT);
  const VECTOR4 &vMaterialDiffuse = m_material.GetColor(GL_DIFFUSE);

  m_vScaledAmbient.x = vMaterialAmbient.x * m_vLightModelAmbient.x;
  m_vScaledAmbient.y = vMaterialAmbient.y * m_vLightModelAmbient.y;
  m_vScaledAmbient.z = vMaterialAmbient.z * m_vLightModelAmbient.z;
  m_vScaledAmbient.w = vMaterialDiffuse.w;

  m_dirtyFlags.ScaledAmbient = 0;
}

void CTNL::SetupLights() {
  m_pActiveLights = nullptr;
  Light *pLight = nullptr;

  for (unsigned i = 0, activeMask = m_caps.Lights; activeMask;
       ++i, activeMask >>= 1) {
    if (activeMask & 1) {
      if (nullptr == pLight) {
        m_pActiveLights = pLight = &m_lights[i];
      } else {
        pLight->pNext = &m_lights[i];
        pLight = pLight->pNext;
      }

      pLight->pNext = nullptr;
    }
  }

  m_dirtyFlags.Lights = 0;
}

void CTNL::UpdateLights() {
  const VECTOR4 &vMaterialAmbient = m_material.GetColor(GL_AMBIENT);
  const VECTOR4 &vMaterialDiffuse = m_material.GetColor(GL_DIFFUSE);
  const VECTOR4 &vMaterialSpecular = m_material.GetColor(GL_SPECULAR);

  const DirtyLights dirtyLights = m_dirtyLights;

  for (Light *pLight = m_pActiveLights; pLight; pLight = pLight->pNext) {
    const unsigned mask = 1 << (pLight - m_lights.GetBegin());

    if (dirtyLights.Ambient & mask) {
      const VECTOR4 &vLightAmbient = pLight->GetColor(GL_AMBIENT);
      pLight->vScaledAmbient.x = vLightAmbient.x * vMaterialAmbient.x;
      pLight->vScaledAmbient.y = vLightAmbient.y * vMaterialAmbient.y;
      pLight->vScaledAmbient.z = vLightAmbient.z * vMaterialAmbient.z;
    }

    if (dirtyLights.Diffuse & mask) {
      const VECTOR4 &vLightDiffuse = pLight->GetColor(GL_DIFFUSE);
      pLight->vScaledDiffuse.x = vMaterialDiffuse.x * vLightDiffuse.x;
      pLight->vScaledDiffuse.y = vMaterialDiffuse.y * vLightDiffuse.y;
      pLight->vScaledDiffuse.z = vMaterialDiffuse.z * vLightDiffuse.z;
    }

    if (dirtyLights.Specular & mask) {
      const VECTOR4 &vLightSpecular = pLight->GetColor(GL_SPECULAR);
      pLight->vScaledSpecular.x = vMaterialSpecular.x * vLightSpecular.x;
      pLight->vScaledSpecular.y = vMaterialSpecular.y * vLightSpecular.y;
      pLight->vScaledSpecular.z = vMaterialSpecular.z * vLightSpecular.z;
    }

    if (dirtyLights.Position & mask) {
      if (pLight->Flags.DirectionalLight) {
        // Normalize the position
        Math::Normalize(reinterpret_cast<VECTOR3 *>(&pLight->vPosition));

        // Compute the halfway vector
        const VECTOR3 vDir(fZERO, fZERO, fONE);
        Math::Add(&pLight->vHalfway,
                  reinterpret_cast<const VECTOR3 &>(pLight->vPosition), vDir);

        // Normalize the halfway vector
        Math::Normalize(&pLight->vHalfway);
      } else {
        if (Math::TIsZero(pLight->GetAttenuation(GL_CONSTANT_ATTENUATION) -
                          fONE) &&
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

  m_dirtyLights.Value = 0;
}

void CTNL::UpdateMatrixDirtyFlags() {
  if (m_pMatrixStack == m_pMsModelView) {
    m_dirtyFlags.ModelViewInvT44 = 1;
    m_dirtyFlags.ModelViewInvT33 = 1;
    m_dirtyFlags.ModelViewProj = 1;
  } else if (m_pMatrixStack == m_pMsProjection) {
    m_dirtyFlags.ModelViewProj = 1;
    m_dirtyFlags.ProjectionInvT = 1;
    m_dirtyFlags.ClipPlanesCS = CLIPPLANES_MASK;
  }
}
