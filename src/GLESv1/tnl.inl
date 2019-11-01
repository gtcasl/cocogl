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
#pragma once

template <eVertexFormat> struct TVertexData {};

template <> struct TVertexData<VERTEX_UNKNOWN> {};

template <> struct TVertexData<VERTEX_BYTE> {
  typedef TVector1<char> Input;
  typedef VECTOR1 Output;
};
template <> struct TVertexData<VERTEX_BYTE2> {
  typedef TVector2<char> Input;
  typedef VECTOR2 Output;
};
template <> struct TVertexData<VERTEX_BYTE3> {
  typedef TVector3<char> Input;
  typedef VECTOR3 Output;
};
template <> struct TVertexData<VERTEX_BYTE4> {
  typedef TVector4<char> Input;
  typedef VECTOR4 Output;
};

template <> struct TVertexData<VERTEX_SHORT> {
  typedef TVector1<short> Input;
  typedef VECTOR1 Output;
};
template <> struct TVertexData<VERTEX_SHORT2> {
  typedef TVector2<short> Input;
  typedef VECTOR2 Output;
};
template <> struct TVertexData<VERTEX_SHORT3> {
  typedef TVector3<short> Input;
  typedef VECTOR3 Output;
};
template <> struct TVertexData<VERTEX_SHORT4> {
  typedef TVector4<short> Input;
  typedef VECTOR4 Output;
};

template <> struct TVertexData<VERTEX_FIXED> {
  typedef TVector1<fixed16> Input;
  typedef VECTOR1 Output;
};
template <> struct TVertexData<VERTEX_FIXED2> {
  typedef TVector2<fixed16> Input;
  typedef VECTOR2 Output;
};
template <> struct TVertexData<VERTEX_FIXED3> {
  typedef TVector3<fixed16> Input;
  typedef VECTOR3 Output;
};
template <> struct TVertexData<VERTEX_FIXED4> {
  typedef TVector4<fixed16> Input;
  typedef VECTOR4 Output;
};

template <> struct TVertexData<VERTEX_FLOAT> {
  typedef TVector1<float> Input;
  typedef VECTOR1 Output;
};
template <> struct TVertexData<VERTEX_FLOAT2> {
  typedef TVector2<float> Input;
  typedef VECTOR2 Output;
};
template <> struct TVertexData<VERTEX_FLOAT3> {
  typedef TVector3<float> Input;
  typedef VECTOR3 Output;
};
template <> struct TVertexData<VERTEX_FLOAT4> {
  typedef TVector4<float> Input;
  typedef VECTOR4 Output;
};

template <> struct TVertexData<VERTEX_RGBA> {
  typedef TVector4<uint8_t> Input;
  typedef VECTOR4 Output;
};

template <class D, class T>
inline void TDecodeVertex(D *pOut, const uint8_t *pbIn) {
  ASSERT(pOut);
  ASSERT(pbIn);

  const typename T::Input *const pIn =
      reinterpret_cast<const typename T::Input *>(pbIn);

  if
    constexpr(T::Input::DIM >= 1) { pOut->x = Math::TCast<floatf>(pIn->x); }
  else {
    if
      constexpr(D::DIM >= 1) { pOut->x = fZERO; }
  }

  if
    constexpr(T::Input::DIM >= 2) { pOut->y = Math::TCast<floatf>(pIn->y); }
  else {
    if
      constexpr(D::DIM >= 2) { pOut->y = fZERO; }
  }

  if
    constexpr(T::Input::DIM >= 3) { pOut->z = Math::TCast<floatf>(pIn->z); }
  else {
    if
      constexpr(D::DIM >= 3) { pOut->z = fZERO; }
  }

  if
    constexpr(T::Input::DIM >= 4) { pOut->w = Math::TCast<floatf>(pIn->w); }
  else {
    if
      constexpr(D::DIM >= 4) { pOut->w = fONE; }
  }
}

