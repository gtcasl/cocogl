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

class GLFont {
public:
  GLFont();

  ~GLFont();

  bool initialize(const char *filename);

  void setColor(float r, float g, float b);

  void drawText(const char *text, float x, float y, float scale);

  void destroy();

private:
  enum { NUMCHARS = 256, SPACE = 32 };

  struct glyph_t {
    int l;
    int t;
    int w;
    int h;
  };

  struct vertex_t {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
  };

  std::vector<vertex_t> vertices_;
  std::array<uint8_t, 256> widths_;

  float col_factor_;
  float row_factor_;
  float inv_width_;
  int cell_height_;

  float cr_;
  float cg_;
  float cb_;

  GLuint texture_;
};