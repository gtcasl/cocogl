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
#include "context.hpp"
#include "context_rast.inl"
#include "context_tnl.inl"

GLContext::GLContext(HandleTable *pHandles, CRasterCache *pRasterCache,
                     GLContext *pCtxShared) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(pHandles);
  pHandles->addRef();
  handles_ = pHandles;

  assert(pRasterCache);
  pRasterCache->addRef();
  pRasterCache_ = pRasterCache;

  if (pCtxShared) {
    pCtxShared->addRef();
  }

  pCtxShared_ = pCtxShared;

  pSurfDraw_ = nullptr;
  pSurfRead_ = nullptr;

  pTexDefault_ = nullptr;
  pBufDefault_ = nullptr;

  error_ = GL_NO_ERROR;

  for (uint32_t i = 0; i < BUFFER_OBJECTS_SIZE; ++i) {
    bufferObjects_[i] = nullptr;
  }

#ifdef GL_COCOJIT
  pCGAssembler_ = nullptr;
#endif
}

GLContext::~GLContext() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  __safeRelease(pRasterCache_);
  __safeRelease(pTexDefault_);
  __safeRelease(pBufDefault_);

  for (int i = 0; i < BUFFER_OBJECTS_SIZE; ++i) {
    __safeRelease(bufferObjects_[i]);
  }

  __safeRelease(pMsModelView_);
  __safeRelease(pMsProjection_);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    __safeRelease(pMsTexCoords_[i]);
  }

  __safeRelease(pCtxShared_);
  __safeRelease(pSurfDraw_);
  __safeRelease(pSurfRead_);

  auto enumerator = handles_->getEnumerator(this);
  while (!enumerator.isEnd()) {
    reinterpret_cast<IObject *>(enumerator.removeNext())->release();
  }

  __safeRelease(handles_);

#ifdef GL_COCOJIT
  __safeRelease(pCGAssembler_);
#endif
}

GLenum GLContext::Create(GLContext **ppContext, HandleTable *pHandles,
                         CRasterCache *pRasterCache, GLContext *pCtxShared) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  assert(pHandles && ppContext);

  // Create a new context object
  auto pContext = new GLContext(pHandles, pRasterCache, pCtxShared);
  if (nullptr == pContext) {
    __glLogError("GLContext allocation failed, out of memory.\r\n");
    return GL_OUT_OF_MEMORY;
  }

  pContext->addRef();

  // Initialize the context
  err = pContext->initialize();
  if (__glFailed(err)) {
    __glLogError("GLContext::initialize() failed, err = %d.\r\n", err);
    __safeRelease(pContext);
    return err;
  }

  *ppContext = pContext;

  return GL_NO_ERROR;
}

GLenum GLContext::initialize() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  GLenum err;

  // Initialize the matrix stacks
  err = CMatrixStack::Create(&pMsModelView_, MODELVIEW_STACK_SIZE);
  if (__glFailed(err)) {
    __glLogError("CMatrixStack::Create() failed, err = %d.\r\n", err);
    return err;
  }

  err = CMatrixStack::Create(&pMsProjection_, PROJECTION_STACK_SIZE);
  if (__glFailed(err)) {
    __glLogError("CMatrixStack::Create() failed, err = %d.\r\n", err);
    return err;
  }

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    err = CMatrixStack::Create(&pMsTexCoords_[i], TEXTURE_STACK_SIZE);
    if (__glFailed(err)) {
      __glLogError("CMatrixStack::Create() failed, err = %d.\r\n", err);
      return err;
    }
  }

  err = CTexture::Create(&pTexDefault_);
  if (__glFailed(err)) {
    __glLogError("CTexture::Create() failed, err = %d.\r\n", err);
    return err;
  }

  err = GLBuffer::Create(&pBufDefault_);
  if (__glFailed(err)) {
    __glLogError("GLBuffer::Create() failed, err = %d.\r\n", err);
    return err;
  }

  vClipPlanesES_.resize(MAX_CLIPPLANES);

  vClipPlanesCS_.resize(MAX_CLIPPLANES);

  lights_.resize(MAX_LIGHTS);

