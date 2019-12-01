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

#include "raster.hpp"

class GLContext : public CRasterizer {
public:
  //--
  static GLenum Create(GLContext **ppContext, HandleTable *pHandles,
                       CRasterCache *pRasterCache, GLContext *pCtxShared);

  //--
  void setDrawSurface(GLSurface *pSurface);
  void setReadSurface(GLSurface *pSurface);

  //--
  void setError(GLenum error);
  GLenum getError() const;

  //--
  void clearColor(floatf red, floatf green, floatf blue, floatf alpha);
  void clearDepth(floatf depth);
  void clearStencil(GLint stencil);

  void setColorMask(GLboolean red, GLboolean green, GLboolean blue,
                 GLboolean alpha);

  void setDepthMask(GLboolean flag);
  void setStencilMask(GLuint mask);

  void setClientState(GLenum array, bool bValue);
  void setClientActiveTexture(GLenum texture);
  void setActiveTexture(GLenum texture);

  void setVertexPointer(GLint size, GLenum type, GLsizei stride,
                     const GLvoid *pPointer);

  void setNormalPointer(GLenum type, GLsizei stride, const GLvoid *pPointer);

  void setColorPointer(GLint size, GLenum type, GLsizei stride,
                    const GLvoid *pPointer);

  void setTexCoordPointer(GLint size, GLenum type, GLsizei stride,
                       const GLvoid *pPointer);

  void setPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pPointer);

  void setColor(floatf red, floatf green, floatf blue, floatf alpha);
  void setNormal(floatf nx, floatf ny, floatf nz);
  void setMultiTexCoord(GLenum target, floatf s, floatf t, floatf r, floatf q);

  void genBuffers(GLsizei n, GLuint *phBuffers);
  void bindBuffer(GLenum target, GLuint buffer);

  void setBufferData(GLenum target, GLsizeiptr size, const GLvoid *pData,
                  GLenum usage);

  void setBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                     const GLvoid *pData);

  void getBufferParameter(GLenum target, GLenum pname, GLint *pParams);
  void deleteBuffers(GLsizei n, const GLuint *phBuffers);

  void genTextures(GLsizei n, GLuint *phTextures);
  void bindTexture(GLenum target, GLuint texture);

  void setTexImage2D(GLenum target, GLint level, GLint internalformat,
                  GLsizei width, GLsizei height, GLint border, GLenum format,
                  GLenum type, const GLvoid *pPixels);

  void setTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                     GLsizei width, GLsizei height, GLenum format, GLenum type,
                     const GLvoid *pPixels);

  void copyTexImage2D(GLenum target, GLint level, GLenum internalformat,
                      GLint x, GLint y, GLsizei width, GLsizei height,
                      GLint border);

  void copyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                         GLint yoffset, GLint x, GLint y, GLsizei width,
                         GLsizei height);

  void compressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
                            GLsizei width, GLsizei height, GLint border,
                            GLsizei imageSize, const GLvoid *pData);

  void compressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
                               GLint yoffset, GLsizei width, GLsizei height,
                               GLenum format, GLsizei imageSize,
                               const GLvoid *pData);

  void deleteTextures(GLsizei n, const GLuint *phTextures);
  void setPixelStorei(GLenum pname, GLint param);

  void readPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                  GLenum format, GLenum type, GLvoid *pPixels);

  GLenum bindTexImage(GLSurface *pSurface, bool bGenMipMaps);
  GLenum releaseTexImage(GLSurface *pSurface);

  void setMatrixMode(GLenum mode);
  void pushMatrix();
  void popMatrix();
  void loadIdentity();
  void loadMatrix(const MATRIX44 &matrix);

  void scale(floatf x, floatf y, floatf z);
  void translate(floatf x, floatf y, floatf z);
  void rotate(floatf angle, floatf x, floatf y, floatf z);
  void multiply(const MATRIX44 &matrix);
  void ortho(floatf left, floatf right, floatf bottom, floatf top, floatf zNear,
             floatf zFar);
  void frustum(floatf left, floatf right, floatf bottom, floatf top,
               floatf zNear, floatf zFar);

  template <class T> GLbitfield queryMatrix(T *pMantissa, GLint exponent[16]);

  void setDepthRange(floatf zNear, floatf zFar);
  void setClipPlane(GLenum plane, const VECTOR4 &equation);
  void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);

  template <class T> void setLightParameter(GLenum light, GLenum pname, const T *pParams);

  template <class T> void setLightParameterModel(GLenum pname, const T *pParams);

  template <class T>
  void setMaterial(GLenum face, GLenum pname, const T *pParams);

  template <class T> void setFog(GLenum pname, const T *pParams);

  template <class T>
  void setTexParameter(GLenum target, GLenum pname, const T *pParams);

  template <class T>
  void setTexEnv(GLenum target, GLenum pname, const T *pParams);

  void setAlphaFunc(GLenum func, floatf ref);
  void setStencilFunc(GLenum func, GLint ref, GLuint mask);
  void setStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
  void setDepthFunc(GLenum func);
  void setBlendFunc(GLenum sfactor, GLenum dfactor);
  void setLogicOp(GLenum opcode);

  void setShadeModel(GLenum mode);

  void setFrontFace(GLenum mode);

  void setCullFace(GLenum mode);

  void setScissor(GLint x, GLint y, GLsizei width, GLsizei height);

  void activate(GLenum cap, bool bValue);
  void setActivateLight(GLenum cap, bool bValue);

  void setSampleCoverage(floatf value, GLboolean invert);

  void hint(GLenum target, GLenum mode);

  void setPolygonOffset(floatf factor, floatf units);

  void setPointSize(floatf size);
  void setLineWidth(floatf width);

  //--
  const GLubyte *getString(GLenum name);

  //--
  template <class T> void getClipPlane(GLenum plane, T eqn[4]);

  template <class T> void get(GLenum pname, T *pParams);

  template <class T> void getLight(GLenum light, GLenum pname, T *pParams);

  template <class T> void getMaterial(GLenum face, GLenum pname, T *pParams);

  template <class T> void getTexEnv(GLenum env, GLenum pname, T *pParams);

  template <class T>
  void getTexParameter(GLenum target, GLenum pname, T *pParams);

  void getPointer(void **ppParams, GLenum pname);
  bool isEnabled(GLenum cap);
  bool isBuffer(GLuint buffer);
  bool isTexture(GLuint texture);

  template <class T> void setPointParameter(GLenum pname, const T *pParams);

  void clear(GLbitfield mask);

  void drawArrays(GLenum mode, GLint first, GLsizei count);

  void drawElements(GLenum mode, GLsizei count, GLenum type,
                    const GLvoid *pIndices);

  void flush();
  void finish();

private:
  GLContext(HandleTable *pHandles, CRasterCache *pRasterCache,
             GLContext *pSharedCtx);

  ~GLContext();

  GLenum initialize();
};
