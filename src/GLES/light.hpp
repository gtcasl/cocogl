//
// Copyright (c) Microsoft Corporation.  All rights reserved.
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


struct Light {
  enum {
    COLORS_FIRST = GL_AMBIENT,
    COLORS_LAST = GL_SPECULAR,
    COLORS_SIZE = (COLORS_LAST - COLORS_FIRST) + 1,
    ATTENUATIONS_FIRST = GL_CONSTANT_ATTENUATION,
    ATTENUATIONS_LAST = GL_QUADRATIC_ATTENUATION,
    ATTENUATIONS_SIZE = (ATTENUATIONS_LAST - ATTENUATIONS_FIRST) + 1,
  };

  void SetColor(GLenum pname, const VECTOR4 &color) {
    ASSERT((pname >= COLORS_FIRST) && (pname <= COLORS_LAST));
    m_vColors[pname - COLORS_FIRST] = color;
  }

  const VECTOR4 &GetColor(GLenum pname) const {
    ASSERT((pname >= COLORS_FIRST) && (pname <= COLORS_LAST));
    return m_vColors[pname - COLORS_FIRST];
  }

  void SetAttenuation(GLenum pname, floatf value) {
    ASSERT((pname >= ATTENUATIONS_FIRST) && (pname <= ATTENUATIONS_LAST));
    m_fAttenuations[pname - ATTENUATIONS_FIRST] = value;
  }

  floatf GetAttenuation(GLenum pname) const {
    ASSERT((pname >= ATTENUATIONS_FIRST) && (pname <= ATTENUATIONS_LAST));
    return m_fAttenuations[pname - ATTENUATIONS_FIRST];
  }

private:
  VECTOR4 m_vColors[COLORS_SIZE];

public:
  LIGHTFLAGS Flags;
  VECTOR4 vPosition;
  VECTOR3 vSpotDirection;
  floatf fSpotExponent;
  floatf fSpotCutOff;
  floatf fSpotCutOffCos;
  VECTOR3 vScaledAmbient;
  VECTOR3 vScaledDiffuse;
  VECTOR3 vHalfway;
  VECTOR3 vScaledSpecular;

private:
  floatf m_fAttenuations[ATTENUATIONS_SIZE];

public:
  Light *pNext;
};


struct Material {
public:
  enum {
    COLORS_FIRST = GL_AMBIENT,
    COLORS_LAST = GL_SPECULAR,
    COLORS_SIZE = (COLORS_LAST - COLORS_FIRST) + 1,
  };

  void SetColor(GLenum pname, const VECTOR4 &color) {
    ASSERT((pname >= COLORS_FIRST) && (pname <= COLORS_LAST));
    m_vColors[pname - COLORS_FIRST] = color;
  }

  const VECTOR4 &GetColor(GLenum pname) const {
    ASSERT((pname >= COLORS_FIRST) && (pname <= COLORS_LAST));
    return m_vColors[pname - COLORS_FIRST];
  }

private:
  VECTOR4 m_vColors[COLORS_SIZE];
};