#ifdef GL_COCOJIT
  // Create the CG assembler
  err = GLERROR_FROM_HRESULT(CG::CAssembler::Create(&pCGAssembler_));
  if (__glFailed(err)) {
    __glLogError("CG::CAssembler::Create() failed, err = %d.\r\n", err);
    return err;
  }
#endif

  //--
  this->setShadeModel(GL_SMOOTH);

  this->clearColor(fZERO, fZERO, fZERO, fZERO);
  this->setColorMask(true, true, true, true);

  this->clearDepth(fONE);
  this->setDepthMask(true);

  this->clearStencil(0);
  this->setStencilMask(0xffffffff);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->setActiveTexture(GL_TEXTURE0 + i);
    this->setMatrixMode(GL_TEXTURE);
    this->loadIdentity();
  }

  this->setMatrixMode(GL_PROJECTION);
  this->loadIdentity();

  this->setMatrixMode(GL_MODELVIEW);
  this->loadIdentity();

  this->bindBuffer(GL_ARRAY_BUFFER, 0);
  this->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  this->setClientState(GL_VERTEX_ARRAY, false);
  this->setVertexPointer(4, GL_FLOAT, 0, nullptr);

  this->setClientState(GL_NORMAL_ARRAY, false);
  this->setNormalPointer(GL_FLOAT, 0, nullptr);

  this->setClientState(GL_COLOR_ARRAY, false);
  this->setColorPointer(4, GL_FLOAT, 0, nullptr);

  this->setClientState(GL_POINT_SIZE_ARRAY_OES, false);
  this->setPointSizePointerOES(GL_FLOAT, 0, nullptr);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->setClientActiveTexture(GL_TEXTURE0 + i);
    this->setClientState(GL_TEXTURE_COORD_ARRAY, false);
    this->setTexCoordPointer(4, GL_FLOAT, 0, nullptr);
  }

  this->setNormal(fZERO, fZERO, fONE);

  this->setColor(fONE, fONE, fONE, fONE);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->setMultiTexCoord(GL_TEXTURE0 + i, fZERO, fZERO, fZERO, fONE);
  }

  for (uint32_t i = 0; i < MAX_LIGHTS; ++i) {
    GLenum light = GL_LIGHT0 + i;

    this->setLightParameter<floatf>(light, GL_AMBIENT,
                                    VECTOR4(fZERO, fZERO, fZERO, fONE).m);

    if (GL_LIGHT0 == light) {
      this->setLightParameter<floatf>(light, GL_DIFFUSE,
                                      VECTOR4(fONE, fONE, fONE, fONE).m);
      this->setLightParameter<floatf>(light, GL_SPECULAR,
                                      VECTOR4(fONE, fONE, fONE, fONE).m);
    } else {
      this->setLightParameter<floatf>(light, GL_DIFFUSE,
                                      VECTOR4(fZERO, fZERO, fZERO, fZERO).m);
      this->setLightParameter<floatf>(light, GL_SPECULAR,
                                      VECTOR4(fZERO, fZERO, fZERO, fZERO).m);
    }

    this->setLightParameter<floatf>(light, GL_POSITION,
                                    VECTOR4(fZERO, fZERO, fONE, fZERO).m);
    this->setLightParameter<floatf>(light, GL_SPOT_DIRECTION,
                                    VECTOR3(fZERO, fZERO, -fONE).m);
    this->setLightParameter<floatf>(light, GL_SPOT_EXPONENT,
                                    TAddressOf<floatf>(fZERO));
    this->setLightParameter<floatf>(light, GL_SPOT_CUTOFF,
                                    TAddressOf<floatf>(f180));
    this->setLightParameter<floatf>(light, GL_CONSTANT_ATTENUATION,
                                    TAddressOf<floatf>(fONE));
    this->setLightParameter<floatf>(light, GL_LINEAR_ATTENUATION,
                                    TAddressOf<floatf>(fZERO));
    this->setLightParameter<floatf>(light, GL_QUADRATIC_ATTENUATION,
                                    TAddressOf<floatf>(fZERO));
  }

  this->setMaterial<floatf>(GL_FRONT_AND_BACK, GL_AMBIENT,
                            VECTOR4(f02, f02, f02, fONE).m);
  this->setMaterial<floatf>(GL_FRONT_AND_BACK, GL_DIFFUSE,
                            VECTOR4(f08, f08, f08, fONE).m);
  this->setMaterial<floatf>(GL_FRONT_AND_BACK, GL_SPECULAR,
                            VECTOR4(fZERO, fZERO, fZERO, fONE).m);
  this->setMaterial<floatf>(GL_FRONT_AND_BACK, GL_EMISSION,
                            VECTOR4(fZERO, fZERO, fZERO, fONE).m);
  this->setMaterial<floatf>(GL_FRONT_AND_BACK, GL_SHININESS,
                            TAddressOf<floatf>(fZERO));

  this->setFog<fixed16>(GL_FOG_MODE,
                        TAddressOf<fixed16>(fixed16::make(GL_EXP)));
  this->setFog<floatf>(GL_FOG_DENSITY, TAddressOf<floatf>(fONE));
  this->setFog<floatf>(GL_FOG_START, TAddressOf<floatf>(fZERO));
  this->setFog<floatf>(GL_FOG_END, TAddressOf<floatf>(fONE));
  this->setFog<floatf>(GL_FOG_COLOR, VECTOR4(fZERO, fZERO, fZERO, fZERO).m);

  this->setLightParameterModel<floatf>(GL_LIGHT_MODEL_TWO_SIDE,
                                       TAddressOf<floatf>(fZERO));
  this->setLightParameterModel<floatf>(GL_LIGHT_MODEL_AMBIENT,
                                       VECTOR4(f02, f02, f02, fONE).m);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->setClipPlane(GL_CLIP_PLANE0 + i, VECTOR4(fZERO, fZERO, fZERO, fZERO));
  }

  this->setDepthRange(fZERO, fONE);

  this->setFrontFace(GL_CCW);
  this->setCullFace(GL_BACK);

  this->setPixelStorei(GL_PACK_ALIGNMENT, 4);
  this->setPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->setActiveTexture(GL_TEXTURE0 + i);

    this->bindTexture(GL_TEXTURE_2D, 0);

    this->setTexEnv<fixed16>(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
                             TAddressOf<fixed16>(fixed16::make(GL_MODULATE)));

    this->setTexEnv<floatf>(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,
                            VECTOR4(fZERO, fZERO, fZERO, fZERO).m);

    this->setTexEnv<fixed16>(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES,
                             TAddressOf<fixed16>(fixed16::make(GL_FALSE)));
  }

  this->setAlphaFunc(GL_ALWAYS, fZERO);
  this->setStencilFunc(GL_ALWAYS, 0, 0xffffffff);
  this->setStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  this->setDepthFunc(GL_LESS);
  this->setBlendFunc(GL_ONE, GL_ZERO);
  this->setLogicOp(GL_COPY);

  this->setSampleCoverage(fONE, GL_FALSE);

  this->hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
  this->hint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
  this->hint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  this->hint(GL_FOG_HINT, GL_DONT_CARE);
  this->hint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);

  this->setPolygonOffset(fZERO, fZERO);

  this->setPointSize(fONE);
  this->setPointParameter<floatf>(GL_POINT_SIZE_MIN, TAddressOf<floatf>(fZERO));
  this->setPointParameter<floatf>(GL_POINT_SIZE_MAX, TAddressOf<floatf>(fONE));
  this->setPointParameter<floatf>(GL_POINT_FADE_THRESHOLD_SIZE,
                                  TAddressOf<floatf>(fONE));
  this->setPointParameter<floatf>(GL_POINT_DISTANCE_ATTENUATION,
                                  VECTOR3(fONE, fZERO, fZERO).m);
  this->setLineWidth(fONE);

  //--
  this->activate(GL_FOG, false);
  this->activate(GL_LIGHTING, false);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->setActiveTexture(GL_TEXTURE0 + i);
    this->activate(GL_TEXTURE_2D, false);
  }

  this->activate(GL_CULL_FACE, false);
  this->activate(GL_ALPHA_TEST, false);
  this->activate(GL_BLEND, false);
  this->activate(GL_COLOR_LOGIC_OP, false);
  this->activate(GL_DITHER, true);
  this->activate(GL_STENCIL_TEST, false);
  this->activate(GL_DEPTH_TEST, false);
  this->activate(GL_POINT_SMOOTH, false);
  this->activate(GL_POINT_SPRITE_OES, false);
  this->activate(GL_LINE_SMOOTH, false);
  this->activate(GL_SCISSOR_TEST, false);
  this->activate(GL_COLOR_MATERIAL, false);
  this->activate(GL_NORMALIZE, false);

  this->activate(GL_CLIP_PLANE0, false);
  this->activate(GL_CLIP_PLANE1, false);
  this->activate(GL_CLIP_PLANE2, false);
  this->activate(GL_CLIP_PLANE3, false);
  this->activate(GL_CLIP_PLANE4, false);
  this->activate(GL_CLIP_PLANE5, false);

  this->activate(GL_LIGHT0, false);
  this->activate(GL_LIGHT1, false);
  this->activate(GL_LIGHT2, false);
  this->activate(GL_LIGHT3, false);
  this->activate(GL_LIGHT4, false);
  this->activate(GL_LIGHT5, false);
  this->activate(GL_LIGHT6, false);
  this->activate(GL_LIGHT7, false);

  this->activate(GL_RESCALE_NORMAL, false);
  this->activate(GL_POLYGON_OFFSET_FILL, false);
  this->activate(GL_MULTISAMPLE, true);
  this->activate(GL_SAMPLE_ALPHA_TO_COVERAGE, false);
  this->activate(GL_SAMPLE_ALPHA_TO_ONE, false);
  this->activate(GL_SAMPLE_COVERAGE, false);

  this->setActiveTexture(GL_TEXTURE0);
  this->setClientActiveTexture(GL_TEXTURE0);

  return GL_NO_ERROR;
}

