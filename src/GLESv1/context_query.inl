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

template <class T>
inline void CGLContext::TGetClipPlane(GLenum plane, T eqn[4]) {
  if ((plane - GL_CLIP_PLANE0) >= MAX_CLIPPLANES) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::TGetClipPlane() failed, ")
                               _T("invalid plane parameter: %d.\r\n"),
              plane);
    return;
  }

  uint32_t index = plane - GL_CLIP_PLANE0;

  eqn[0] = Math::TCast<T>(m_vClipPlanesES[index].x);
  eqn[1] = Math::TCast<T>(m_vClipPlanesES[index].y);
  eqn[2] = Math::TCast<T>(m_vClipPlanesES[index].z);
  eqn[3] = Math::TCast<T>(m_vClipPlanesES[index].w);
}

template <class T>
inline void CGLContext::TGetLight(GLenum light, GLenum pname, T *pParams) {
  ASSERT(pParams);

  if ((light - GL_LIGHT0) >= MAX_LIGHTS) {
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::TGetLight() failed, invalid light parameter: %d.\r\n"),
        light);
    return;
  }

  uint32_t index = light - GL_LIGHT0;
  Light &_light = m_lights[index];

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
    pParams[0] = Math::TCast<T>(_light.GetColor(pname).x);
    pParams[1] = Math::TCast<T>(_light.GetColor(pname).y);
    pParams[2] = Math::TCast<T>(_light.GetColor(pname).z);
    pParams[3] = Math::TCast<T>(_light.GetColor(pname).w);
    break;

  case GL_POSITION:
    pParams[0] = Math::TCast<T>(_light.vPosition.x);
    pParams[1] = Math::TCast<T>(_light.vPosition.y);
    pParams[2] = Math::TCast<T>(_light.vPosition.z);
    pParams[3] = Math::TCast<T>(_light.vPosition.w);
    break;

  case GL_SPOT_DIRECTION:
    pParams[0] = Math::TCast<T>(_light.vSpotDirection.x);
    pParams[1] = Math::TCast<T>(_light.vSpotDirection.y);
    pParams[2] = Math::TCast<T>(_light.vSpotDirection.z);
    break;

  case GL_SPOT_EXPONENT:
    pParams[0] = Math::TCast<T>(_light.fSpotExponent);
    break;

  case GL_SPOT_CUTOFF:
    pParams[0] = Math::TCast<T>(_light.fSpotCutOff);
    break;

  case GL_CONSTANT_ATTENUATION:
  case GL_LINEAR_ATTENUATION:
  case GL_QUADRATIC_ATTENUATION:
    pParams[0] = Math::TCast<T>(_light.GetAttenuation(pname));
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::TGetLight() failed, invalid pname parameter: %d.\r\n"),
        pname);
    return;
  }
}

template <class T>
inline void CGLContext::TGetMaterial(GLenum face, GLenum pname, T *pParams) {
  ASSERT(pParams);

  if (face != GL_FRONT && face != GL_BACK) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::TGetMaterial() failed, invalid ")
                               _T("face parameter: %d.\r\n"),
              face);
    return;
  }

  switch (pname) {
  case GL_AMBIENT:
  case GL_DIFFUSE:
  case GL_SPECULAR:
    pParams[0] = Math::TCast<T>(m_material.GetColor(pname).x);
    pParams[1] = Math::TCast<T>(m_material.GetColor(pname).y);
    pParams[2] = Math::TCast<T>(m_material.GetColor(pname).z);
    pParams[3] = Math::TCast<T>(m_material.GetColor(pname).w);
    break;

  case GL_EMISSION:
    pParams[0] = Math::TCast<T>(m_vMatEmissive.x);
    pParams[1] = Math::TCast<T>(m_vMatEmissive.y);
    pParams[2] = Math::TCast<T>(m_vMatEmissive.z);
    pParams[3] = Math::TCast<T>(m_vMatEmissive.w);
    break;

  case GL_SHININESS:
    pParams[0] = Math::TCast<T>(m_fMatShininess);
    break;

  default:
    __glError(GL_INVALID_ENUM, _T("CGLContext::TGetMaterial() failed, invalid ")
                               _T("pname parameter: %d.\r\n"),
              pname);
    return;
  }
}

