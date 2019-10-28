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
  GLenum PrepareIndices(GLenum type, const GLvoid **ppvIndices, unsigned count,
                        unsigned *pMin, unsigned *pMax);

protected:
  CInputAssembler() {}
  ~CInputAssembler() {}

  VertexArray m_positionArray;
  VertexArray m_normalArray;
  VertexArray m_colorArray;
  VertexArray m_texCoordArrays[MAX_TEXTURES];
  VertexArray m_pointSizeArray;

  VECTOR4 m_vColor;
  VECTOR3 m_vNormal;
  VECTOR2 m_vTexCoords[MAX_TEXTURES];
  floatf m_fPointSize;
};