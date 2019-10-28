//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#include "stdafx.h"
#include "texture.hpp"

Texture::Texture(LPCTSTR lpszFileName, GLenum minFilter, GLenum magFilter,
                 GLenum wrapS, GLenum wrapT, int width, int height, int bpp) {
  GLubyte *pixels = NULL;

  // Check the filename's length for security
  int len = _tcslen(lpszFileName);
  if (len < 4) {
    m_state = false;
    return;
  }

  // choose the texture format based on the filename extension
  // these two functions returns the texture in the pixels' array
  if (!_tcscmp(L".tga", &lpszFileName[len - 4]))
    m_state = LoadTGA(lpszFileName, &pixels);
  else if (!_tcscmp(L".raw", &lpszFileName[len - 4]))
    m_state = LoadRaw(lpszFileName, &pixels, width, height, bpp);
  else {
    m_state = false;
    return;
  }

  if (!m_state) {
    if (pixels)
      delete[] pixels;
    m_state = false;
    return;
  }

  // setup format (it shall be , 2, 3 or 4)
  switch (m_format) {
  case 1:
    m_format = GL_LUMINANCE;
    break;
  case 2:
    m_format = GL_LUMINANCE_ALPHA;
    break;
  case 3:
    m_format = GL_RGB;
    break;
  case 4:
    m_format = GL_RGBA;
    break;
  };

  m_minFilter = minFilter;
  m_magFilter = magFilter;
  m_wrapS = wrapS;
  m_wrapT = wrapT;

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
  glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
  glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
  glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);

  if ((m_minFilter != GL_LINEAR) && (m_minFilter != GL_NEAREST))
    glTexParameterx(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,
                    GL_TRUE); // autogenerate mipmaps
  int a = glGetError();
  glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format,
               GL_UNSIGNED_BYTE, pixels);
  a = glGetError();
  delete[] pixels;
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

bool Texture::LoadRaw(LPCTSTR lpszFileName, GLubyte **pixels, int width,
                      int height, int bpp) {
  FILE *f = NULL;
  int format = 0;

  TCHAR szFullPath[MAX_PATH];
  if (!::GetModuleFileName(NULL, szFullPath, MAX_PATH)) {
    return false;
  }

  PathRemoveFileSpec(szFullPath);
  PathAppend(szFullPath, MAX_PATH, lpszFileName);

  f = _tfopen(szFullPath, _T("rb"));
  if (NULL == f) {
    return false;
  }

  format = bpp >> 3; // format = bpp / 8
  m_height = height;
  m_width = width;

  *pixels = new GLubyte[width * height * format];
  fread(*pixels, width * height * format, 1, f);
  fclose(f);

  FlipBitmapVertical(*pixels, format); // flip
  m_format = format;                   // bytes per pixel
  return true;
}

bool Texture::LoadTGA(LPCTSTR lpszFileName, GLubyte **pixels) {
  TCHAR szFullPath[MAX_PATH];
  if (!::GetModuleFileName(NULL, szFullPath, MAX_PATH)) {
    return false;
  }

  PathRemoveFileSpec(szFullPath);
  PathAppend(szFullPath, MAX_PATH, lpszFileName);

  FILE *f = _tfopen(szFullPath, _T("rb"));
  if (NULL == f) {
    return false;
  }

  uint16_t width, height;
  byte headerLength = 0;
  byte imageType = 0;
  byte bits = 0;
  int format = 0;
  int lineWidth = 0;

  fread(&headerLength, sizeof(byte), 1, f);
  fseek(f, 1, SEEK_CUR);
  fread(&imageType, sizeof(byte), 1, f);
  fseek(f, 9, SEEK_CUR);
  fread(&width, sizeof(uint16_t), 1, f);
  fread(&height, sizeof(uint16_t), 1, f);
  fread(&bits, sizeof(byte), 1, f);

  m_width = width;
  m_height = height;

  fseek(f, headerLength + 1, SEEK_CUR);

  GLubyte *buffer = NULL;
  if (10 == imageType) {
    fclose(f);
    return false;
  } else {
    if ((bits == 24) ||
        (bits == 32)) // added to support for LUMINANCE and RGBA textures
    {
      format = bits >> 3;
      lineWidth = format * m_width;
      buffer = new GLubyte[lineWidth * m_height];

      for (int y = 0; y < m_height; y++) {
        GLubyte *line = &buffer[lineWidth * y];
        fread(line, lineWidth, 1, f);

        if (format != 1) {
          for (int i = 0; i < lineWidth;
               i += format) // swap R and B because TGA are stored in BGR format
          {
            GLubyte temp = line[i];
            line[i] = line[i + 2];
            line[i + 2] = temp;
          }
        }
      }
    } else {
      fclose(f);
      *pixels = buffer;
      return false;
    }
  }

  fclose(f);
  *pixels = buffer;
  m_format = format; // bytes per pixel
  return true;
}

void Texture::FlipBitmapVertical(GLubyte *data, int format) {
  int i, lineWidth = m_width * format;
  GLubyte *row = NULL, *top = NULL, *bottom = NULL;
  row = new GLubyte[m_height * m_width * format];

  top = data;
  bottom = data + lineWidth * (m_height - 1);

  for (i = 0; i<m_height>> 1; i++) {
    memcpy(row, top, lineWidth);
    memcpy(top, bottom, lineWidth);
    memcpy(bottom, row, lineWidth);
    top = top + lineWidth;
    bottom = bottom - lineWidth;
  }

  if (row)
    delete[] row;
}
