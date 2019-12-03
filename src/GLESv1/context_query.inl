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

extern const GLint g_compressedFormats[] = {
    GL_PALETTE4_RGB8_OES,   GL_PALETTE4_RGBA8_OES,    GL_PALETTE4_R5_G6_B5_OES,
    GL_PALETTE4_RGBA4_OES,  GL_PALETTE4_RGB5_A1_OES,  GL_PALETTE8_RGB8_OES,
    GL_PALETTE8_RGBA8_OES,  GL_PALETTE8_R5_G6_B5_OES, GL_PALETTE8_RGBA4_OES,
    GL_PALETTE8_RGB5_A1_OES};

template <typename T>
inline void GLContext::getClipPlane(GLenum plane, T eqn[4]) {
  if ((plane - GL_CLIP_PLANE0) >= MAX_CLIPPLANES) {
    __glError(GL_INVALID_ENUM,
              "GLContext::getClipPlane() failed, "
              "invalid plane parameter: %d.\r\n",
              plane);
    return;
  }

  uint32_t index = plane - GL_CLIP_PLANE0;

  eqn[0] = static_cast<T>(vClipPlanesES_[index].x);
  eqn[1] = static_cast<T>(vClipPlanesES_[index].y);
  eqn[2] = static_cast<T>(vClipPlanesES_[index].z);
  eqn[3] = static_cast<T>(vClipPlanesES_[index].w);
}

