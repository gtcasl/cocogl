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

#include "inputasm.hpp"

typedef void (*PFN_DECODEPOSITION)(VECTOR4 *pOut, const uint8_t *pbIn,
                                   uint32_t stride, uint32_t count);

class CTNL : public CInputAssembler {
public:
  typedef void (CTNL::*PFN_VERTEXDECODE)(uint32_t count);
  typedef void (CTNL::*PFN_DECODETEXCOORD)(uint32_t dstIndex, uint32_t srcIndex,
                                           uint32_t count);

  template <bool QuadraticAttenuation, eVertexFormat VertexFormat>
  void TProcessPointSize(uint32_t count);

  template <bool ColorMaterial, eVertexFormat ColorFormat,
            eVertexFormat NormalFormat>
  void TProcessLighting_OneSided(uint32_t count);

  template <bool ColorMaterial, eVertexFormat ColorFormat,
            eVertexFormat NormalFormat>
  void TProcessLighting_TwoSided(uint32_t count);

  template <eVertexFormat VertexFormat>
  void TProcessVertexColor(uint32_t count);

  template <bool Transform, eVertexFormat VertexFormat>
  void TProcessTexCoords(uint32_t dstIndex, uint32_t srcIndex, uint32_t count);

  template <eFogMode FogMode> void TProcessFog(uint32_t count);

protected:
  CTNL() {}
  ~CTNL() {}

  struct Lighting {
    Light lights[MAX_LIGHTS];
    MATRIX44 mModelViewInvT;
    Material material;
    VECTOR3 vNormNormal;
    VECTOR4 vLightModelAmbient;
    VECTOR4 vScaledAmbient;
    VECTOR4 vMatEmissive;
    Light *pActiveLights;
    floatf fMatShininess;
  };

  struct Clipping {
    VECTOR4 vClipPlanesES[MAX_CLIPPLANES];
    VECTOR4 vClipPlanesCS[MAX_CLIPPLANES];
    MATRIX44 mProjectionInvT;
  };

  GLenum SetupTNLStates(GLenum mode, int first, uint32_t count);

  void ProcessVertices(uint32_t count);

  GLenum UpdatePoints(uint8_t **ppbVertexData, int first, uint32_t count);

  GLenum UpdateColor(uint8_t **ppbVertexData, int first, uint32_t count);

  GLenum UpdateLighting(uint8_t **ppbVertexData, int first, uint32_t count);

  void UpdateNormal();

  GLenum UpdateTexcoords(uint8_t **ppbVertexData, int first, uint32_t count);

  void UpdateFog(uint8_t **ppbVertexData, int first, uint32_t count);

  void UpdateModelViewInvT44();

  void UpdateModelViewProj();

  void UpdateScreenXform();

  void UpdateClipPlanes();

  void UpdateProjectionInvT();

  void UpdateModelViewInvT33();

  void UpdateMaterial();

  void SetupLights();

  void UpdateLights();

  static uint32_t CalcClipFlags(const VECTOR4 &vPosition);

  uint32_t CalcUserClipFlags(uint32_t count);

  void XformScreenSpace(RDVECTOR *pRDVertex, const VECTOR4 *pvClipPos,
                        uint32_t count);

  void XformEyeSpace(uint32_t count);

  void ProcessPointSize(uint32_t count);

  void ProcessColor(uint32_t count);

  void ProcessLights_OneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal,
                              const VECTOR4 &vVertexColor);

  void ProcessLights_OneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal);

  void ProcessLights_TwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal,
                              const VECTOR4 &vVertexColor);

  void ProcessLights_TwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal);

  void ProcessTexCoords(uint32_t dstIndex, uint32_t srcIndex, uint32_t count);

  void UpdateMatrixDirtyFlags();

  std::vector<uint8_t> vertexBuffer_;

  CMatrixStack *pMsModelView_;
  CMatrixStack *pMsProjection_;
  CMatrixStack *pMsTexCoords_[MAX_TEXTURES];
  CMatrixStack *pMatrixStack_;
  GLenum matrixMode_;

  std::vector<VECTOR4> vClipPlanesES_;
  std::vector<Light> lights_;
  Material material_;

  MATRIX44 mProjectionInvT_;
  std::vector<VECTOR4> vClipPlanesCS_;

  MATRIX44 mModelViewInvT_;
  VECTOR4 vLightModelAmbient_;
  VECTOR4 vScaledAmbient_;
  VECTOR4 vMatEmissive_;
  VECTOR3 vNormNormal_;
  Light *pActiveLights_;
  floatf fMatShininess_;

  Fog fog_;
  PointParams pointParams_;

  MATRIX44 mModelViewProj_;
  ScreenXform screenXform_;

  PFN_DECODEPOSITION pfnDecodePosition_;
  PFN_VERTEXDECODE pfnPointSize_;
  PFN_VERTEXDECODE pfnColor_;
  PFN_DECODETEXCOORD pfnTexCoords_[MAX_TEXTURES];
  PFN_VERTEXDECODE pfnFog_;

  VertexDecoder positionDecode_;
  VertexDecoder normalDecode_;
  VertexDecoder colorDecode_;
  VertexDecoder texCoordDecodes_[MAX_TEXTURES];
  VertexDecoder pointSizeDecode_;

  uint8_t *pbVertexData_[VERTEXDATA_SIZE];
  uint8_t *pbVertexColor_;
  uint32_t clipVerticesBaseIndex_;

  TNLFLAGS TNLFlags_;

  RASTERID rasterID_;

  CullStates cullStates_;
};