template <>
inline void
TDecodeVertex<VECTOR4, TVertexData<VERTEX_UNKNOWN>>(VECTOR4 * /*pOut*/,
                                                    const uint8_t * /*pbIn*/) {
  //--
}

template <>
inline void
TDecodeVertex<VECTOR4, TVertexData<VERTEX_RGBA>>(VECTOR4 *pOut,
                                                 const uint8_t *pbIn) {
  ASSERT(pOut);
  ASSERT(pbIn);

  pOut->x = Math::TFromUNORM8<floatf>(pbIn[0]);
  pOut->y = Math::TFromUNORM8<floatf>(pbIn[1]);
  pOut->z = Math::TFromUNORM8<floatf>(pbIn[2]);
  pOut->w = Math::TFromUNORM8<floatf>(pbIn[3]);
}

//////////////////////////////////////////////////////////////////////////////

template <eVertexFormat VertexFormat>
void TDecodePosition(VECTOR4 *pOut, const uint8_t *pbIn, unsigned stride,
                     unsigned count) {
  for (unsigned i = 0; i < count; ++i) {
    TDecodeVertex<VECTOR4, TVertexData<VertexFormat>>(&pOut[i],
                                                      pbIn + i * stride);
  }
}

//////////////////////////////////////////////////////////////////////////////

template <bool ColorMaterial, eVertexFormat ColorFormat,
          eVertexFormat NormalFormat>
inline void CTNL::TProcessLighting_OneSided(unsigned count) {

  VECTOR3 *const pvEyePos = (VECTOR3 *)m_pbVertexData[VERTEXDATA_EYEPOS];
  ColorARGB *const pcFrontColors =
      (ColorARGB *)m_pbVertexData[VERTEXDATA_FRONTCOLOR];

  VECTOR3 vNormal;
  VECTOR4 vVertexColor;
  const uint8_t *pbNormal = nullptr;
  unsigned normalStride = 0;
  const uint8_t *pbColor = nullptr;
  unsigned colorStride = 0;

  if
    constexpr(NormalFormat != VERTEX_UNKNOWN) {
      pbNormal = m_normalDecode.pBits;
      normalStride = m_normalDecode.Stride;
    }
  else {
    vNormal = m_vNormNormal;
  }

  if
    constexpr(ColorFormat != VERTEX_UNKNOWN) {
      pbColor = m_colorDecode.pBits;
      colorStride = m_colorDecode.Stride;
    }
  else {
    vVertexColor.x = m_vColor.x * m_vLightModelAmbient.x;
    vVertexColor.y = m_vColor.y * m_vLightModelAmbient.y;
    vVertexColor.z = m_vColor.z * m_vLightModelAmbient.z;
    vVertexColor.w = m_vColor.w;
  }

  for (unsigned i = 0; i < count; ++i) {
    VECTOR4 vResult;

    if
      constexpr(NormalFormat != VERTEX_UNKNOWN) {
        TDecodeVertex<VECTOR3, TVertexData<NormalFormat>>(
            &vNormal, pbNormal + i * normalStride);

        // Transform the normal to world space
        Math::Mul(&vNormal, vNormal, m_mModelViewInvT);

        // Normalize the normal
        if (m_TNLFlags.Normalize) {
          Math::Normalize(&vNormal);
        }
      }

    if
      constexpr(ColorMaterial) {
        if
          constexpr(ColorFormat != VERTEX_UNKNOWN) {
            TDecodeVertex<VECTOR4, TVertexData<ColorFormat>>(
                &vVertexColor, pbColor + i * colorStride);
            vVertexColor.x *= m_vLightModelAmbient.x;
            vVertexColor.y *= m_vLightModelAmbient.y;
            vVertexColor.z *= m_vLightModelAmbient.z;
          }

        vResult = vVertexColor;
      }
    else {
      vResult = m_vScaledAmbient;
    }

    vResult.x += m_vMatEmissive.x;
    vResult.y += m_vMatEmissive.y;
    vResult.z += m_vMatEmissive.z;

    // Apply lights components
    if
      constexpr(ColorMaterial) {
        this->ProcessLights_OneSided(&vResult, pvEyePos[i], vNormal,
                                     vVertexColor);
      }
    else {
      this->ProcessLights_OneSided(&vResult, pvEyePos[i], vNormal);
    }

    // Clamp the front color
    vResult.x = Math::TSat(vResult.x);
    vResult.y = Math::TSat(vResult.y);
    vResult.z = Math::TSat(vResult.z);
    vResult.w = Math::TSat(vResult.w);

    // Set the front color
    pcFrontColors[i].r = static_cast<uint8_t>(Math::TToUNORM8(vResult.x));
    pcFrontColors[i].g = static_cast<uint8_t>(Math::TToUNORM8(vResult.y));
    pcFrontColors[i].b = static_cast<uint8_t>(Math::TToUNORM8(vResult.z));
    pcFrontColors[i].a = static_cast<uint8_t>(Math::TToUNORM8(vResult.w));
  }
}

