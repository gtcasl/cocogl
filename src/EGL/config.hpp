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

class _EGLConfig : public Object {
public:

  enum {
    ATTRIBUTES_FIRST = EGL_BUFFER_SIZE,
    ATTRIBUTES_LAST = EGL_RENDERABLE_TYPE,
    ATTRIBUTES_SIZE = (ATTRIBUTES_LAST - ATTRIBUTES_FIRST) + 1,
  };

  static EGLint Create(_EGLConfig **ppConfig, 
                       EGLint red, 
                       EGLint green,
                       EGLint blue, 
                       EGLint alpha, 
                       EGLint depth, 
                       EGLint stencil);

  EGLint getAttribute(EGLint name) const {
    assert((name >= ATTRIBUTES_FIRST) && (name <= ATTRIBUTES_LAST));
    return attributes_[name - ATTRIBUTES_FIRST];
  }

  void setAttribute(EGLint name, EGLint value) {
    assert((name >= ATTRIBUTES_FIRST) && (name <= ATTRIBUTES_LAST));
    attributes_[name - ATTRIBUTES_FIRST] = value;
  }

  EGLint getAtttribute(EGLint name, EGLint *pValue) const;

  EGLint matches(const EGLint *pAttrib_list, bool *pbResult) const;

  static int compare(const _EGLConfig *pConfigA, const _EGLConfig *pConfigB);

private:

  _EGLConfig(EGLint red, 
             EGLint green, 
             EGLint blue, 
             EGLint alpha, 
             EGLint depth,
             EGLint stencil);
             
  ~_EGLConfig();

  EGLint attributes_[ATTRIBUTES_SIZE];
};