template <class T>
inline void CGLContext::TGetTexEnv(GLenum env, GLenum pname, T *pParams) {
  ASSERT(pParams);

  TexUnit &texUnit = m_texUnits[m_activeTexture];

  switch (env) {
  case GL_TEXTURE_ENV:
    switch (pname) {
    case GL_TEXTURE_ENV_MODE:
      pParams[0] = Math::TCast<T>(EnumFromTexEnv(texUnit.EnvMode));
      break;

    case GL_TEXTURE_ENV_COLOR:
      pParams[0] = Math::TFromUNORM8<T>(texUnit.cEnvColor.r);
      pParams[1] = Math::TFromUNORM8<T>(texUnit.cEnvColor.g);
      pParams[2] = Math::TFromUNORM8<T>(texUnit.cEnvColor.b);
      pParams[3] = Math::TFromUNORM8<T>(texUnit.cEnvColor.a);
      break;

    default:
      __glError(GL_INVALID_ENUM, _T("CGLContext::TGetTexEnv() failed, invalid ")
                                 _T("pname parameter: %d.\r\n"),
                pname);
      return;
    }

    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T("CGLContext::TGetTexEnv() failed, invalid env parameter: %d.\r\n"),
        env);
    return;
  }
}

template <class T>
inline void CGLContext::TGetTexParameter(GLenum target, GLenum pname,
                                         T *pParams) {
  ASSERT(pParams);

  if (target != GL_TEXTURE_2D) {
    __glError(GL_INVALID_ENUM, _T("CGLContext::TGetTexParameter() failed, ")
                               _T("invalid target parameter: %d.\r\n"),
              target);
    return;
  }

  auto pTexture = this->GetTexture(m_activeTexture);
  ASSERT(pTexture);

  switch (pname) {
  case GL_TEXTURE_MIN_FILTER:
    pParams[0] = Math::TCast<T>(EnumFromTexFilter(pTexture->Params.MinFilter));
    break;

  case GL_TEXTURE_MAG_FILTER:
    pParams[0] = Math::TCast<T>(EnumFromTexFilter(pTexture->Params.MagFilter));
    break;

  case GL_TEXTURE_WRAP_S:
    pParams[0] = Math::TCast<T>(EnumFromTexAddress(pTexture->Params.AddressU));
    break;

  case GL_TEXTURE_WRAP_T:
    pParams[0] = Math::TCast<T>(EnumFromTexAddress(pTexture->Params.AddressV));
    break;

  case GL_GENERATE_MIPMAP:
    pParams[0] = Math::TCast<T>(pTexture->bGenMipMaps);
    break;

  default:
    __glError(GL_INVALID_ENUM, _T("CGLContext::TGetTexParameter() failed, ")
                               _T("invalid pname parameter: %d.\r\n"),
              pname);
    return;
  }
}

template <> inline void CGLContext::TGet<bool>(GLenum pname, bool *pParams) {
  ASSERT(pParams);

  switch (pname) {
  case GL_LIGHT_MODEL_TWO_SIDE:
    pParams[0] = m_caps.TwoSidedLighting;
    break;

  case GL_COLOR_WRITEMASK:
    pParams[0] = (m_cColorWriteMask.r != 0);
    pParams[1] = (m_cColorWriteMask.g != 0);
    pParams[2] = (m_cColorWriteMask.b != 0);
    pParams[3] = (m_cColorWriteMask.a != 0);
    break;

  case GL_DEPTH_WRITEMASK:
    pParams[0] = (m_depthWriteMask != 0);
    break;

  case GL_SAMPLE_COVERAGE_INVERT:
    pParams[0] = m_sampleCoverage.bInvert;
    break;

  case GL_COORD_REPLACE_OES:
    pParams[0] = m_texUnits[m_activeTexture].bCoordReplace;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TGet() failed, invalid pname parameter: %d.\r\n"),
              pname);
    return;
  }
}

