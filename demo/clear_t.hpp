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

#include "test.hpp"

class ClearTest : public TestBase {
public:
  bool OnInitialize(uint32_t /*width*/, uint32_t /*height*/) {
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    return true;
  }

  void OnRender() { glClear(GL_COLOR_BUFFER_BIT); }
};