template <bool ColorMaterial, eVertexFormat ColorFormat,
          eVertexFormat NormalFormat>
inline void CTNL::TProcessLighting_TwoSided(unsigned count) {

  VECTOR3 *const pvEyePos = (VECTOR3 *)m_pbVertexData[VERTEXDATA_EYEPOS];
  ColorARGB *const pcFrontColors =
      (ColorARGB *)m_pbVertexData[VERTEXDATA_FRONTCOLOR];
  ColorARGB *const pcBackColors =
      (ColorARGB *)m_pbVertexData[VERTEXDATA_BACKCOLOR];

  VECTOR3 vNormal;
  VECTOR4 vVertexColor;
  const uint8_t *pbNormal;
  unsigned normalStride;
  const uint8_t *pbColor;
  unsigned colorStride;

  if
    constexpr(NormalFormat != VERTEX_UNKNOWN) {
      pbNormal = m_normalDecode.pBits;
      normalStride = m_normalDecode.Stride;
    }
  else {
    vNormal = m_vNormNormal;
  }

  if
    constexpr(ColorFormat != VERTEX_UNKNOWN) {
      pbColor = m_colorDecode.pBits;
      colorStride = m_colorDecode.Stride;
    }
  else {
    vVertexColor.x = m_vColor.x * m_vLightModelAmbient.x;
    vVertexColor.y = m_vColor.y * m_vLightModelAmbient.y;
    vVertexColor.z = m_vColor.z * m_vLightModelAmbient.z;
    vVertexColor.w = m_vColor.w;
  }

  for (unsigned i = 0; i < count; ++i) {
    VECTOR4 vResults[2];

    if
      constexpr(NormalFormat != VERTEX_UNKNOWN) {
        TDecodeVertex<VECTOR3, TVertexData<NormalFormat>>(
            &vNormal, pbNormal + i * normalStride);

        // Transform the normal to world space
        Math::Mul(&vNormal, vNormal, m_mModelViewInvT);

        // Normalize the normal
        if (m_TNLFlags.Normalize) {
          Math::Normalize(&vNormal);
        }
      }

    if
      constexpr(ColorMaterial) {
        if
          constexpr(ColorFormat != VERTEX_UNKNOWN) {
            TDecodeVertex<VECTOR4, TVertexData<ColorFormat>>(
                &vVertexColor, pbColor + i * colorStride);
            vVertexColor.x *= m_vLightModelAmbient.x;
            vVertexColor.y *= m_vLightModelAmbient.y;
            vVertexColor.z *= m_vLightModelAmbient.z;
          }

        vResults[0] = vVertexColor;
      }
    else {
      vResults[0] = m_vScaledAmbient;
    }

    vResults[0].x += m_vMatEmissive.x;
    vResults[0].y += m_vMatEmissive.y;
    vResults[0].z += m_vMatEmissive.z;

    vResults[1] = vResults[0];

    // Apply lights components
    if
      constexpr(ColorMaterial) {
        this->ProcessLights_TwoSided(vResults, pvEyePos[i], vNormal,
                                     vVertexColor);
      }
    else {
      this->ProcessLights_TwoSided(vResults, pvEyePos[i], vNormal);
    }

    // Clamp the front color
    vResults[0].x = Math::TSat(vResults[0].x);
    vResults[0].y = Math::TSat(vResults[0].y);
    vResults[0].z = Math::TSat(vResults[0].z);
    vResults[0].w = Math::TSat(vResults[0].w);

    // Clamp the back color
    vResults[1].x = Math::TSat(vResults[1].x);
    vResults[1].y = Math::TSat(vResults[1].y);
    vResults[1].z = Math::TSat(vResults[1].z);
    vResults[1].w = Math::TSat(vResults[1].w);

    // Set the front color
    pcFrontColors[i].r = static_cast<uint8_t>(Math::TToUNORM8(vResults[0].x));
    pcFrontColors[i].g = static_cast<uint8_t>(Math::TToUNORM8(vResults[0].y));
    pcFrontColors[i].b = static_cast<uint8_t>(Math::TToUNORM8(vResults[0].z));
    pcFrontColors[i].a = static_cast<uint8_t>(Math::TToUNORM8(vResults[0].w));

    // Set the back color
    pcBackColors[i].r = static_cast<uint8_t>(Math::TToUNORM8(vResults[1].x));
    pcBackColors[i].g = static_cast<uint8_t>(Math::TToUNORM8(vResults[1].y));
    pcBackColors[i].b = static_cast<uint8_t>(Math::TToUNORM8(vResults[1].z));
    pcBackColors[i].a = static_cast<uint8_t>(Math::TToUNORM8(vResults[1].w));
  }
}