template <> inline void CGLContext::TGet<int>(GLenum pname, int *pParams) {
  ASSERT(pParams);

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
    pParams[0] = m_pSurfDraw->GetAttribute(GL_SUBPIXEL_BITS);
    break;

  case GL_RED_BITS:
    pParams[0] = m_pSurfDraw->GetAttribute(GL_RED_BITS);
    break;

  case GL_GREEN_BITS:
    pParams[0] = m_pSurfDraw->GetAttribute(GL_GREEN_BITS);
    break;

  case GL_BLUE_BITS:
    pParams[0] = m_pSurfDraw->GetAttribute(GL_BLUE_BITS);
    break;

  case GL_ALPHA_BITS:
    pParams[0] = m_pSurfDraw->GetAttribute(GL_ALPHA_BITS);
    break;

  case GL_DEPTH_BITS:
    pParams[0] = m_pSurfDraw->GetAttribute(GL_DEPTH_BITS);
    break;

  case GL_STENCIL_BITS:
    pParams[0] = m_pSurfDraw->GetAttribute(GL_STENCIL_BITS);
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
    pParams[0] = m_pMsModelView->GetSize();
    break;

  case GL_MAX_PROJECTION_STACK_DEPTH:
    pParams[0] = m_pMsProjection->GetSize();
    break;

  case GL_MAX_TEXTURE_STACK_DEPTH:
    pParams[0] = m_pMsTexCoords[m_activeTexture]->GetSize();
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
    pParams[0] = DecodeDataSize(m_positionArray.Format);
    break;

  case GL_VERTEX_ARRAY_STRIDE:
    pParams[0] = m_positionArray.Stride;
    break;

  case GL_VERTEX_ARRAY_TYPE:
    pParams[0] = DecodeDataType(m_positionArray.Format);
    break;

  case GL_NORMAL_ARRAY_STRIDE:
    pParams[0] = m_normalArray.Stride;
    break;

  case GL_NORMAL_ARRAY_TYPE:
    pParams[0] = DecodeDataType(m_normalArray.Format);
    break;

  case GL_COLOR_ARRAY_SIZE:
    pParams[0] = DecodeDataSize(m_colorArray.Format);
    break;

  case GL_COLOR_ARRAY_STRIDE:
    pParams[0] = m_colorArray.Stride;
    break;

  case GL_COLOR_ARRAY_TYPE:
    pParams[0] = DecodeDataType(m_colorArray.Format);
    break;

  case GL_TEXTURE_COORD_ARRAY_SIZE:
    pParams[0] = DecodeDataSize(m_texCoordArrays[m_clientActiveTexture].Format);
    break;

  case GL_TEXTURE_COORD_ARRAY_STRIDE:
    pParams[0] = m_texCoordArrays[m_clientActiveTexture].Stride;
    break;

  case GL_TEXTURE_COORD_ARRAY_TYPE:
    pParams[0] = DecodeDataType(m_texCoordArrays[m_clientActiveTexture].Format);
    break;

  case GL_POINT_SIZE_ARRAY_TYPE_OES:
    pParams[0] = DecodeDataType(m_pointSizeArray.Format);
    break;

  case GL_POINT_SIZE_ARRAY_STRIDE_OES:
    pParams[0] = m_pointSizeArray.Stride;
    break;

  case GL_VERTEX_ARRAY_BUFFER_BINDING:
    pParams[0] = m_positionArray.GetBufferHandle();
    break;

  case GL_NORMAL_ARRAY_BUFFER_BINDING:
    pParams[0] = m_normalArray.GetBufferHandle();
    break;

  case GL_COLOR_ARRAY_BUFFER_BINDING:
    pParams[0] = m_colorArray.GetBufferHandle();
    break;

  case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
    pParams[0] = m_texCoordArrays[m_clientActiveTexture].GetBufferHandle();
    break;

  case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
    pParams[0] = m_pointSizeArray.GetBufferHandle();
    break;

  case GL_ARRAY_BUFFER_BINDING:
    pParams[0] = this->GetBufferObjectHandle(GL_ARRAY_BUFFER);
    break;

  case GL_ELEMENT_ARRAY_BUFFER_BINDING:
    pParams[0] = this->GetBufferObjectHandle(GL_ELEMENT_ARRAY_BUFFER);
    break;

  case GL_TEXTURE_BINDING_2D:
    pParams[0] = this->GetBoundTextureHandle();
    break;

  case GL_UNPACK_ALIGNMENT:
    pParams[0] = m_unpackAlignment;
    break;

  case GL_PACK_ALIGNMENT:
    pParams[0] = m_packAlignment;
    break;

  case GL_STENCIL_CLEAR_VALUE:
    pParams[0] = m_clearStencil;
    break;

  case GL_SCISSOR_BOX:
    pParams[0] = m_scissor.left;
    pParams[1] = m_scissor.top;
    pParams[2] = m_scissor.right - m_scissor.left;
    pParams[3] = m_scissor.bottom - m_scissor.top;
    break;

  case GL_VIEWPORT:
    pParams[0] = m_viewport.left;
    pParams[1] = m_viewport.top;
    pParams[2] = m_viewport.right - m_viewport.left;
    pParams[3] = m_viewport.bottom - m_viewport.top;
    break;

  case GL_STENCIL_WRITEMASK:
    pParams[0] = m_stencilWriteMask;
    break;

  case GL_STENCIL_VALUE_MASK:
    pParams[0] = m_rasterData.StencilMask;
    break;

  case GL_STENCIL_REF:
    pParams[0] = m_rasterData.StencilRef;
    break;

  case GL_ALPHA_TEST_FUNC:
    pParams[0] = EnumFromCompareFunc(m_rasterStates.AlphaFunc);
    break;

  case GL_STENCIL_FUNC:
    pParams[0] = EnumFromCompareFunc(m_rasterStates.StencilFunc);
    break;

  case GL_STENCIL_FAIL:
    pParams[0] = EnumFromStencilOp(m_rasterStates.StencilFail);
    break;

  case GL_STENCIL_PASS_DEPTH_FAIL:
    pParams[0] = EnumFromStencilOp(m_rasterStates.StencilZFail);
    break;

  case GL_STENCIL_PASS_DEPTH_PASS:
    pParams[0] = EnumFromStencilOp(m_rasterStates.StencilZPass);
    break;

  case GL_DEPTH_FUNC:
    pParams[0] = EnumFromCompareFunc(m_rasterStates.DepthFunc);
    break;

  case GL_LOGIC_OP_MODE:
    pParams[0] = EnumFromLogicOp(m_rasterStates.LogicFunc);
    break;

  case GL_BLEND_SRC:
    pParams[0] = EnumFromBlendFunc(m_rasterStates.BlendSrc);
    break;

  case GL_BLEND_DST:
    pParams[0] = EnumFromBlendFunc(m_rasterStates.BlendDst);
    break;

  case GL_FOG_MODE:
    pParams[0] = EnumFromFogMode(m_fog.Mode);
    break;

  case GL_SHADE_MODEL:
    pParams[0] = EnumFromShadeModel(m_caps.ShadeModel);
    break;

  case GL_CULL_FACE_MODE:
    pParams[0] = EnumFromCullFace(m_cullStates.CullFace);
    break;

  case GL_FRONT_FACE:
    pParams[0] = EnumFromFrontFace(m_cullStates.FrontFace);
    break;

  case GL_MATRIX_MODE:
    pParams[0] = m_matrixMode;
    break;

  case GL_ACTIVE_TEXTURE:
  case GL_CLIENT_ACTIVE_TEXTURE:
    pParams[0] = GL_TEXTURE0;
    break;

  case GL_PERSPECTIVE_CORRECTION_HINT:
    pParams[0] = EnumFromHint(m_hints.Perspective);
    break;

  case GL_POINT_SMOOTH_HINT:
    pParams[0] = EnumFromHint(m_hints.PointSmooth);
    break;

  case GL_LINE_SMOOTH_HINT:
    pParams[0] = EnumFromHint(m_hints.LineSmooth);
    break;

  case GL_FOG_HINT:
    pParams[0] = EnumFromHint(m_hints.Fog);
    break;

  case GL_GENERATE_MIPMAP_HINT:
    pParams[0] = EnumFromHint(m_hints.GenerateMipmap);
    break;

  // GL Extensions
  case GL_MAX_VERTEX_UNITS_OES:
  case GL_MAX_PALETTE_MATRICES_OES:
    pParams[0] = 0;
    break;

  default:
    __glError(GL_INVALID_ENUM,
              _T("CGLContext::TGet() failed, invalid pname parameter: %d.\r\n"),
              pname);
    return;
  }
}

