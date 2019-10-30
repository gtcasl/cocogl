#include "stdafx.h"
#include <math.h>
#include <string.h>
#include "test.hpp"

void Ortho2D(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrthox(FixedFromInt(0), FixedFromInt(width), FixedFromInt(0),
           FixedFromInt(height), FixedFromInt(-1), FixedFromInt(1));
  glMatrixMode(GL_MODELVIEW);
}

void Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
  GLfixed xmin, xmax, ymin, ymax, aspectFixed, znearFixed;

  aspectFixed = FixedFromFloat(aspect);
  znearFixed = FixedFromFloat(zNear);

  ymax = MultiplyFixed(
      znearFixed, FixedFromFloat((GLfloat)tan(fovy * 3.1415962f / 360.0f)));
  ymin = -ymax;

  xmin = MultiplyFixed(ymin, aspectFixed);
  xmax = MultiplyFixed(ymax, aspectFixed);
  glFrustumx(xmin, xmax, ymin, ymax, znearFixed, FixedFromFloat(zFar));
}

void normalizef(float v[3]) {
  float r;
  r = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
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

void __identx(GLfixed m[]) {
  memset(m, 0, sizeof m[0] * 16);
  m[0] = m[5] = m[10] = m[15] = __f2x(1.0f);
}

void LookAtf(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
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

bool LoadTGA(LPCTSTR lpszFileName, GLuint *id) {

  FILE *f = _tfopen(lpszFileName, _T("rb"));
  if (nullptr == f) {
    return false;
  }

  GLubyte *pixels = nullptr;

  uint16_t width = 0, height = 0;
  uint8_t headerLength = 0;
  uint8_t imageType = 0;
  uint8_t bits = 0;
  int format = 0;
  int lineWidth = 0;

  fread(&headerLength, sizeof(uint8_t), 1, f);

  // skip next uint8_t
  fseek(f, 1, SEEK_CUR);

  // read in the imageType (RLE, RGB, etc...)
  fread(&imageType, sizeof(uint8_t), 1, f);

  // skip information we don't care about
  fseek(f, 9, SEEK_CUR);

  // read the width, height and bits per pixel (16, 24 or 32). We only will take
  // care of 24 bits uncompressed TGAs
  fread(&width, sizeof(uint16_t), 1, f);
  fread(&height, sizeof(uint16_t), 1, f);
  fread(&bits, sizeof(uint8_t), 1, f);

  // move the file pointer to the pixel data
  fseek(f, headerLength + 1, SEEK_CUR);

  // check if the image is not compressed.
  if (imageType != 10) {
    switch (bits) {
    case 24:
    case 32: {
      format = bits >> 3; // Another way to divide between 8. We want to know
                          // the pixel size in uint8_ts.
      lineWidth = format * width;
      pixels = new GLubyte[lineWidth * height];

      // we are going to load the pixel data line by line
      for (int y = 0; y < height; ++y) {
        // Read current line of pixels
        GLubyte *line = &pixels[lineWidth * y];
        fread(line, lineWidth, 1, f);

        //Because the TGA is BGR instead of RGB, we must swap the R and G
      //components (OGL ES does not have the
      //GL_BGR_EXT extension
        for (int i = 0; i < lineWidth; i += format) {
          GLubyte temp = line[i];
          line[i] = line[i + 2];
          line[i + 2] = temp;
        }
      }

      break;
    }

    default:
      fclose(f);
      *id = 0;
      return false;
    }
  }

  fclose(f);

  glGenTextures(1, id);
  glBindTexture(GL_TEXTURE_2D, *id);
  glTexParameterx(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

  switch (bits) {
  case 24:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pixels);
    break;

  case 32:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
    break;
  }

  delete[] pixels;

  return (GL_NO_ERROR == glGetError());
}