void GLContext::setDrawSurface(GLSurface *pSurface) {
  if (pSurface) {
    pSurface->addRef();
  }

  if (pSurfDraw_) {
    pSurfDraw_->release();
  }

  pSurfDraw_ = pSurface;
  dirtyFlags_.ScissorRECT = 1;
  dirtyFlags_.ClearColor = 1;
  dirtyFlags_.ColorWriteMask = 1;

  if (pSurface) {
    // Initialize surface specific states
    GLSurfaceDesc colorDesc;
    pSurface->getColorDesc(&colorDesc);

    this->setViewport(0, 0, colorDesc.Width, colorDesc.Height);
    this->setScissor(0, 0, colorDesc.Width, colorDesc.Height);

    rasterData_.pColorBits = reinterpret_cast<uint8_t *>(colorDesc.pBits);
    rasterData_.ColorFormat = colorDesc.Format;
    rasterData_.ColorPitch = colorDesc.Pitch;

    GLSurfaceDesc depthStencilDesc;
    pSurface->getDepthStencilDesc(&depthStencilDesc);

    rasterData_.pDepthStencilBits =
        reinterpret_cast<uint8_t *>(depthStencilDesc.pBits);
    rasterData_.DepthStencilFormat = depthStencilDesc.Format;
    rasterData_.DepthStencilPitch = depthStencilDesc.Pitch;
  } else {
    rasterData_.pColorBits = nullptr;
    rasterData_.ColorFormat = FORMAT_UNKNOWN;
    rasterData_.ColorPitch = 0;

    rasterData_.pDepthStencilBits = nullptr;
    rasterData_.DepthStencilFormat = FORMAT_UNKNOWN;
    rasterData_.DepthStencilPitch = 0;
  }
}

void GLContext::setReadSurface(GLSurface *pSurface) {
  if (pSurface) {
    pSurface->addRef();
  }

  if (pSurfRead_) {
    pSurfRead_->release();
  }

  pSurfRead_ = pSurface;
}
