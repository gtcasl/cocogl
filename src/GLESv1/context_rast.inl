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
inline void GLContext::setTexParameter(GLenum target, GLenum pname,
                                       const T *pParams) {
  assert(pParams);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext::setTexParameter() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  GLenum param = TToGLenum<T>(pParams[0]);

  auto pTexture = texUnits_[activeTexture_].getTexture();
  assert(pTexture);

  switch (pname) {
  case GL_TEXTURE_MIN_FILTER:
    switch (param) {
    case GL_NEAREST:
    case GL_LINEAR:
    case GL_NEAREST_MIPMAP_NEAREST:
    case GL_LINEAR_MIPMAP_NEAREST:
    case GL_NEAREST_MIPMAP_LINEAR:
    case GL_LINEAR_MIPMAP_LINEAR:
      pTexture->Params.MinFilter = TexFilterFromEnum(param);
      pTexture->Invalidate();
      break;

    default:
      __glError(GL_INVALID_VALUE,
                "GLContext::setTexParameter() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    break;

  case GL_TEXTURE_MAG_FILTER:
    switch (param) {
    case GL_NEAREST:
    case GL_LINEAR:
      pTexture->Params.MagFilter = TexFilterFromEnum(param);
      pTexture->Invalidate();
      break;

    default:
      __glError(GL_INVALID_VALUE,
                "GLContext::setTexParameter() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    break;

  case GL_TEXTURE_WRAP_S:
    switch (param) {
    case GL_CLAMP_TO_EDGE:
    case GL_REPEAT:
      pTexture->Params.AddressU = TexAddressFromEnum(param);
      break;

    default:
      __glError(GL_INVALID_VALUE,
                "GLContext::setTexParameter() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    break;

  case GL_TEXTURE_WRAP_T:
    switch (param) {
    case GL_CLAMP_TO_EDGE:
    case GL_REPEAT:
      pTexture->Params.AddressV = TexAddressFromEnum(param);
      break;

    default:
      __glError(GL_INVALID_VALUE,
                "GLContext::setTexParameter() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    break;

  case GL_GENERATE_MIPMAP:
    pTexture->bGenMipMaps = (param != 0);
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setTexParameter() failed, "
              "invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <typename T>
inline void GLContext::setTexEnv(GLenum env, GLenum pname, const T *pParams) {
  assert(pParams);

  GLenum param = TToGLenum<T>(pParams[0]);

  TexUnit &texUnit = texUnits_[activeTexture_];

  switch (env) {
  case GL_TEXTURE_ENV:
    switch (pname) {
    case GL_TEXTURE_ENV_MODE:
      switch (param) {
      case GL_ADD:
      case GL_BLEND:
      case GL_REPLACE:
      case GL_MODULATE:
      case GL_DECAL:
        texUnit.EnvMode = static_cast<uint8_t>(TexEnvFromEnum(param));
        break;

      default:
        __glError(GL_INVALID_ENUM,
                  "GLContext::setTexEnv() failed, invalid "
                  "pname parameter: %d.\r\n",
                  pname);
        return;
      }

      break;

    case GL_TEXTURE_ENV_COLOR:
      texUnit.cEnvColor.b = static_cast<uint8_t>(
          Math::ToUNORM8(Math::Sat(static_cast<floatf>(pParams[0]))));
      texUnit.cEnvColor.g = static_cast<uint8_t>(
          Math::ToUNORM8(Math::Sat(static_cast<floatf>(pParams[1]))));
      texUnit.cEnvColor.r = static_cast<uint8_t>(
          Math::ToUNORM8(Math::Sat(static_cast<floatf>(pParams[2]))));
      texUnit.cEnvColor.a = static_cast<uint8_t>(
          Math::ToUNORM8(Math::Sat(static_cast<floatf>(pParams[3]))));
      break;
    }

    break;

  case GL_POINT_SPRITE_OES:
    switch (pname) {
    case GL_COORD_REPLACE_OES:
      texUnit.bCoordReplace = (param != 0);
      break;

    default:
      __glError(
          GL_INVALID_ENUM,
          "GLContext::setTexEnv() failed, invalid pname parameter: %d.\r\n",
          pname);
      return;
    }

    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setTexEnv() failed, invalid env parameter: %d.\r\n",
              env);
    return;
  }
}

template <typename T>
inline void GLContext::setPointParameter(GLenum pname, const T *pParams) {
  assert(pParams);

  switch (pname) {
  case GL_POINT_SIZE_MIN:
  case GL_POINT_SIZE_MAX:
  case GL_POINT_FADE_THRESHOLD_SIZE: {
    auto param = static_cast<floatf>(pParams[0]);
    if (param < fZERO) {
      __glError(GL_INVALID_VALUE,
                "GLContext::setPointParameter() failed, "
                "invalid param parameter: %d.\r\n",
                param);
      return;
    }

    pointParams_.set(pname, param);
  }

  break;

  case GL_POINT_DISTANCE_ATTENUATION:
    pointParams_.vAttenuation.x = static_cast<floatf>(pParams[0]);
    pointParams_.vAttenuation.y = static_cast<floatf>(pParams[1]);
    pointParams_.vAttenuation.z = static_cast<floatf>(pParams[2]);

    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::setPointParameter() failed, "
              "invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}