template <typename T>
inline void GLContext::getLight(GLenum light, GLenum pname, T *pParams) {
  assert(pParams);

  if ((light - GL_LIGHT0) >= MAX_LIGHTS) {
    __glError(GL_INVALID_ENUM,
              "GLContext::getLight() failed, invalid light parameter: %d.\r\n",
              light);
    return;
  }

  uint32_t index = light - GL_LIGHT0;
  Light &_light = lights_[index];

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
    pParams[0] = static_cast<T>(_light.getColor(pname).x);
    pParams[1] = static_cast<T>(_light.getColor(pname).y);
    pParams[2] = static_cast<T>(_light.getColor(pname).z);
    pParams[3] = static_cast<T>(_light.getColor(pname).w);
    break;

  case GL_POSITION:
    pParams[0] = static_cast<T>(_light.vPosition.x);
    pParams[1] = static_cast<T>(_light.vPosition.y);
    pParams[2] = static_cast<T>(_light.vPosition.z);
    pParams[3] = static_cast<T>(_light.vPosition.w);
    break;

  case GL_SPOT_DIRECTION:
    pParams[0] = static_cast<T>(_light.vSpotDirection.x);
    pParams[1] = static_cast<T>(_light.vSpotDirection.y);
    pParams[2] = static_cast<T>(_light.vSpotDirection.z);
    break;

  case GL_SPOT_EXPONENT:
    pParams[0] = static_cast<T>(_light.fSpotExponent);
    break;

  case GL_SPOT_CUTOFF:
    pParams[0] = static_cast<T>(_light.fSpotCutOff);
    break;

  case GL_CONSTANT_ATTENUATION:
  case GL_LINEAR_ATTENUATION:
  case GL_QUADRATIC_ATTENUATION:
    pParams[0] = static_cast<T>(_light.getAttenuation(pname));
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::getLight() failed, invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <typename T>
inline void GLContext::getMaterial(GLenum face, GLenum pname, T *pParams) {
  assert(pParams);

  if (face != GL_FRONT && face != GL_BACK) {
    __glError(GL_INVALID_ENUM,
              "GLContext::getMaterial() failed, invalid "
              "face parameter: %d.\r\n",
              face);
    return;
  }

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
    pParams[0] = static_cast<T>(material_.getColor(pname).x);
    pParams[1] = static_cast<T>(material_.getColor(pname).y);
    pParams[2] = static_cast<T>(material_.getColor(pname).z);
    pParams[3] = static_cast<T>(material_.getColor(pname).w);
    break;

  case GL_EMISSION:
    pParams[0] = static_cast<T>(vMatEmissive_.x);
    pParams[1] = static_cast<T>(vMatEmissive_.y);
    pParams[2] = static_cast<T>(vMatEmissive_.z);
    pParams[3] = static_cast<T>(vMatEmissive_.w);
    break;

  case GL_SHININESS:
    pParams[0] = static_cast<T>(fMatShininess_);
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::getMaterial() failed, invalid "
              "pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <typename T>
inline void GLContext::getTexEnv(GLenum env, GLenum pname, T *pParams) {
  assert(pParams);

  TexUnit &texUnit = texUnits_[activeTexture_];

  switch (env) {
  case GL_TEXTURE_ENV:
    switch (pname) {
    case GL_TEXTURE_ENV_MODE:
      pParams[0] = static_cast<T>(EnumFromTexEnv(texUnit.EnvMode));
      break;

    case GL_TEXTURE_ENV_COLOR:
      pParams[0] = Math::TFromUNORM8<T>(texUnit.cEnvColor.r);
      pParams[1] = Math::TFromUNORM8<T>(texUnit.cEnvColor.g);
      pParams[2] = Math::TFromUNORM8<T>(texUnit.cEnvColor.b);
      pParams[3] = Math::TFromUNORM8<T>(texUnit.cEnvColor.a);
      break;

    default:
      __glError(GL_INVALID_ENUM,
                "GLContext::getTexEnv() failed, invalid "
                "pname parameter: %d.\r\n",
                pname);
      return;
    }

    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::getTexEnv() failed, invalid env parameter: %d.\r\n",
              env);
    return;
  }
}

template <typename T>
inline void GLContext::getTexParameter(GLenum target, GLenum pname,
                                       T *pParams) {
  assert(pParams);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM,
              "GLContext::getTexParameter() failed, "
              "invalid target parameter: %d.\r\n",
              target);
    return;
  }

  auto pTexture = this->getTexture(activeTexture_);
  assert(pTexture);

  switch (pname) {
  case GL_TEXTURE_MIN_FILTER:
    pParams[0] = static_cast<T>(EnumFromTexFilter(pTexture->Params.MinFilter));
    break;

  case GL_TEXTURE_MAG_FILTER:
    pParams[0] = static_cast<T>(EnumFromTexFilter(pTexture->Params.MagFilter));
    break;

  case GL_TEXTURE_WRAP_S:
    pParams[0] = static_cast<T>(EnumFromTexAddress(pTexture->Params.AddressU));
    break;

  case GL_TEXTURE_WRAP_T:
    pParams[0] = static_cast<T>(EnumFromTexAddress(pTexture->Params.AddressV));
    break;

  case GL_GENERATE_MIPMAP:
    pParams[0] = static_cast<T>(pTexture->bGenMipMaps);
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::getTexParameter() failed, "
              "invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <> inline void GLContext::get<bool>(GLenum pname, bool *pParams) {
  assert(pParams);

  switch (pname) {
  case GL_LIGHT_MODEL_TWO_SIDE:
    pParams[0] = caps_.TwoSidedLighting;
    break;

  case GL_COLOR_WRITEMASK:
    pParams[0] = (cColorWriteMask_.r != 0);
    pParams[1] = (cColorWriteMask_.g != 0);
    pParams[2] = (cColorWriteMask_.b != 0);
    pParams[3] = (cColorWriteMask_.a != 0);
    break;

  case GL_DEPTH_WRITEMASK:
    pParams[0] = (depthWriteMask_ != 0);
    break;

  case GL_SAMPLE_COVERAGE_INVERT:
    pParams[0] = sampleCoverage_.bInvert;
    break;

  case GL_COORD_REPLACE_OES:
    pParams[0] = texUnits_[activeTexture_].bCoordReplace;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::get() failed, invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <> inline void GLContext::get<int>(GLenum pname, int *pParams) {
  assert(pParams);

  switch (pname) {
  case GL_ALIASED_POINT_SIZE_RANGE:
  case GL_ALIASED_LINE_WIDTH_RANGE:
    pParams[0] = pParams[1] = 32;
    break;

  case GL_SMOOTH_LINE_WIDTH_RANGE:
  case GL_SMOOTH_POINT_SIZE_RANGE:
    pParams[0] = pParams[1] = 1;
    break;

  case GL_SUBPIXEL_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_SUBPIXEL_BITS);
    break;

  case GL_RED_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_RED_BITS);
    break;

  case GL_GREEN_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_GREEN_BITS);
    break;

  case GL_BLUE_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_BLUE_BITS);
    break;

  case GL_ALPHA_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_ALPHA_BITS);
    break;

  case GL_DEPTH_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_DEPTH_BITS);
    break;

  case GL_STENCIL_BITS:
    pParams[0] = pSurfDraw_->getAttribute(GL_STENCIL_BITS);
    break;

  case GL_COMPRESSED_TEXTURE_FORMATS:
    for (uint32_t i = 0; i < __countof(g_compressedFormats); ++i) {
      pParams[i] = g_compressedFormats[i];
    }
    break;

  case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
    pParams[0] = __countof(g_compressedFormats);
    break;

  case GL_MAX_LIGHTS:
    pParams[0] = MAX_LIGHTS;
    break;

  case GL_MAX_CLIP_PLANES:
    pParams[0] = MAX_CLIPPLANES;
    break;

  case GL_MAX_MODELVIEW_STACK_DEPTH:
    pParams[0] = pMsModelView_->getSize();
    break;

  case GL_MAX_PROJECTION_STACK_DEPTH:
    pParams[0] = pMsProjection_->getSize();
    break;

  case GL_MAX_TEXTURE_STACK_DEPTH:
    pParams[0] = pMsTexCoords_[activeTexture_]->getSize();
    break;

  case GL_MAX_TEXTURE_SIZE:
    pParams[0] = MAX_TEXTURE_SIZE;
    break;

  case GL_MAX_TEXTURE_UNITS:
    pParams[0] = MAX_TEXTURES;
    break;

  case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
    pParams[0] = IMPLEMENTATION_COLOR_READ_TYPE_OES;
    break;

  case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
    pParams[0] = IMPLEMENTATION_COLOR_READ_FORMAT_OES;
    break;

  case GL_MAX_VIEWPORT_DIMS:
    pParams[0] = MAX_TEXTURE_SIZE;
    pParams[1] = MAX_TEXTURE_SIZE;
    break;

  case GL_VERTEX_ARRAY_SIZE:
    pParams[0] = DecodeDataSize(positionArray_.Format);
    break;

  case GL_VERTEX_ARRAY_STRIDE:
    pParams[0] = positionArray_.Stride;
    break;

  case GL_VERTEX_ARRAY_TYPE:
    pParams[0] = DecodeDataType(positionArray_.Format);
    break;

  case GL_NORMAL_ARRAY_STRIDE:
    pParams[0] = normalArray_.Stride;
    break;

  case GL_NORMAL_ARRAY_TYPE:
    pParams[0] = DecodeDataType(normalArray_.Format);
    break;

  case GL_COLOR_ARRAY_SIZE:
    pParams[0] = DecodeDataSize(colorArray_.Format);
    break;

  case GL_COLOR_ARRAY_STRIDE:
    pParams[0] = colorArray_.Stride;
    break;

  case GL_COLOR_ARRAY_TYPE:
    pParams[0] = DecodeDataType(colorArray_.Format);
    break;

  case GL_TEXTURE_COORD_ARRAY_SIZE:
    pParams[0] = DecodeDataSize(texCoordArrays_[clientActiveTexture_].Format);
    break;

  case GL_TEXTURE_COORD_ARRAY_STRIDE:
    pParams[0] = texCoordArrays_[clientActiveTexture_].Stride;
    break;

  case GL_TEXTURE_COORD_ARRAY_TYPE:
    pParams[0] = DecodeDataType(texCoordArrays_[clientActiveTexture_].Format);
    break;

  case GL_POINT_SIZE_ARRAY_TYPE_OES:
    pParams[0] = DecodeDataType(pointSizeArray_.Format);
    break;

  case GL_POINT_SIZE_ARRAY_STRIDE_OES:
    pParams[0] = pointSizeArray_.Stride;
    break;

  case GL_VERTEX_ARRAY_BUFFER_BINDING:
    pParams[0] = positionArray_.getBufferHandle();
    break;

  case GL_NORMAL_ARRAY_BUFFER_BINDING:
    pParams[0] = normalArray_.getBufferHandle();
    break;

  case GL_COLOR_ARRAY_BUFFER_BINDING:
    pParams[0] = colorArray_.getBufferHandle();
    break;

  case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
    pParams[0] = texCoordArrays_[clientActiveTexture_].getBufferHandle();
    break;

  case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
    pParams[0] = pointSizeArray_.getBufferHandle();
    break;

  case GL_ARRAY_BUFFER_BINDING:
    pParams[0] = this->getBufferObjectHandle(GL_ARRAY_BUFFER);
    break;

  case GL_ELEMENT_ARRAY_BUFFER_BINDING:
    pParams[0] = this->getBufferObjectHandle(GL_ELEMENT_ARRAY_BUFFER);
    break;

  case GL_TEXTURE_BINDING_2D:
    pParams[0] = this->getBoundTextureHandle();
    break;

  case GL_UNPACK_ALIGNMENT:
    pParams[0] = unpackAlignment_;
    break;

  case GL_PACK_ALIGNMENT:
    pParams[0] = packAlignment_;
    break;

  case GL_STENCIL_CLEAR_VALUE:
    pParams[0] = clearStencil_;
    break;

  case GL_SCISSOR_BOX:
    pParams[0] = scissor_.left;
    pParams[1] = scissor_.top;
    pParams[2] = scissor_.right - scissor_.left;
    pParams[3] = scissor_.bottom - scissor_.top;
    break;

  case GL_VIEWPORT:
    pParams[0] = viewport_.left;
    pParams[1] = viewport_.top;
    pParams[2] = viewport_.right - viewport_.left;
    pParams[3] = viewport_.bottom - viewport_.top;
    break;

  case GL_STENCIL_WRITEMASK:
    pParams[0] = stencilWriteMask_;
    break;

  case GL_STENCIL_VALUE_MASK:
    pParams[0] = rasterData_.StencilMask;
    break;

  case GL_STENCIL_REF:
    pParams[0] = rasterData_.StencilRef;
    break;

  case GL_ALPHA_TEST_FUNC:
    pParams[0] = EnumFromcompareFunc(rasterStates_.AlphaFunc);
    break;

  case GL_STENCIL_FUNC:
    pParams[0] = EnumFromcompareFunc(rasterStates_.StencilFunc);
    break;

  case GL_STENCIL_FAIL:
    pParams[0] = EnumFromStencilOp(rasterStates_.StencilFail);
    break;

  case GL_STENCIL_PASS_DEPTH_FAIL:
    pParams[0] = EnumFromStencilOp(rasterStates_.StencilZFail);
    break;

  case GL_STENCIL_PASS_DEPTH_PASS:
    pParams[0] = EnumFromStencilOp(rasterStates_.StencilZPass);
    break;

  case GL_DEPTH_FUNC:
    pParams[0] = EnumFromcompareFunc(rasterStates_.DepthFunc);
    break;

  case GL_LOGIC_OP_MODE:
    pParams[0] = EnumFromLogicOp(rasterStates_.LogicFunc);
    break;

  case GL_BLEND_SRC:
    pParams[0] = EnumFromBlendFunc(rasterStates_.BlendSrc);
    break;

  case GL_BLEND_DST:
    pParams[0] = EnumFromBlendFunc(rasterStates_.BlendDst);
    break;

  case GL_FOG_MODE:
    pParams[0] = EnumFromFogMode(fog_.Mode);
    break;

  case GL_SHADE_MODEL:
    pParams[0] = EnumFromShadeModel(caps_.ShadeModel);
    break;

  case GL_CULL_FACE_MODE:
    pParams[0] = EnumFromCullFace(cullStates_.CullFace);
    break;

  case GL_FRONT_FACE:
    pParams[0] = EnumFromFrontFace(cullStates_.FrontFace);
    break;

  case GL_MATRIX_MODE:
    pParams[0] = matrixMode_;
    break;

  case GL_ACTIVE_TEXTURE:
  case GL_CLIENT_ACTIVE_TEXTURE:
    pParams[0] = GL_TEXTURE0;
    break;

  case GL_PERSPECTIVE_CORRECTION_HINT:
    pParams[0] = EnumFromHint(hints_.Perspective);
    break;

  case GL_POINT_SMOOTH_HINT:
    pParams[0] = EnumFromHint(hints_.PointSmooth);
    break;

  case GL_LINE_SMOOTH_HINT:
    pParams[0] = EnumFromHint(hints_.LineSmooth);
    break;

  case GL_FOG_HINT:
    pParams[0] = EnumFromHint(hints_.Fog);
    break;

  case GL_GENERATE_MIPMAP_HINT:
    pParams[0] = EnumFromHint(hints_.GenerateMipmap);
    break;

  // GL Extensions
  case GL_MAX_VERTEX_UNITS_OES:
  case GL_MAX_PALETTE_MATRICES_OES:
    pParams[0] = 0;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::get() failed, invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <typename T> inline void GLContext::get(GLenum pname, T *pParams) {
  assert(pParams);

  switch (pname) {
  case GL_CURRENT_COLOR:
    pParams[0] = static_cast<T>(vColor_.x);
    pParams[1] = static_cast<T>(vColor_.y);
    pParams[2] = static_cast<T>(vColor_.z);
    pParams[3] = static_cast<T>(vColor_.w);
    break;

  case GL_DEPTH_CLEAR_VALUE:
    pParams[0] = static_cast<T>(fClearDepth_);
    break;

  case GL_CURRENT_TEXTURE_COORDS:
    pParams[0] = static_cast<T>(vTexCoords_[activeTexture_].x);
    pParams[1] = static_cast<T>(vTexCoords_[activeTexture_].y);
    pParams[2] = static_cast<T>(fZERO);
    pParams[3] = static_cast<T>(fONE);
    break;

  case GL_CURRENT_NORMAL:
    pParams[0] = static_cast<T>(vNormal_.x);
    pParams[1] = static_cast<T>(vNormal_.y);
    pParams[2] = static_cast<T>(vNormal_.z);
    break;

  case GL_MODELVIEW_MATRIX:
    pMsModelView_->getTop<T>(pParams);
    break;

  case GL_PROJECTION_MATRIX:
    pMsProjection_->getTop<T>(pParams);
    break;

  case GL_TEXTURE_MATRIX:
    pMsTexCoords_[activeTexture_]->getTop<T>(pParams);
    break;

  case GL_FOG_COLOR:
    pParams[0] = static_cast<T>(vFogColor_.x);
    pParams[1] = static_cast<T>(vFogColor_.y);
    pParams[2] = static_cast<T>(vFogColor_.z);
    pParams[3] = static_cast<T>(vFogColor_.w);
    break;

  case GL_FOG_DENSITY:
    pParams[0] = static_cast<T>(fog_.getFactor(GL_FOG_DENSITY));
    break;

  case GL_FOG_START:
    pParams[0] = static_cast<T>(fog_.getFactor(GL_FOG_START));
    break;

  case GL_FOG_END:
    pParams[0] = static_cast<T>(fog_.getFactor(GL_FOG_END));
    break;

  case GL_ALPHA_TEST_REF:
    pParams[0] = Math::TFromUNORM8<T>(rasterData_.AlphaRef);
    break;

  case GL_LIGHT_MODEL_AMBIENT:
    pParams[0] = static_cast<T>(vLightModelAmbient_.x);
    pParams[1] = static_cast<T>(vLightModelAmbient_.y);
    pParams[2] = static_cast<T>(vLightModelAmbient_.z);
    pParams[3] = static_cast<T>(vLightModelAmbient_.w);
    break;

  case GL_COLOR_CLEAR_VALUE:
    pParams[0] = static_cast<T>(vClearColor_.x);
    pParams[1] = static_cast<T>(vClearColor_.y);
    pParams[2] = static_cast<T>(vClearColor_.z);
    pParams[3] = static_cast<T>(vClearColor_.w);
    break;

  case GL_POLYGON_OFFSET_UNITS:
    pParams[0] = static_cast<T>(polygonOffset_.fUnits);
    break;

  case GL_POLYGON_OFFSET_FACTOR:
    pParams[0] = static_cast<T>(polygonOffset_.fFactor);
    break;

  case GL_SAMPLE_COVERAGE_VALUE:
    pParams[0] = static_cast<T>(sampleCoverage_.fValue);
    break;

  case GL_POINT_SIZE_MIN:
    pParams[0] = static_cast<T>(pointParams_.get(GL_POINT_SIZE_MIN));
    break;

  case GL_POINT_SIZE_MAX:
    pParams[0] = static_cast<T>(pointParams_.get(GL_POINT_SIZE_MAX));
    break;

  case GL_POINT_FADE_THRESHOLD_SIZE:
    pParams[0] = static_cast<T>(pointParams_.get(GL_POINT_FADE_THRESHOLD_SIZE));
    break;

  case GL_POINT_DISTANCE_ATTENUATION:
    pParams[0] = static_cast<T>(pointParams_.vAttenuation.x);
    pParams[1] = static_cast<T>(pointParams_.vAttenuation.y);
    pParams[2] = static_cast<T>(pointParams_.vAttenuation.z);
    break;

  case GL_DEPTH_RANGE:
    pParams[0] = static_cast<T>(depthRange_.fNear);
    pParams[1] = static_cast<T>(depthRange_.fFar);
    break;

  case GL_POINT_SIZE:
    pParams[0] = static_cast<T>(fPointSize_);
    break;

  case GL_LINE_WIDTH:
    pParams[0] = static_cast<T>(fLineWidth_);
    break;

  default:
    __glError(GL_INVALID_ENUM,
              "GLContext::get() failed, invalid pname parameter: %d.\r\n",
              pname);
    return;
  }
}

template <typename T>
inline GLbitfield GLContext::queryMatrix(T *pMantissa, GLint exponent[16]) {
  const MATRIX44 &matrix = pMatrixStack_->getMatrix();

  for (uint32_t i = 0; i < 16; ++i) {
    pMantissa[i] = static_cast<T>(matrix._m[i]);
    exponent[i] = 0;
  }

  return 0;
}
