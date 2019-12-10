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
#include <math.h>
#include <fstream>

struct __attribute__((__packed__)) tga_header_t {
  int8_t idlength;
  int8_t colourmaptype;
  int8_t datatypecode;
  int16_t colourmaporigin;
  int16_t colourmaplength;
  int8_t colourmapdepth;
  int16_t x_origin;
  int16_t y_origin;
  int16_t width;
  int16_t height;
  int8_t bitsperpixel;
  int8_t imagedescriptor;
};

void Ortho2D(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrthof(0, width, 0, height, -1, 1);
  glMatrixMode(GL_MODELVIEW);
}

void Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
  auto ymax = zNear * tan(fovy * 3.1415962f / 360.0f);
  auto ymin = -ymax;
  auto xmin = ymin * aspect;
  auto xmax = ymax * aspect;
  glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

void normalizef(float v[3]) {
  auto r = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (r == 0.0)
    return;
  r = 1.f / r;
  v[0] *= r;
  v[1] *= r;
  v[2] *= r;
}

void crossf(float v0[3], float v1[3], float r[3]) {
  r[0] = v0[1] * v1[2] - v0[2] * v1[1];
  r[1] = v0[2] * v1[0] - v0[0] * v1[2];
  r[2] = v0[0] * v1[1] - v0[1] * v1[0];
}

void __identf(GLfloat m[]) {
  memset(m, 0, sizeof m[0] * 16);
  m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void LookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
            GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
            GLfloat upz) {
  GLfloat forward[3], side[3], up[3];
  GLfloat m[4][4];

  forward[0] = centerx - eyex;
  forward[1] = centery - eyey;
  forward[2] = centerz - eyez;

  normalizef(forward);

  up[0] = upx;
  up[1] = upy;
  up[2] = upz;

  crossf(forward, up, side);

  normalizef(side);
  crossf(side, forward, up);

  __identf(&m[0][0]);
  m[0][0] = side[0];
  m[1][0] = side[1];
  m[2][0] = side[2];

  m[0][1] = up[0];
  m[1][1] = up[1];
  m[2][1] = up[2];

  m[0][2] = -forward[0];
  m[1][2] = -forward[1];
  m[2][2] = -forward[2];

  glMultMatrixf(&m[0][0]);
  glTranslatef(-eyex, -eyey, -eyez);
}

GLuint loadTexture(const std::vector<uint8_t> &pixels, int width, int height,
                   int bpp) {
  assert(bpp == 32 || bpp == 24);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterx(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  auto format = (bpp == 32) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, pixels.data());
  assert(GL_NO_ERROR == glGetError());
  return texture;
}

bool LoadTGA(const char *filename, std::vector<uint8_t> &pixels, int *width,
             int *height, int *bpp) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "couldn't open TGA file: " << filename << "!" << std::endl;
    return false;
  }

  tga_header_t header;
  ifs.read(reinterpret_cast<char *>(&header), sizeof(tga_header_t));
  if (ifs.fail()) {
    std::cerr << "invalid TGA file header!" << std::endl;
    return false;
  }

  if (header.datatypecode != 2) {
    std::cerr << "unsupported TGA encoding format!" << std::endl;
    return false;
  }

  ifs.seekg(header.idlength, std::ios::cur); // skip string
  if (ifs.fail()) {
    std::cerr << "invalid TGA file!" << std::endl;
    return false;
  }

  switch (header.bitsperpixel) {
  case 24:
  case 32: {
    auto stride = header.bitsperpixel / 8;
    auto pitch = header.width * stride;
    pixels.resize(header.height * pitch);

    // we are going to load the pixel data line by line
    for (int y = 0; y < header.height; ++y) {
      // Read current line of pixels
      auto line = pixels.data() + y * pitch;
      ifs.read(reinterpret_cast<char *>(line), pitch);
      if (ifs.fail()) {
        std::cerr << "invalid TGA file!" << std::endl;
        return false;
      }

      // Because the TGA is BGR instead of RGB, we must swap RG components
      for (int i = 0; i < pitch; i += stride) {
        auto tmp = line[i];
        line[i] = line[i + 2];
        line[i + 2] = tmp;
      }
    }
    break;
  }
  default:
    std::cerr << "unsupported TGA bitsperpixel!" << std::endl;
    return false;
  }

  *width = header.width;
  *height = header.height;
  *bpp = header.bitsperpixel;

  return true;
}