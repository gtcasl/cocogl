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

template <typename D, typename T>
inline void TDecodeVertex(D *pOut, const uint8_t *pbIn) {
  assert(pOut);
  assert(pbIn);

  auto pIn = reinterpret_cast<const typename T::Input *>(pbIn);

  if constexpr (T::Input::DIM >= 1) {
    pOut->x = static_cast<floatf>(pIn->x);
  } else {
    if constexpr (D::DIM >= 1) {
      pOut->x = fZERO;
    }
  }

  if constexpr (T::Input::DIM >= 2) {
    pOut->y = static_cast<floatf>(pIn->y);
  } else {
    if constexpr (D::DIM >= 2) {
      pOut->y = fZERO;
    }
  }

  if constexpr (T::Input::DIM >= 3) {
    pOut->z = static_cast<floatf>(pIn->z);
  } else {
    if constexpr (D::DIM >= 3) {
      pOut->z = fZERO;
    }
  }

  if constexpr (T::Input::DIM >= 4) {
    pOut->w = static_cast<floatf>(pIn->w);
  } else {
    if constexpr (D::DIM >= 4) {
      pOut->w = fONE;
    }
  }
}

template <>
inline void
TDecodeVertex<VECTOR4, TVertexData<VERTEX_UNKNOWN>>(VECTOR4 *pOut,
                                                    const uint8_t *pbIn) {
  __unreferenced(pOut);
  __unreferenced(pbIn);
}

template <>
inline void
TDecodeVertex<VECTOR4, TVertexData<VERTEX_RGBA>>(VECTOR4 *pOut,
                                                 const uint8_t *pbIn) {
  assert(pOut);
  assert(pbIn);

  pOut->x = Math::TFromUNORM8<floatf>(pbIn[0]);
  pOut->y = Math::TFromUNORM8<floatf>(pbIn[1]);
  pOut->z = Math::TFromUNORM8<floatf>(pbIn[2]);
  pOut->w = Math::TFromUNORM8<floatf>(pbIn[3]);
}

//////////////////////////////////////////////////////////////////////////////

template <eVertexFormat VertexFormat>
void TDecodePosition(VECTOR4 *pOut, const uint8_t *pbIn, uint32_t stride,
                     uint32_t count) {
  for (uint32_t i = 0; i < count; ++i) {
    TDecodeVertex<VECTOR4, TVertexData<VertexFormat>>(&pOut[i],
                                                      pbIn + i * stride);
  }
}

//////////////////////////////////////////////////////////////////////////////

template <bool ColorMaterial, eVertexFormat ColorFormat,
          eVertexFormat NormalFormat>