template <eVertexFormat VertexFormat>
void CTNL::TProcessVertexColor(unsigned count) {
  ColorARGB *const pcFrontColors =
      (ColorARGB *)m_pbVertexData[VERTEXDATA_FRONTCOLOR];

  const uint8_t *const pbIn = m_colorDecode.pBits;
  const unsigned stride = m_colorDecode.Stride;

  for (unsigned i = 0; i < count; ++i) {
    VECTOR4 vColor;

    TDecodeVertex<VECTOR4, TVertexData<VertexFormat>>(&vColor,
                                                      pbIn + i * stride);

    // Clamp the color
    vColor.x = Math::TSat(vColor.x);
    vColor.y = Math::TSat(vColor.y);
    vColor.z = Math::TSat(vColor.z);
    vColor.w = Math::TSat(vColor.w);

    // Set the front color
    pcFrontColors[i].r = static_cast<uint8_t>(Math::TToUNORM8(vColor.x));
    pcFrontColors[i].g = static_cast<uint8_t>(Math::TToUNORM8(vColor.y));
    pcFrontColors[i].b = static_cast<uint8_t>(Math::TToUNORM8(vColor.z));
    pcFrontColors[i].a = static_cast<uint8_t>(Math::TToUNORM8(vColor.w));
  }
}

template <bool Transform, eVertexFormat VertexFormat>
void CTNL::TProcessTexCoords(unsigned dstIndex, unsigned srcIndex,
                             unsigned count) {
  TEXCOORD2 *const pvTexCoords =
      (TEXCOORD2 *)m_pbVertexData[VERTEXDATA_TEXCOORD0 + dstIndex];

  const MATRIX44 &transform = m_pMsTexCoords[srcIndex]->GetMatrix();

  const uint8_t *const pbIn = m_texCoordDecodes[srcIndex].pBits;
  const unsigned stride = m_texCoordDecodes[srcIndex].Stride;

  for (unsigned i = 0; i < count; ++i) {
    typename TVertexData<VertexFormat>::Output vIn;

    TDecodeVertex<typename TVertexData<VertexFormat>::Output,
                  TVertexData<VertexFormat>>(&vIn, pbIn + i * stride);

    if
      constexpr(Transform) { Math::Mul(&vIn, vIn, transform); }

    if
      constexpr(TVertexData<VertexFormat>::Output::DIM >= 4) {
        if (!Math::TIsZero(vIn.w - fONE) && !Math::TIsZero(vIn.w)) {
          const floatf fInvW = Math::TInv<floatf>(vIn.w);
          vIn.x *= fInvW;
          vIn.y *= fInvW;
        }
      }

    pvTexCoords[i].m[0] = Math::TCast<float20>(vIn.x);
    pvTexCoords[i].m[1] = Math::TCast<float20>(vIn.y);
  }
}

