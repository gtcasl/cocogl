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

CGLContext::CGLContext(CHandleTable *pHandles, CRasterCache *pRasterCache,
                       CGLContext *pCtxShared) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  assert(pHandles);
  pHandles->AddRef();
  handles_ = pHandles;

  assert(pRasterCache);
  pRasterCache->AddRef();
  pRasterCache_ = pRasterCache;

  if (pCtxShared) {
    pCtxShared->AddRef();
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

CGLContext::~CGLContext() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

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

  auto enumerator = handles_->GetEnumerator(this);
  while (!enumerator.IsEnd()) {
    reinterpret_cast<IObject *>(enumerator.RemoveNext())->Release();
  }

  __safeRelease(handles_);

#ifdef GL_COCOJIT
  __safeRelease(pCGAssembler_);
#endif
}

GLenum CGLContext::Create(CGLContext **ppContext, CHandleTable *pHandles,
                          CRasterCache *pRasterCache, CGLContext *pCtxShared) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  assert(pHandles && ppContext);

  // Create a new context object
  auto pContext = new CGLContext(pHandles, pRasterCache, pCtxShared);
  if (nullptr == pContext) {
    __glLogError(_T("CGLContext allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  pContext->AddRef();

  // Initialize the context
  err = pContext->Initialize();
  if (__glFailed(err)) {
    __safeRelease(pContext);
    __glLogError(_T("CGLContext::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  *ppContext = pContext;

  return GL_NO_ERROR;
}

GLenum CGLContext::Initialize() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  // Initialize the matrix stacks
  err = CMatrixStack::Create(&pMsModelView_, MODELVIEW_STACK_SIZE);
  if (__glFailed(err)) {
    __glLogError(_T("CMatrixStack::Init() failed, err = %d.\r\n"), err);
    return err;
  }

  err = CMatrixStack::Create(&pMsProjection_, PROJECTION_STACK_SIZE);
  if (__glFailed(err)) {
    __glLogError(_T("CMatrixStack::Init() failed, err = %d.\r\n"), err);
    return err;
  }

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    err = CMatrixStack::Create(&pMsTexCoords_[i], TEXTURE_STACK_SIZE);
    if (__glFailed(err)) {
      __glLogError(_T("CMatrixStack::Init() failed, err = %d.\r\n"), err);
      return err;
    }
  }

  err = CTexture::Create(&pTexDefault_);
  if (__glFailed(err)) {
    __glLogError(_T("CTexture::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  err = CBuffer::Create(&pBufDefault_);
  if (__glFailed(err)) {
    __glLogError(_T("CBuffer::Create() failed, err = %d.\r\n"), err);
    return err;
  }

  vClipPlanesES_.resize(MAX_CLIPPLANES);

  vClipPlanesCS_.resize(MAX_CLIPPLANES);

  lights_.resize(MAX_LIGHTS);

#ifdef GL_COCOJIT
  // Create the CG assembler
  err = GLERROR_FROM_HRESULT(CG::CAssembler::Create(&pCGAssembler_));
  if (__glFailed(err)) {
    __glLogError(_T("CG::CAssembler::Create() failed, err = %d.\r\n"), err);
    return err;
  }
#endif

  //--
  this->ShadeModel(GL_SMOOTH);

  this->ClearColor(fZERO, fZERO, fZERO, fZERO);
  this->ColorMask(true, true, true, true);

  this->ClearDepth(fONE);
  this->DepthMask(true);

  this->ClearStencil(0);
  this->StencilMask(0xffffffff);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->ActiveTexture(GL_TEXTURE0 + i);
    this->MatrixMode(GL_TEXTURE);
    this->LoadIdentity();
  }

  this->MatrixMode(GL_PROJECTION);
  this->LoadIdentity();

  this->MatrixMode(GL_MODELVIEW);
  this->LoadIdentity();

  this->BindBuffer(GL_ARRAY_BUFFER, 0);
  this->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  this->ClientState(GL_VERTEX_ARRAY, false);
  this->VertexPointer(4, GL_FLOAT, 0, nullptr);

  this->ClientState(GL_NORMAL_ARRAY, false);
  this->NormalPointer(GL_FLOAT, 0, nullptr);

  this->ClientState(GL_COLOR_ARRAY, false);
  this->ColorPointer(4, GL_FLOAT, 0, nullptr);

  this->ClientState(GL_POINT_SIZE_ARRAY_OES, false);
  this->PointSizePointerOES(GL_FLOAT, 0, nullptr);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->ClientActiveTexture(GL_TEXTURE0 + i);
    this->ClientState(GL_TEXTURE_COORD_ARRAY, false);
    this->TexCoordPointer(4, GL_FLOAT, 0, nullptr);
  }

  this->Normal(fZERO, fZERO, fONE);

  this->Color(fONE, fONE, fONE, fONE);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->MultiTexCoord(GL_TEXTURE0 + i, fZERO, fZERO, fZERO, fONE);
  }

  for (uint32_t i = 0; i < MAX_LIGHTS; ++i) {
    GLenum light = GL_LIGHT0 + i;

    this->TLight<floatf>(light, GL_AMBIENT,
                         VECTOR4(fZERO, fZERO, fZERO, fONE).m);

    if (GL_LIGHT0 == light) {
      this->TLight<floatf>(light, GL_DIFFUSE,
                           VECTOR4(fONE, fONE, fONE, fONE).m);
      this->TLight<floatf>(light, GL_SPECULAR,
                           VECTOR4(fONE, fONE, fONE, fONE).m);
    } else {
      this->TLight<floatf>(light, GL_DIFFUSE,
                           VECTOR4(fZERO, fZERO, fZERO, fZERO).m);
      this->TLight<floatf>(light, GL_SPECULAR,
                           VECTOR4(fZERO, fZERO, fZERO, fZERO).m);
    }

    this->TLight<floatf>(light, GL_POSITION,
                         VECTOR4(fZERO, fZERO, fONE, fZERO).m);
    this->TLight<floatf>(light, GL_SPOT_DIRECTION,
                         VECTOR3(fZERO, fZERO, -fONE).m);
    this->TLight<floatf>(light, GL_SPOT_EXPONENT, TAddressOf<floatf>(fZERO));
    this->TLight<floatf>(light, GL_SPOT_CUTOFF, TAddressOf<floatf>(f180));
    this->TLight<floatf>(light, GL_CONSTANT_ATTENUATION,
                         TAddressOf<floatf>(fONE));
    this->TLight<floatf>(light, GL_LINEAR_ATTENUATION,
                         TAddressOf<floatf>(fZERO));
    this->TLight<floatf>(light, GL_QUADRATIC_ATTENUATION,
                         TAddressOf<floatf>(fZERO));
  }

  this->TMaterial<floatf>(GL_FRONT_AND_BACK, GL_AMBIENT,
                          VECTOR4(f02, f02, f02, fONE).m);
  this->TMaterial<floatf>(GL_FRONT_AND_BACK, GL_DIFFUSE,
                          VECTOR4(f08, f08, f08, fONE).m);
  this->TMaterial<floatf>(GL_FRONT_AND_BACK, GL_SPECULAR,
                          VECTOR4(fZERO, fZERO, fZERO, fONE).m);
  this->TMaterial<floatf>(GL_FRONT_AND_BACK, GL_EMISSION,
                          VECTOR4(fZERO, fZERO, fZERO, fONE).m);
  this->TMaterial<floatf>(GL_FRONT_AND_BACK, GL_SHININESS,
                          TAddressOf<floatf>(fZERO));

  this->TFog<fixed16>(GL_FOG_MODE, TAddressOf<fixed16>(fixed16::make(GL_EXP)));
  this->TFog<floatf>(GL_FOG_DENSITY, TAddressOf<floatf>(fONE));
  this->TFog<floatf>(GL_FOG_START, TAddressOf<floatf>(fZERO));
  this->TFog<floatf>(GL_FOG_END, TAddressOf<floatf>(fONE));
  this->TFog<floatf>(GL_FOG_COLOR, VECTOR4(fZERO, fZERO, fZERO, fZERO).m);

  this->TLightModel<floatf>(GL_LIGHT_MODEL_TWO_SIDE, TAddressOf<floatf>(fZERO));
  this->TLightModel<floatf>(GL_LIGHT_MODEL_AMBIENT,
                            VECTOR4(f02, f02, f02, fONE).m);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->ClipPlane(GL_CLIP_PLANE0 + i, VECTOR4(fZERO, fZERO, fZERO, fZERO));
  }

  this->DepthRange(fZERO, fONE);

  this->FrontFace(GL_CCW);
  this->CullFace(GL_BACK);

  this->PixelStorei(GL_PACK_ALIGNMENT, 4);
  this->PixelStorei(GL_UNPACK_ALIGNMENT, 4);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->ActiveTexture(GL_TEXTURE0 + i);

    this->BindTexture(GL_TEXTURE_2D, 0);

    this->TTexEnv<fixed16>(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
                           TAddressOf<fixed16>(fixed16::make(GL_MODULATE)));

    this->TTexEnv<floatf>(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,
                          VECTOR4(fZERO, fZERO, fZERO, fZERO).m);

    this->TTexEnv<fixed16>(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES,
                           TAddressOf<fixed16>(fixed16::make(GL_FALSE)));
  }

  this->AlphaFunc(GL_ALWAYS, fZERO);
  this->StencilFunc(GL_ALWAYS, 0, 0xffffffff);
  this->StencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  this->DepthFunc(GL_LESS);
  this->BlendFunc(GL_ONE, GL_ZERO);
  this->LogicOp(GL_COPY);

  this->SampleCoverage(fONE, GL_FALSE);

  this->Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
  this->Hint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
  this->Hint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  this->Hint(GL_FOG_HINT, GL_DONT_CARE);
  this->Hint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);

  this->PolygonOffset(fZERO, fZERO);

  this->PointSize(fONE);
  this->TPointParameter<floatf>(GL_POINT_SIZE_MIN, TAddressOf<floatf>(fZERO));
  this->TPointParameter<floatf>(GL_POINT_SIZE_MAX, TAddressOf<floatf>(fONE));
  this->TPointParameter<floatf>(GL_POINT_FADE_THRESHOLD_SIZE,
                                TAddressOf<floatf>(fONE));
  this->TPointParameter<floatf>(GL_POINT_DISTANCE_ATTENUATION,
                                VECTOR3(fONE, fZERO, fZERO).m);
  this->LineWidth(fONE);

  //--
  this->Activate(GL_FOG, false);
  this->Activate(GL_LIGHTING, false);

  for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
    this->ActiveTexture(GL_TEXTURE0 + i);
    this->Activate(GL_TEXTURE_2D, false);
  }

  this->Activate(GL_CULL_FACE, false);
  this->Activate(GL_ALPHA_TEST, false);
  this->Activate(GL_BLEND, false);
  this->Activate(GL_COLOR_LOGIC_OP, false);
  this->Activate(GL_DITHER, true);
  this->Activate(GL_STENCIL_TEST, false);
  this->Activate(GL_DEPTH_TEST, false);
  this->Activate(GL_POINT_SMOOTH, false);
  this->Activate(GL_POINT_SPRITE_OES, false);
  this->Activate(GL_LINE_SMOOTH, false);
  this->Activate(GL_SCISSOR_TEST, false);
  this->Activate(GL_COLOR_MATERIAL, false);
  this->Activate(GL_NORMALIZE, false);

  this->Activate(GL_CLIP_PLANE0, false);
  this->Activate(GL_CLIP_PLANE1, false);
  this->Activate(GL_CLIP_PLANE2, false);
  this->Activate(GL_CLIP_PLANE3, false);
  this->Activate(GL_CLIP_PLANE4, false);
  this->Activate(GL_CLIP_PLANE5, false);

  this->Activate(GL_LIGHT0, false);
  this->Activate(GL_LIGHT1, false);
  this->Activate(GL_LIGHT2, false);
  this->Activate(GL_LIGHT3, false);
  this->Activate(GL_LIGHT4, false);
  this->Activate(GL_LIGHT5, false);
  this->Activate(GL_LIGHT6, false);
  this->Activate(GL_LIGHT7, false);

  this->Activate(GL_RESCALE_NORMAL, false);
  this->Activate(GL_POLYGON_OFFSET_FILL, false);
  this->Activate(GL_MULTISAMPLE, true);
  this->Activate(GL_SAMPLE_ALPHA_TO_COVERAGE, false);
  this->Activate(GL_SAMPLE_ALPHA_TO_ONE, false);
  this->Activate(GL_SAMPLE_COVERAGE, false);

  this->ActiveTexture(GL_TEXTURE0);
  this->ClientActiveTexture(GL_TEXTURE0);

  return GL_NO_ERROR;
}

