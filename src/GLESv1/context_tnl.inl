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

template <typename T>
inline void GLContext::setLightParameter(GLenum light, GLenum pname,
                                         const T *pParams) {
  assert(pParams);

  if ((light - GL_LIGHT0) >= MAX_LIGHTS) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setLightParameter() failed, invalid light parameter: "
              "%d.\r\n",
              light);
    return;
  }

  uint32_t index = light - GL_LIGHT0;
  Light &_light = lights_[index];

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
  case GL_POSITION: {
    VECTOR4 vParam(
      static_cast<floatf>(pParams[0]),
      static_cast<floatf>(pParams[1]),
      static_cast<floatf>(pParams[2]),
      static_cast<floatf>(pParams[3])
    );

    if (GL_POSITION == pname) {
      _light.Flags.DirectionalLight = Math::IsAlmostZero(vParam.w) ? 1 : 0;
      Math::Mul(&_light.vPosition, vParam, pMsModelView_->getMatrix());
    } else {
      _light.setColor(pname, vParam);
    }

    dirtyLights_.States[pname - GL_AMBIENT] |= (1 << index);
  }

  break;

  case GL_SPOT_DIRECTION: {
    VECTOR3 vParam(
      static_cast<floatf>(pParams[0]),
      static_cast<floatf>(pParams[1]),
      static_cast<floatf>(pParams[2])
    );

    if (dirtyFlags_.ModelViewInvT33) {
      this->updateModelViewInvT33();
    }

    Math::Mul(&_light.vSpotDirection, vParam, mModelViewInvT_);
  }

  break;

  case GL_SPOT_EXPONENT: {
    VECTOR1 vParam(static_cast<floatf>(pParams[0]));
    if ((vParam.x < Math::Zero<floatf>()) || (vParam.x > Math::F128<floatf>())) {
      __glError(GL_INVALID_VALUE,
                "GLContext::setLightParameter() failed, invalid param "
                "parameter: %d.\r\n",
                vParam.x);
      return;
    }

    _light.fSpotExponent = vParam.x;
  }

  break;

  case GL_SPOT_CUTOFF: {
    VECTOR1 vParam(static_cast<floatf>(pParams[0]));
    if (((vParam.x < Math::Zero<floatf>()) || (vParam.x > Math::F90<floatf>())) && (vParam.x != Math::F180<floatf>())) {
      __glError(GL_INVALID_VALUE,
                "GLContext::setLightParameter() failed, invalid param "
                "parameter: %d.\r\n",
                vParam.x);
      return;
    }

    _light.fSpotCutOff = vParam.x;
    _light.fSpotCutOffCos = std::cos(Math::DegToRad(vParam.x));
  }

  break;

  case GL_CONSTANT_ATTENUATION:
  case GL_LINEAR_ATTENUATION:
  case GL_QUADRATIC_ATTENUATION: {
    VECTOR1 vParam(static_cast<floatf>(pParams[0]));
    if (vParam.x < Math::Zero<floatf>()) {
      __glError(GL_INVALID_VALUE,
                "GLContext::setLightParameter() failed, invalid param "
                "parameter: %d.\r\n",
                vParam.x);
      return;
    }
    _light.setAttenuation(pname, vParam.x);
  }

  break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setLightParameter() failed, invalid pname parameter: "
              "%d.\r\n",
              pname);
    return;
  }
}

template <typename T>
inline void GLContext::setLightParameterModel(GLenum pname, const T *pParams) {
  assert(pParams);

  switch (pname) {
  case GL_LIGHT_MODEL_TWO_SIDE:
    caps_.TwoSidedLighting = (static_cast<floatf>(pParams[0]) != Math::Zero<floatf>());
    break;

  case GL_LIGHT_MODEL_AMBIENT:
    vLightModelAmbient_.x = static_cast<floatf>(pParams[0]);
    vLightModelAmbient_.y = static_cast<floatf>(pParams[1]);
    vLightModelAmbient_.z = static_cast<floatf>(pParams[2]);
    vLightModelAmbient_.w = static_cast<floatf>(pParams[3]);
    dirtyFlags_.ScaledAmbient = 1;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setLightParameterModel() failed, invalid "
              "pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <typename T>
inline void GLContext::setMaterial(GLenum face, GLenum pname,
                                   const T *pParams) {
  assert(pParams);

  if (face != GL_FRONT_AND_BACK) {
    __glError(
        GL_INVALID_ENUM,
        "GLContext::setMaterial() failed, invalid face parameter: %d.\r\n",
        face);
    return;
  }

  VECTOR4 vParam;

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_AMBIENT_AND_DIFFUSE:
    dirtyFlags_.ScaledAmbient = 1;
    [[fallthrough]];
  case GL_SPECULAR:
  case GL_EMISSION: {
    vParam.x = static_cast<floatf>(pParams[0]);
    vParam.y = static_cast<floatf>(pParams[1]);
    vParam.z = static_cast<floatf>(pParams[2]);
    vParam.w = static_cast<floatf>(pParams[3]);

    if (GL_EMISSION == pname) {
      vMatEmissive_ = vParam;
    } else {
      if (GL_AMBIENT_AND_DIFFUSE == pname) {
        material_.setColor(GL_AMBIENT, vParam);
        dirtyLights_.States[0] = LIGHTS_MASK;
        pname = GL_DIFFUSE;
      }

      material_.setColor(pname, vParam);
      dirtyLights_.States[pname - GL_AMBIENT] = LIGHTS_MASK;
    }
  }

  break;

  case GL_SHININESS: {
    vParam.x = static_cast<floatf>(pParams[0]);
    if ((vParam.x < Math::Zero<floatf>()) || (vParam.x > Math::F128<floatf>())) {
      __glError(GL_INVALID_VALUE,
                "GLContext::setMaterial() failed, invalid "
                "param parameter: %d.\r\n",
                vParam.x);
      return;
    }

    fMatShininess_ = vParam.x;
  }

  break;

  default:
    __glError(
        GL_INVALID_ENUM,
        "GLContext::setMaterial() failed, invalid pname parameter: %d.\r\n",
        pname);
    return;
  }
}

template <typename T>
inline void GLContext::setFog(GLenum pname, const T *pParams) {
  assert(pParams);

  switch (pname) {
  case GL_FOG_MODE: {
    GLenum param = ToGLenum<T>(*pParams);
    switch (param) {
    case GL_LINEAR:
    case GL_EXP:
    case GL_EXP2:
      fog_.Mode = static_cast<eFogMode>(FogModeFromEnum(param));
      break;

    default:
      __glError(GL_INVALID_ENUM,
                "GLContext::setFog() failed, invalid param parameter: %d.\r\n",
                param);
      return;
    }
  }

  break;

  case GL_FOG_START:
  case GL_FOG_END:
    dirtyFlags_.FogRatio = 1;
    [[fallthrough]];
  case GL_FOG_DENSITY:
    fog_.setFactor(pname, static_cast<floatf>(pParams[0]));
    break;

  case GL_FOG_COLOR:
    vFogColor_.x = static_cast<floatf>(pParams[0]);
    vFogColor_.y = static_cast<floatf>(pParams[1]);
    vFogColor_.z = static_cast<floatf>(pParams[2]);
    vFogColor_.w = static_cast<floatf>(pParams[3]);
    dirtyFlags_.FogColor = 1;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setFog() failed, invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}
