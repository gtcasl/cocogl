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
#include "device.hpp"

class CInputAssembler : public CDevice {
public:
  GLenum PrepareIndices(GLenum type, const GLvoid **ppvIndices, uint32_t count,
                        uint32_t *pMin, uint32_t *pMax);

protected:
  CInputAssembler() {}
  ~CInputAssembler() {}

  VertexArray positionArray_;
  VertexArray normalArray_;
  VertexArray colorArray_;
  VertexArray texCoordArrays_[MAX_TEXTURES];
  VertexArray pointSizeArray_;

  VECTOR4 vColor_;
  VECTOR3 vNormal_;
  VECTOR2 vTexCoords_[MAX_TEXTURES];
  floatf fPointSize_;
};