template <bool QuadraticAttenuation, eVertexFormat VertexFormat>
void CTNL::TProcessPointSize(unsigned count) {
  fixed4 *const pfPointSizes = (fixed4 *)m_pbVertexData[VERTEXDATA_POINTSIZE];

  const VECTOR3 &vAttenuation = m_pointParams.vAttenuation;
  const VECTOR3 *pvEyePos;
  floatf fInvAtt;

  if
    constexpr(QuadraticAttenuation) {
      pvEyePos = (VECTOR3 *)m_pbVertexData[VERTEXDATA_EYEPOS];
    }
  else {
    fInvAtt = vAttenuation.x;
    if (!Math::TIsZero(fInvAtt - fONE)) {
      fInvAtt = Math::TInvSqrt(fInvAtt);
    }
  }

  const uint8_t *const pbIn = m_pointSizeDecode.pBits;
  const unsigned stride = m_pointSizeDecode.Stride;

  for (unsigned i = 0; i < count; ++i) {
    VECTOR1 vPointSize;

    TDecodeVertex<VECTOR1, TVertexData<VertexFormat>>(&vPointSize,
                                                      pbIn + i * stride);

    if
      constexpr(QuadraticAttenuation) {
        const floatf fEyeDist = Math::TAbs(pvEyePos[i].z);
        const floatf fInvAtt = vAttenuation.z * fEyeDist * fEyeDist +
                               vAttenuation.y * fEyeDist + vAttenuation.x;
        if (!Math::TIsZero(fInvAtt - fONE)) {
          vPointSize.x *= Math::TInvSqrt(fInvAtt);
        }
      }
    else {
      vPointSize.x *= fInvAtt;
    }

    pfPointSizes[i] =
        static_cast<fixed4>(Math::TMax<floatf>(vPointSize.x, fONE));
  }
}

template <eFogMode FogMode> void CTNL::TProcessFog(unsigned count) {

  VECTOR3 *const pvEyePos = (VECTOR3 *)m_pbVertexData[VERTEXDATA_EYEPOS];
  float20 *const pfFogs = (float20 *)m_pbVertexData[VERTEXDATA_FOG];

  if
    constexpr(FogMode == FogLinear) {
      const floatf fFogEnd = m_fog.GetFactor(GL_FOG_END);
      const fixedRF fFogRatio = m_fog.fRatio;

      for (unsigned i = 0; i < count; ++i) {
        const floatf fEyeDist = Math::TAbs(pvEyePos[i].z);
        pfFogs[i] = Math::TSat(fFogRatio * (fFogEnd - fEyeDist));
      }
    }
  if
    constexpr(FogMode == FogExp) {
      const floatf fFogDensity = m_fog.GetFactor(GL_FOG_DENSITY);

      for (unsigned i = 0; i < count; ++i) {
        const floatf fEyeDist = Math::TAbs(pvEyePos[i].z);
        floatf fTmp = fEyeDist * fFogDensity;
        pfFogs[i] = static_cast<fixedRF>(Math::TSat(Math::TExp(-fTmp)));
      }
    }
  if
    constexpr(FogMode == FogExp2) {
      const floatf fFogDensity = m_fog.GetFactor(GL_FOG_DENSITY);

      for (unsigned i = 0; i < count; ++i) {
        const floatf fEyeDist = Math::TAbs(pvEyePos[i].z);
        floatf fTmp = fEyeDist * fFogDensity;
        pfFogs[i] =
            static_cast<fixedRF>(Math::TSat(Math::TExp(-(fTmp * fTmp))));
      }
    }
}