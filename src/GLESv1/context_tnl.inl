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

template <class T>
inline void CGLContext::TLight(GLenum light, GLenum pname, const T *pParams) {
  ASSERT(pParams);

  if ((light - GL_LIGHT0) >= MAX_LIGHTS) {
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::Light() failed, invalid light parameter: %d.\r\n"),
        light);
    return;
  }

  VECTOR4 vParam;

  uint32_t index = light - GL_LIGHT0;
  Light &_light = m_lights[index];

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
  case GL_POSITION: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    vParam.y = Math::TCast<floatf>(pParams[1]);
    vParam.z = Math::TCast<floatf>(pParams[2]);
    vParam.w = Math::TCast<floatf>(pParams[3]);

    if (GL_POSITION == pname) {
      _light.Flags.DirectionalLight = Math::TIsZero(vParam.w) ? 1 : 0;
      Math::Mul(&_light.vPosition, vParam, m_pMsModelView->GetMatrix());
    } else {
      _light.SetColor(pname, vParam);
    }

    m_dirtyLights.States[pname - GL_AMBIENT] |= (1 << index);
  }

  break;

  case GL_SPOT_DIRECTION: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    vParam.y = Math::TCast<floatf>(pParams[1]);
    vParam.z = Math::TCast<floatf>(pParams[2]);

    if (m_dirtyFlags.ModelViewInvT33) {
      this->UpdateModelViewInvT33();
    }

    Math::Mul(&_light.vSpotDirection, reinterpret_cast<const VECTOR3 &>(vParam),
              m_mModelViewInvT);
  }

  break;

  case GL_SPOT_EXPONENT: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    if ((vParam.x < fZERO) || (vParam.x > f128)) {
      __glError(
          GL_INVALID_VALUE,
          _T("CGLContext::TLight() failed, invalid param parameter: %d.\r\n"),
          vParam.x);
      return;
    }

    _light.fSpotExponent = vParam.x;
  }

  break;

  case GL_SPOT_CUTOFF: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    if (((vParam.x < fZERO) || (vParam.x > f90)) && (vParam.x != f180)) {
      __glError(
          GL_INVALID_VALUE,
          _T("CGLContext::TLight() failed, invalid param parameter: %d.\r\n"),
          vParam.x);
      return;
    }

    _light.fSpotCutOff = vParam.x;
    _light.fSpotCutOffCos = Math::TCos<floatf>(Math::DegToRad(vParam.x));
  }

  break;

  case GL_CONSTANT_ATTENUATION:
  case GL_LINEAR_ATTENUATION:
  case GL_QUADRATIC_ATTENUATION: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    if (vParam.x < fZERO) {
      __glError(
          GL_INVALID_VALUE,
          _T("CGLContext::TLight() failed, invalid param parameter: %d.\r\n"),
          vParam.x);
      return;
    }

    _light.SetAttenuation(pname, vParam.x);
  }

  break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::TLight() failed, invalid pname parameter: %d.\r\n"),
        pname);
    return;
  }
}

template <class T>
inline void CGLContext::TLightModel(GLenum pname, const T *pParams) {
  ASSERT(pParams);

  switch (pname) {
  case GL_LIGHT_MODEL_TWO_SIDE:
    m_caps.TwoSidedLighting = (Math::TCast<floatf>(pParams[0]) != fZERO);
    break;

  case GL_LIGHT_MODEL_AMBIENT:
    m_vLightModelAmbient.x = Math::TCast<floatf>(pParams[0]);
    m_vLightModelAmbient.y = Math::TCast<floatf>(pParams[1]);
    m_vLightModelAmbient.z = Math::TCast<floatf>(pParams[2]);
    m_vLightModelAmbient.w = Math::TCast<floatf>(pParams[3]);
    m_dirtyFlags.ScaledAmbient = 1;
    break;

  default:
    __glError(GL_INVALID_ENUM, _T("CGLContext::TLightModel() failed, invalid ")
                               _T("pname parameter: %d.\r\n"),
              pname);
    return;
  }
}

template <class T>
inline void CGLContext::TMaterial(GLenum face, GLenum pname, const T *pParams) {
  ASSERT(pParams);

  if (face != GL_FRONT_AND_BACK) {
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::TMaterial() failed, invalid face parameter: %d.\r\n"),
        face);
    return;
  }

  VECTOR4 vParam;

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_AMBIENT_AND_DIFFUSE:
    m_dirtyFlags.ScaledAmbient = 1;
    [[fallthrough]];
  case GL_SPECULAR:
  case GL_EMISSION: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    vParam.y = Math::TCast<floatf>(pParams[1]);
    vParam.z = Math::TCast<floatf>(pParams[2]);
    vParam.w = Math::TCast<floatf>(pParams[3]);

    if (GL_EMISSION == pname) {
      m_vMatEmissive = vParam;
    } else {
      if (GL_AMBIENT_AND_DIFFUSE == pname) {
        m_material.SetColor(GL_AMBIENT, vParam);
        m_dirtyLights.States[0] = LIGHTS_MASK;
        pname = GL_DIFFUSE;
      }

      m_material.SetColor(pname, vParam);
      m_dirtyLights.States[pname - GL_AMBIENT] = LIGHTS_MASK;
    }
  }

  break;

  case GL_SHININESS: {
    vParam.x = Math::TCast<floatf>(pParams[0]);
    if ((vParam.x < fZERO) || (vParam.x > f128)) {
      __glError(GL_INVALID_VALUE, _T("CGLContext::TMaterial() failed, invalid ")
                                  _T("param parameter: %d.\r\n"),
                vParam.x);
      return;
    }

    m_fMatShininess = vParam.x;
  }

  break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::TMaterial() failed, invalid pname parameter: %d.\r\n"),
        pname);
    return;
  }
}

template <class T>
inline void CGLContext::TFog(GLenum pname, const T *pParams) {
  ASSERT(pParams);

  switch (pname) {
  case GL_FOG_MODE: {
    GLenum param = TToGLenum<T>(*pParams);
    switch (param) {
    case GL_LINEAR:
    case GL_EXP:
    case GL_EXP2:
      m_fog.Mode = static_cast<eFogMode>(FogModeFromEnum(param));
      break;

    default:
      __glError(
          GL_INVALID_ENUM,
          _T("CGLContext::TFog() failed, invalid param parameter: %d.\r\n"),
          param);
      return;
    }
  }

  break;

  case GL_FOG_START:
  case GL_FOG_END:
    m_dirtyFlags.FogRatio = 1;
    [[fallthrough]];
  case GL_FOG_DENSITY:
    m_fog.SetFactor(pname, Math::TCast<floatf>(pParams[0]));
    break;

  case GL_FOG_COLOR:
    m_vFogColor.x = Math::TCast<floatf>(pParams[0]);
    m_vFogColor.y = Math::TCast<floatf>(pParams[1]);
    m_vFogColor.z = Math::TCast<floatf>(pParams[2]);
    m_vFogColor.w = Math::TCast<floatf>(pParams[3]);
    m_dirtyFlags.FogColor = 1;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TFog() failed, invalid pname parameter: %d.\r\n"),
              pname);
    return;
  }
}