void CGLContext::SetDrawSurface(CGLSurface *pSurface) {
  if (pSurface) {
    pSurface->AddRef();
  }

  if (pSurfDraw_) {
    pSurfDraw_->Release();
  }

  pSurfDraw_ = pSurface;
  dirtyFlags_.ScissorRECT = 1;
  dirtyFlags_.ClearColor = 1;
  dirtyFlags_.ColorWriteMask = 1;

  if (pSurface) {
    // Initialize surface specific states
    GLSurfaceDesc colorDesc;
    pSurface->GetColorDesc(&colorDesc);

    this->Viewport(0, 0, colorDesc.Width, colorDesc.Height);
    this->Scissor(0, 0, colorDesc.Width, colorDesc.Height);

    rasterData_.pColorBits = reinterpret_cast<uint8_t *>(colorDesc.pBits);
    rasterData_.ColorFormat = colorDesc.Format;
    rasterData_.ColorPitch = colorDesc.Pitch;

    GLSurfaceDesc depthStencilDesc;
    pSurface->GetDepthStencilDesc(&depthStencilDesc);

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

void CGLContext::SetReadSurface(CGLSurface *pSurface) {
  if (pSurface) {
    pSurface->AddRef();
  }

  if (pSurfRead_) {
    pSurfRead_->Release();
  }

  pSurfRead_ = pSurface;
}
