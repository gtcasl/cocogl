

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
#include "stdafx.h"
#include <fstream>
#include "glfont.hpp"

struct __attribute__((__packed__)) bff_header_t {
  uint16_t fileid;
  uint32_t width;
  uint32_t height;
  uint32_t cellX;
  uint32_t cellY;
  uint8_t bpp;
  uint8_t base;
};

GLFont::GLFont() {
  cr_ = 1.0f;
  cg_ = 1.0f;
  cb_ = 1.0f;
}

GLFont::~GLFont() {
  //--
}

bool GLFont::initialize(const char *filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "couldn't open TGA file: " << filename << "!" << std::endl;
    return false;
  }

  bff_header_t header;
  ifs.read(reinterpret_cast<char *>(&header), sizeof(bff_header_t));
  if (ifs.fail()) {
    std::cerr << "invalid BFF file!" << std::endl;
    return false;
  }

  if (header.fileid != 0xF2BF) {
    std::cerr << "invalid BFF file!" << std::endl;
    return false;
  }

  if (header.width / header.cellX != 16 || header.height / header.cellY != 16) {
    std::cerr << "only 16x16 cells BFF supported!" << std::endl;
    return false;
  }

  if (header.bpp != 8) {
    std::cerr << "only grayscale BFF supported!" << std::endl;
    return false;
  }

  if (header.base != 0) {
    std::cerr << "BFF base not support!" << std::endl;
    return false;
  }

  ifs.read(reinterpret_cast<char *>(widths_.data()), widths_.size());
  if (ifs.fail()) {
    std::cerr << "invalid BFF file!" << std::endl;
    return false;
  }

  std::vector<uint8_t> pixels(header.width * header.height);
  ifs.read(reinterpret_cast<char *>(pixels.data()), pixels.size());
  if (ifs.fail()) {
    std::cerr << "invalid BFF file!" << std::endl;
    return false;
  }

  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameterx(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, header.width, header.height, 0,
               GL_ALPHA, GL_UNSIGNED_BYTE, pixels.data());

  col_factor_ = float(header.cellX) / header.width;
  row_factor_ = float(header.cellY) / header.height;
  inv_width_ = 1.0f / header.width;
  cell_height_ = header.cellY;

  return true;
}

void GLFont::drawText(const char *text, float x, float y, float scale) {
  int n = 0;

  auto x0 = x;
  auto y0 = y;
  auto y1 = y0 + cell_height_ * scale;

  vertices_.resize(6 * strlen(text));

  for (const char *p = text; *p; ++p) {
    auto c = *p;
    auto w = widths_[c];
    auto col = c & 0xf;
    auto row = c >> 4;

    auto x1 = x0 + w * scale;

    auto u0 = col * col_factor_;
    auto v0 = row * row_factor_;
    auto u1 = u0 + w * inv_width_;
    auto v1 = v0 + row_factor_;

    vertices_[n++] = {x0, y0, u0, v0};
    vertices_[n++] = {x1, y0, u1, v0};
    vertices_[n++] = {x0, y1, u0, v1};
    vertices_[n++] = {x1, y0, u1, v0};
    vertices_[n++] = {x0, y1, u0, v1};
    vertices_[n++] = {x1, y1, u1, v1};

    x0 = x1;
  }

  GLint viewport[4];
  GLint matrixMode;

  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glVertexPointer(2, GL_FLOAT, 16, vertices_.data());
  glTexCoordPointer(2, GL_FLOAT, 16,
                    reinterpret_cast<uint8_t *>(vertices_.data()) + 8);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrthof(0.0f, viewport[2], viewport[3], 0.0f, 0.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(cr_, cg_, cb_, 1.0f);

  glDrawArrays(GL_TRIANGLES, 0, n);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(matrixMode);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GLFont::setColor(float r, float g, float b) {
  cr_ = r;
  cg_ = g;
  cb_ = b;
}

void GLFont::destroy() { glDeleteTextures(1, &texture_); }