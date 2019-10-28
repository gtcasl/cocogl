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


template <class T> GLenum TToGLenum(T param);


template <class T> class TAddressOf {
public:
  TAddressOf(const T &value) : m_value(value) {}

  operator const T *() const { return &m_value; }

  operator T *() { return &m_value; }

private:
  T m_value;
};

inline bool __glFailed(unsigned int err) { return (err != GL_NO_ERROR); }

inline bool __glSucceeded(unsigned int err) { return (err == GL_NO_ERROR); }

#ifndef NDEBUG
#define __glLog(...) CLogger::Instance().Write(__VA_ARGS__);

#define __glLogError(...)                                                \
  CLogger::Instance().Write(_T("*** Error in file %s at line %d.\r\n"),  \
                            _T(__FILE__), __LINE__);                     \
  CLogger::Instance().Write(__VA_ARGS__);                                \
  ASSERT(false);
#else
#define __glLog(...)
#define __glLogError(...)
#endif
#define __glError(error, ...)                                            \
  __glLogError(__VA_ARGS__);                                             \
  this->SetError(error);

void memset16(void *dst, int fill, int cwCount);
void memset32(void *dst, int fill, int cwCount);

unsigned GLSizeOf(GLenum type);

eVertexFormat ToVertexFormat(GLenum type, GLint size);

unsigned VertexDataSize(eVertexFormat format);

GLenum DecodeDataType(eVertexFormat format);

GLint DecodeDataSize(eVertexFormat format);

GLenum GLERROR_FROM_HRESULT(HRESULT hr);

GLenum ToPixelFormat(ePixelFormat *pOut, unsigned *pBPP, GLenum format,
                     GLenum type);

GLenum ToPixelFormat(ePixelFormat *pOut, GLint format);

eCompare Reverse(eCompare compare);

unsigned CompareFuncFromEnum(GLenum func);
GLenum EnumFromCompareFunc(unsigned func);

unsigned StencilOpFromEnum(GLenum op);
GLenum EnumFromStencilOp(unsigned op);

unsigned BlendFuncFromEnum(GLenum func);
GLenum EnumFromBlendFunc(unsigned func);

unsigned LogicOpFromEnum(GLenum op);
GLenum EnumFromLogicOp(unsigned op);

unsigned FogModeFromEnum(GLenum mode);
GLenum EnumFromFogMode(unsigned mode);

unsigned ShadeModelFromEnum(GLenum mode);
GLenum EnumFromShadeModel(unsigned mode);

unsigned CullFaceFromEnum(GLenum mode);
GLenum EnumFromCullFace(unsigned mode);

unsigned FrontFaceFromEnum(GLenum mode);
GLenum EnumFromFrontFace(unsigned mode);

unsigned HintFromEnum(GLenum mode);
GLenum EnumFromHint(unsigned mode);

unsigned TexFilterFromEnum(GLenum param);
GLenum EnumFromTexFilter(unsigned param);

unsigned TexAddressFromEnum(GLenum param);
GLenum EnumFromTexAddress(unsigned param);

eEnvMode TexEnvFromEnum(GLenum param);
GLenum EnumFromTexEnv(unsigned param);

GLenum CopyBuffers(const GLSurfaceDesc &dstDesc, unsigned dstOffsetX,
                   unsigned dstOffsetY, unsigned copyWidth, unsigned copyHeight,
                   const GLSurfaceDesc &srcDesc, unsigned srcOffsetX,
                   unsigned srcOffsetY);


template <> inline GLenum TToGLenum<float>(float param) {
  return static_cast<GLenum>(param);
}


template <> inline GLenum TToGLenum<fixed16>(fixed16 param) {
  return *reinterpret_cast<const GLenum *>(&param);
}


template <> inline GLenum TToGLenum<int>(int param) {
  return static_cast<GLenum>(param);
}


template <class T, unsigned BITS> class TBitPtr {
public:
  enum {
    MASK = (1 << BITS) - 1,
  };

  TBitPtr(const void *ptr) {
    m_ptr = reinterpret_cast<const T *>(ptr);
    m_shift = (sizeof(T) << 3) - BITS;
  }

  T operator*() const { return (*m_ptr >> m_shift) & MASK; }

  const TBitPtr &operator++() {
    if (m_shift >= BITS) {
      m_shift -= BITS;
    } else {
      m_shift = (sizeof(T) << 3) - BITS;
      ++m_ptr;
    }

    return *this;
  }

private:
  const T *m_ptr;
  unsigned m_shift;
};
