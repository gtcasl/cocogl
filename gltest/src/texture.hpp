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


class Texture {
public:
  Texture(LPCTSTR lpszFileName, GLenum minFilter = GL_LINEAR,
          GLenum magFilter = GL_LINEAR, GLenum wrapS = GL_REPEAT,
          GLenum wrapT = GL_REPEAT, int width = 128, int height = 128,
          int bpp = 24);
  ~Texture();
  inline void BindTexture() const { glBindTexture(GL_TEXTURE_2D, m_id); };

  // Set the texture active for the current context
  inline GLuint GetID() const { return m_id; };

  // returns the OpenGL ES texture handle
  inline bool GetState() const { return m_state; };

  // returns the texture's creation state
private:
  bool LoadTGA(LPCTSTR lpszFileName, GLubyte **pixels); // loads a tga texture
                                                        // (uncompressed, and
                                                        // with 24 or 32 bits)
  bool LoadRaw(LPCTSTR lpszFileName, GLubyte **pixels, int width, int height,
               int bpp);                              // loads a raw texture
  void FlipBitmapVertical(GLubyte *data, int format); // flips vertically a
                                                      // GLubyte array, useful
                                                      // for raw textures,
                                                      // because they turned
                                                      // upside down
  bool m_state;
  int m_width, m_height;
  GLuint m_id;
  GLenum m_minFilter, m_magFilter;
  GLenum m_wrapS, m_wrapT;
  GLenum m_format;
};