template <class T> inline void CGLContext::TGet(GLenum pname, T *pParams) {
  ASSERT(pParams);

  switch (pname) {
  case GL_CURRENT_COLOR:
    pParams[0] = Math::TCast<T>(m_vColor.x);
    pParams[1] = Math::TCast<T>(m_vColor.y);
    pParams[2] = Math::TCast<T>(m_vColor.z);
    pParams[3] = Math::TCast<T>(m_vColor.w);
    break;

  case GL_DEPTH_CLEAR_VALUE:
    pParams[0] = Math::TCast<T>(m_fClearDepth);
    break;

  case GL_CURRENT_TEXTURE_COORDS:
    pParams[0] = Math::TCast<T>(m_vTexCoords[m_activeTexture].x);
    pParams[1] = Math::TCast<T>(m_vTexCoords[m_activeTexture].y);
    pParams[2] = Math::TCast<T>(fZERO);
    pParams[3] = Math::TCast<T>(fONE);
    break;

  case GL_CURRENT_NORMAL:
    pParams[0] = Math::TCast<T>(m_vNormal.x);
    pParams[1] = Math::TCast<T>(m_vNormal.y);
    pParams[2] = Math::TCast<T>(m_vNormal.z);
    break;

  case GL_MODELVIEW_MATRIX:
    m_pMsModelView->TGetTop<T>(pParams);
    break;

  case GL_PROJECTION_MATRIX:
    m_pMsProjection->TGetTop<T>(pParams);
    break;

  case GL_TEXTURE_MATRIX:
    m_pMsTexCoords[m_activeTexture]->TGetTop<T>(pParams);
    break;

  case GL_FOG_COLOR:
    pParams[0] = Math::TCast<T>(m_vFogColor.x);
    pParams[1] = Math::TCast<T>(m_vFogColor.y);
    pParams[2] = Math::TCast<T>(m_vFogColor.z);
    pParams[3] = Math::TCast<T>(m_vFogColor.w);
    break;

  case GL_FOG_DENSITY:
    pParams[0] = Math::TCast<T>(m_fog.GetFactor(GL_FOG_DENSITY));
    break;

  case GL_FOG_START:
    pParams[0] = Math::TCast<T>(m_fog.GetFactor(GL_FOG_START));
    break;

  case GL_FOG_END:
    pParams[0] = Math::TCast<T>(m_fog.GetFactor(GL_FOG_END));
    break;

  case GL_ALPHA_TEST_REF:
    pParams[0] = Math::TFromUNORM8<T>(m_rasterData.AlphaRef);
    break;

  case GL_LIGHT_MODEL_AMBIENT:
    pParams[0] = Math::TCast<T>(m_vLightModelAmbient.x);
    pParams[1] = Math::TCast<T>(m_vLightModelAmbient.y);
    pParams[2] = Math::TCast<T>(m_vLightModelAmbient.z);
    pParams[3] = Math::TCast<T>(m_vLightModelAmbient.w);
    break;

  case GL_COLOR_CLEAR_VALUE:
    pParams[0] = Math::TCast<T>(m_vClearColor.x);
    pParams[1] = Math::TCast<T>(m_vClearColor.y);
    pParams[2] = Math::TCast<T>(m_vClearColor.z);
    pParams[3] = Math::TCast<T>(m_vClearColor.w);
    break;

  case GL_POLYGON_OFFSET_UNITS:
    pParams[0] = Math::TCast<T>(m_polygonOffset.fUnits);
    break;

  case GL_POLYGON_OFFSET_FACTOR:
    pParams[0] = Math::TCast<T>(m_polygonOffset.fFactor);
    break;

  case GL_SAMPLE_COVERAGE_VALUE:
    pParams[0] = Math::TCast<T>(m_sampleCoverage.fValue);
    break;

  case GL_POINT_SIZE_MIN:
    pParams[0] = Math::TCast<T>(m_pointParams.Get(GL_POINT_SIZE_MIN));
    break;

  case GL_POINT_SIZE_MAX:
    pParams[0] = Math::TCast<T>(m_pointParams.Get(GL_POINT_SIZE_MAX));
    break;

  case GL_POINT_FADE_THRESHOLD_SIZE:
    pParams[0] =
        Math::TCast<T>(m_pointParams.Get(GL_POINT_FADE_THRESHOLD_SIZE));
    break;

  case GL_POINT_DISTANCE_ATTENUATION:
    pParams[0] = Math::TCast<T>(m_pointParams.vAttenuation.x);
    pParams[1] = Math::TCast<T>(m_pointParams.vAttenuation.y);
    pParams[2] = Math::TCast<T>(m_pointParams.vAttenuation.z);
    break;

  case GL_DEPTH_RANGE:
    pParams[0] = Math::TCast<T>(m_depthRange.fNear);
    pParams[1] = Math::TCast<T>(m_depthRange.fFar);
    break;

  case GL_POINT_SIZE:
    pParams[0] = Math::TCast<T>(m_fPointSize);
    break;

  case GL_LINE_WIDTH:
    pParams[0] = Math::TCast<T>(m_fLineWidth);
    break;

  default:
    __glError(
        GL_INVALID_ENUM,
        _T
                    ("CGLContext::TGet() failed, invalid pname parameter: %d.\r\n"),
        pname);
    return;
  }
}

template <class T>
inline GLbitfield CGLContext::TQueryMatrix(T *pMantissa, GLint exponent[16]) {
  const MATRIX44 &matrix = m_pMatrixStack->GetMatrix();

  for (uint32_t i = 0; i < 16; ++i) {
    pMantissa[i] = static_cast<T>(matrix._m[i]);
    exponent[i] = 0;
  }

  return 0;
}
