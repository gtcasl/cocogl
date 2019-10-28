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
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#pragma once


class CConfig : public CObject {
public:
  enum {
    ATTRIBUTES_FIRST = EGL_BUFFER_SIZE,
    ATTRIBUTES_LAST = EGL_RENDERABLE_TYPE,
    ATTRIBUTES_SIZE = (ATTRIBUTES_LAST - ATTRIBUTES_FIRST) + 1,
  };

  static EGLint Create(CConfig **ppConfig, EGLint red, EGLint green,
                       EGLint blue, EGLint alpha, EGLint depth, EGLint stencil);

  EGLint GetAttribute(EGLint name) const {
    ASSERT((name >= ATTRIBUTES_FIRST) && (name <= ATTRIBUTES_LAST));
    return m_attributes[name - ATTRIBUTES_FIRST];
  }

  void SetAttribute(GLint name, GLint value) {
    ASSERT((name >= ATTRIBUTES_FIRST) && (name <= ATTRIBUTES_LAST));
    m_attributes[name - ATTRIBUTES_FIRST] = value;
  }

  EGLint GetAtttribute(EGLint name, EGLint *pValue) const;

  EGLint Matches(const EGLint *pAttrib_list, bool *pbResult) const;

  static int Compare(const CConfig *pConfigA, const CConfig *pConfigB);

private:
  CConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha, EGLint depth,
          EGLint stencil);

  ~CConfig();

  EGLint m_attributes[ATTRIBUTES_SIZE];
};
