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

#include "renderer.hpp"
#include "glfont.hpp"

class TextTest : public Renderer {
public:
  GLFont font_;

  TextTest(EGLNativeWindowType window)
      : Renderer(window) {}

  bool OnInitialize() {
    glViewport(0, 0, width_, height_);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    return font_.initialize("media/calibri.bff");
  }

  void OnRender() {
    glClear(GL_COLOR_BUFFER_BIT);
    font_.setColor(0.0f, 0.0f, 0.0f);
    font_.drawText("Lorem ipsum dolor sit amet, consectetur adipiscing elit,",
                   4.0f, 0.0f, 0.5f);
    font_.setColor(1.0f, 0.0f, 0.0f);
    font_.drawText(
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        4.0f, 16.0f, 0.5f);
    font_.setColor(0.0f, 1.0f, 0.0f);
    font_.drawText("quis nostrud exercitation ullamco laboris nisi ut aliquip "
                   "ex ea commodo consequat.",
                   4.0f, 32.0f, 0.5f);
    font_.setColor(1.0f, 1.0f, 0.0f);
    font_.drawText("Duis aute irure dolor in reprehenderit in voluptate velit "
                   "esse cillum dolore eu fugiat nulla pariatur.",
                   4.0f, 48.0f, 0.5f);
    font_.setColor(1.0f, 1.0f, 1.0f);
    font_.drawText("Excepteur sint occaecat cupidatat non proident, sunt in "
                   "culpa qui officia deserunt mollit anim id est laborum.",
                   4.0f, 64.0f, 0.5f);
  }
};