inline void TNL::processLightingOneSided(uint32_t count) {

  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEXDATA_EYEPOS]);
  auto pcFrontColors =
      reinterpret_cast<ColorARGB *>(pbVertexData_[VERTEXDATA_FRONTCOLOR]);

  VECTOR3 vNormal;
  VECTOR4 vVertexColor;
  const uint8_t *pbNormal = nullptr;
  uint32_t normalStride = 0;
  const uint8_t *pbColor = nullptr;
  uint32_t colorStride = 0;

  if constexpr (NormalFormat != VERTEX_UNKNOWN) {
    pbNormal = normalDecode_.pBits;
    normalStride = normalDecode_.Stride;
  } else {
    vNormal = vNormNormal_;
  }

  if constexpr (ColorFormat != VERTEX_UNKNOWN) {
    pbColor = colorDecode_.pBits;
    colorStride = colorDecode_.Stride;
  } else {
    vVertexColor.x = vColor_.x * vLightModelAmbient_.x;
    vVertexColor.y = vColor_.y * vLightModelAmbient_.y;
    vVertexColor.z = vColor_.z * vLightModelAmbient_.z;
    vVertexColor.w = vColor_.w;
  }

  for (uint32_t i = 0; i < count; ++i) {
    VECTOR4 vResult;

    if constexpr (NormalFormat != VERTEX_UNKNOWN) {
      TDecodeVertex<VECTOR3, TVertexData<NormalFormat>>(
          &vNormal, pbNormal + i * normalStride);

      // Transform the normal to world space
      Math::Mul(&vNormal, vNormal, mModelViewInvT_);

      // Normalize the normal
      if (TNLFlags_.Normalize) {
        Math::Normalize(&vNormal);
      }
    }

    if constexpr (ColorMaterial) {
      if constexpr (ColorFormat != VERTEX_UNKNOWN) {
        TDecodeVertex<VECTOR4, TVertexData<ColorFormat>>(
            &vVertexColor, pbColor + i * colorStride);
        vVertexColor.x *= vLightModelAmbient_.x;
        vVertexColor.y *= vLightModelAmbient_.y;
        vVertexColor.z *= vLightModelAmbient_.z;
      }

      vResult = vVertexColor;
    } else {
      vResult = vScaledAmbient_;
    }

    vResult.x += vMatEmissive_.x;
    vResult.y += vMatEmissive_.y;
    vResult.z += vMatEmissive_.z;

    // Apply lights components
    if constexpr (ColorMaterial) {
      this->processLightsOneSided(&vResult, pvEyePos[i], vNormal, vVertexColor);
    } else {
      this->processLightsOneSided(&vResult, pvEyePos[i], vNormal);
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
inline void TNL::processLightingTwoSided(uint32_t count) {

  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEXDATA_EYEPOS]);
  auto pcFrontColors =
      reinterpret_cast<ColorARGB *>(pbVertexData_[VERTEXDATA_FRONTCOLOR]);
  auto pcBackColors =
      reinterpret_cast<ColorARGB *>(pbVertexData_[VERTEXDATA_BACKCOLOR]);

  VECTOR3 vNormal;
  VECTOR4 vVertexColor;
  const uint8_t *pbNormal;
  uint32_t normalStride;
  const uint8_t *pbColor;
  uint32_t colorStride;

  if constexpr (NormalFormat != VERTEX_UNKNOWN) {
    pbNormal = normalDecode_.pBits;
    normalStride = normalDecode_.Stride;
  } else {
    vNormal = vNormNormal_;
  }

  if constexpr (ColorFormat != VERTEX_UNKNOWN) {
    pbColor = colorDecode_.pBits;
    colorStride = colorDecode_.Stride;
  } else {
    vVertexColor.x = vColor_.x * vLightModelAmbient_.x;
    vVertexColor.y = vColor_.y * vLightModelAmbient_.y;
    vVertexColor.z = vColor_.z * vLightModelAmbient_.z;
    vVertexColor.w = vColor_.w;
  }

  for (uint32_t i = 0; i < count; ++i) {
    VECTOR4 vResults[2];

    if constexpr (NormalFormat != VERTEX_UNKNOWN) {
      TDecodeVertex<VECTOR3, TVertexData<NormalFormat>>(
          &vNormal, pbNormal + i * normalStride);

      // Transform the normal to world space
      Math::Mul(&vNormal, vNormal, mModelViewInvT_);

      // Normalize the normal
      if (TNLFlags_.Normalize) {
        Math::Normalize(&vNormal);
      }
    }

    if constexpr (ColorMaterial) {
      if constexpr (ColorFormat != VERTEX_UNKNOWN) {
        TDecodeVertex<VECTOR4, TVertexData<ColorFormat>>(
            &vVertexColor, pbColor + i * colorStride);
        vVertexColor.x *= vLightModelAmbient_.x;
        vVertexColor.y *= vLightModelAmbient_.y;
        vVertexColor.z *= vLightModelAmbient_.z;
      }

      vResults[0] = vVertexColor;
    } else {
      vResults[0] = vScaledAmbient_;
    }

    vResults[0].x += vMatEmissive_.x;
    vResults[0].y += vMatEmissive_.y;
    vResults[0].z += vMatEmissive_.z;

    vResults[1] = vResults[0];

    // Apply lights components
    if constexpr (ColorMaterial) {
      this->processLightsTwoSided(vResults, pvEyePos[i], vNormal, vVertexColor);
    } else {
      this->processLightsTwoSided(vResults, pvEyePos[i], vNormal);
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
void TNL::processVertexColor(uint32_t count) {
  auto pcFrontColors =
      reinterpret_cast<ColorARGB *>(pbVertexData_[VERTEXDATA_FRONTCOLOR]);
  auto pbIn = colorDecode_.pBits;
  auto stride = colorDecode_.Stride;

  for (uint32_t i = 0; i < count; ++i) {
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
void TNL::processTexCoords(uint32_t dstIndex, uint32_t srcIndex,
                            uint32_t count) {
  auto pvTexCoords = reinterpret_cast<TEXCOORD2 *>(
      pbVertexData_[VERTEXDATA_TEXCOORD0 + dstIndex]);
  auto &transform = pMsTexCoords_[srcIndex]->getMatrix();

  auto pbIn = texCoordDecodes_[srcIndex].pBits;
  auto stride = texCoordDecodes_[srcIndex].Stride;

  for (uint32_t i = 0; i < count; ++i) {
    typename TVertexData<VertexFormat>::Output vIn;

    TDecodeVertex<typename TVertexData<VertexFormat>::Output,
                  TVertexData<VertexFormat>>(&vIn, pbIn + i * stride);

    if constexpr (Transform) {
      Math::Mul(&vIn, vIn, transform);
    }

    if constexpr (TVertexData<VertexFormat>::Output::DIM >= 4) {
      if (!Math::TIsZero(vIn.w - fONE) && !Math::TIsZero(vIn.w)) {
        auto fInvW = Math::TInv<floatf>(vIn.w);
        vIn.x *= fInvW;
        vIn.y *= fInvW;
      }
    }

    pvTexCoords[i].m[0] = static_cast<float20>(vIn.x);
    pvTexCoords[i].m[1] = static_cast<float20>(vIn.y);
  }
}

template <bool QuadraticAttenuation, eVertexFormat VertexFormat>
void TNL::processPointSize(uint32_t count) {
  auto pfPointSizes =
      reinterpret_cast<fixed4 *>(pbVertexData_[VERTEXDATA_POINTSIZE]);
  auto &vAttenuation = pointParams_.vAttenuation;
  const VECTOR3 *pvEyePos;
  floatf fInvAtt;

  if constexpr (QuadraticAttenuation) {
    pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEXDATA_EYEPOS]);
  } else {
    fInvAtt = vAttenuation.x;
    if (!Math::TIsZero(fInvAtt - fONE)) {
      fInvAtt = Math::TInvSqrt(fInvAtt);
    }
  }

  auto pbIn = pointSizeDecode_.pBits;
  auto stride = pointSizeDecode_.Stride;

  for (uint32_t i = 0; i < count; ++i) {
    VECTOR1 vPointSize;

    TDecodeVertex<VECTOR1, TVertexData<VertexFormat>>(&vPointSize,
                                                      pbIn + i * stride);
    if constexpr (QuadraticAttenuation) {
      auto fEyeDist = Math::TAbs(pvEyePos[i].z);
      auto fInvAtt = vAttenuation.z * fEyeDist * fEyeDist +
                     vAttenuation.y * fEyeDist + vAttenuation.x;
      if (!Math::TIsZero(fInvAtt - fONE)) {
        vPointSize.x *= Math::TInvSqrt(fInvAtt);
      }
    } else {
      vPointSize.x *= fInvAtt;
    }

    pfPointSizes[i] =
        static_cast<fixed4>(Math::TMax<floatf>(vPointSize.x, fONE));
  }
}

template <eFogMode FogMode> void TNL::processFog(uint32_t count) {

  auto pvEyePos = reinterpret_cast<VECTOR3 *>(pbVertexData_[VERTEXDATA_EYEPOS]);
  auto pfFogs = reinterpret_cast<float20 *>(pbVertexData_[VERTEXDATA_FOG]);

  if constexpr (FogMode == FogLinear) {
    auto fFogEnd = fog_.getFactor(GL_FOG_END);
    auto fFogRatio = fog_.fRatio;

    for (uint32_t i = 0; i < count; ++i) {
      auto fEyeDist = Math::TAbs(pvEyePos[i].z);
      pfFogs[i] = Math::TSat(fFogRatio * (fFogEnd - fEyeDist));
    }
  }
  if constexpr (FogMode == FogExp) {
    auto fFogDensity = fog_.getFactor(GL_FOG_DENSITY);

    for (uint32_t i = 0; i < count; ++i) {
      auto fEyeDist = Math::TAbs(pvEyePos[i].z);
      auto fTmp = fEyeDist * fFogDensity;
      pfFogs[i] = static_cast<fixedRF>(Math::TSat(Math::TExp(-fTmp)));
    }
  }
  if constexpr (FogMode == FogExp2) {
    auto fFogDensity = fog_.getFactor(GL_FOG_DENSITY);

    for (uint32_t i = 0; i < count; ++i) {
      auto fEyeDist = Math::TAbs(pvEyePos[i].z);
      auto fTmp = fEyeDist * fFogDensity;
      pfFogs[i] = static_cast<fixedRF>(Math::TSat(Math::TExp(-(fTmp * fTmp))));
    }
  }
}