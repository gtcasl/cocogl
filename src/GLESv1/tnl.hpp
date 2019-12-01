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

typedef void (*PfnDecodePosition)(VECTOR4 *pOut, const uint8_t *pbIn,
                                   uint32_t stride, uint32_t count);

class CTNL : public CInputAssembler {
public:
  typedef void (CTNL::*PfnDecodeVertices)(uint32_t count);
  typedef void (CTNL::*PfnDecodeTexCoord)(uint32_t dstIndex, uint32_t srcIndex, uint32_t count);

  template <bool QuadraticAttenuation, eVertexFormat VertexFormat>
  void processPointSize(uint32_t count);

  template <bool ColorMaterial, eVertexFormat ColorFormat,
            eVertexFormat NormalFormat>
  void processLightingOneSided(uint32_t count);

  template <bool ColorMaterial, eVertexFormat ColorFormat,
            eVertexFormat NormalFormat>
  void processLightingTwoSided(uint32_t count);

  template <eVertexFormat VertexFormat>
  void processVertexColor(uint32_t count);

  template <bool Transform, eVertexFormat VertexFormat>
  void processTexCoords(uint32_t dstIndex, uint32_t srcIndex, uint32_t count);

  template <eFogMode FogMode> void processFog(uint32_t count);

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

  GLenum setupTNLStates(GLenum mode, int first, uint32_t count);

  void processVertices(uint32_t count);

  GLenum updatePoints(uint8_t **ppbVertexData, int first, uint32_t count);

  GLenum updateColor(uint8_t **ppbVertexData, int first, uint32_t count);

  GLenum updateLighting(uint8_t **ppbVertexData, int first, uint32_t count);

  void updateNormal();

  GLenum updateTexcoords(uint8_t **ppbVertexData, int first, uint32_t count);

  void updateFog(uint8_t **ppbVertexData, int first, uint32_t count);

  void updateModelViewInvT44();

  void updateModelViewProj();

  void updateScreenXform();

  void updateClipPlanes();

  void updateProjectionInvT();

  void updateModelViewInvT33();

  void updateMaterial();

  void setupLights();

  void updateLights();

  static uint32_t CalcClipFlags(const VECTOR4 &vPosition);

  uint32_t calcUserClipFlags(uint32_t count);

  void transformScreenSpace(RDVECTOR *pRDVertex, const VECTOR4 *pvClipPos,
                        uint32_t count);

  void transformEyeSpace(uint32_t count);

  void processPointSize(uint32_t count);

  void processColor(uint32_t count);

  void processLightsOneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal,
                              const VECTOR4 &vVertexColor);

  void processLightsOneSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal);

  void processLightsTwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal,
                              const VECTOR4 &vVertexColor);

  void processLightsTwoSided(VECTOR4 *pvOut, const VECTOR3 &vEyePos,
                              const VECTOR3 &vNormal);

  void processTexCoords(uint32_t dstIndex, uint32_t srcIndex, uint32_t count);

  void updateMatrixDirtyFlags();

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

  PfnDecodePosition pfnDecodePosition_;
  PfnDecodeVertices pfnPointSize_;
  PfnDecodeVertices pfnColor_;
  PfnDecodeTexCoord pfnTexCoords_[MAX_TEXTURES];
  PfnDecodeVertices pfnFog_;

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