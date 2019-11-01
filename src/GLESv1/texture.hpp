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

class CGLSurface;
class CSurface2D;

class SurfaceDesc {
public:
  const uint8_t *GetBits() const { return m_pbBits; }

  uint8_t *GetBits() { return m_pbBits; }

  uint8_t GetLogWidth() const { return m_logWidth; }

  uint8_t GetLogHeight() const { return m_logHeight; }

  uint16_t GetWidth() const { return 1 << m_logWidth; }

  uint16_t GetHeight() const { return 1 << m_logHeight; }

protected:
  uint8_t *m_pbBits;

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint16_t m_logWidth_logHeight;
    struct {
      uint8_t m_logWidth;
      uint8_t m_logHeight;
    };
  };
  DISABLE_WARNING_POP

  friend class CGSurface;
};

struct Sampler {
  const CSurface2D *pMipLevels;

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      uint32_t cEnvColor_MaxMipLevel;
    };
    struct {
      uint8_t cEnvColor[3];
      uint8_t MaxMipLevel; // cEnvColor alpha channel
    };
  };
  DISABLE_WARNING_POP
};

class CSurface2D : public SurfaceDesc {
public:
  CSurface2D() { this->Clear(); }

  ~CSurface2D() { this->Destroy(); }

  GLenum Initialize(uint32_t width, uint32_t height, ePixelFormat format);

  GLenum Initialize(uint32_t width, uint32_t height, int32_t pitch,
                    ePixelFormat format, GLvoid *pPixels);

  GLenum Initialize(uint32_t width, uint32_t height, ePixelFormat format,
                    const GLvoid *pPixels);

  uint32_t GetPitch() const { return m_pitch; }

  ePixelFormat GetFormat() const { return static_cast<ePixelFormat>(m_format); }

  void GetDesc(GLSurfaceDesc *pSurfaceDesc) const {
    ASSERT(pSurfaceDesc);
    pSurfaceDesc->pBits = m_pbBits;
    pSurfaceDesc->Width = this->GetWidth();
    pSurfaceDesc->Height = this->GetHeight();
    pSurfaceDesc->Pitch = m_pitch;
    pSurfaceDesc->Format = m_format;
  }

  void Destroy();

private:
  void Clear();

  int32_t m_pitch;
  uint8_t m_format;
  bool m_bOwnedBuffer;
};

class CTexture : public CObject {
public:
  TexParams Params;
  bool bGenMipMaps;

  static GLenum Create(CTexture **ppTexture);

  const CSurface2D &GetSurface(uint32_t level) const {
    ASSERT(level < MAX_TEXTURE_LEVELS);
    return m_surfaces[level];
  }

  const CSurface2D *GetSurfaces() const { return m_surfaces; }

  ePixelFormat GetFormat() const { return m_surfaces[0].GetFormat(); }

  uint32_t GetHandle() const { return m_dwHandle; }

  void SetHandle(uint32_t dwHandle) { m_dwHandle = dwHandle; }

  GLenum InitializeSurface(uint32_t level, uint32_t width, uint32_t height,
                           ePixelFormat format) {
    if (m_pBoundSurface) {
      this->ReleaseSurface(m_pBoundSurface);
    }

    return m_surfaces[level].Initialize(width, height, format);
  }

  GLenum BindSurface(CGLSurface *pSurface, bool bGenMipMaps);

  GLenum ReleaseSurface(CGLSurface *pSurface);

  GLenum GenerateMipmaps();

  bool Validate();

  bool IsDirty() const { return m_bIsDirty; }

  void Invalidate() { m_bIsDirty = true; }

  uint8_t GetLogWidth() const { return m_surfaces[0].GetLogWidth(); }

  uint8_t GetLogHeight() const { return m_surfaces[0].GetLogHeight(); }

  uint8_t GetMaxMipLevel() const { return m_maxMipLevel; }

  void FreeSurfaces();

private:
  CTexture();
  ~CTexture();

  CSurface2D m_surfaces[MAX_TEXTURE_LEVELS];
  uint8_t *m_pbMipBuffer;
  CGLSurface *m_pBoundSurface;
  uint32_t m_dwHandle;
  uint8_t m_maxMipLevel;
  bool m_bIsDirty;
};

struct TexUnit {
public:
  ColorARGB cEnvColor;
  uint8_t EnvMode;
  bool bCoordReplace;

  TexUnit() : m_pTexture(nullptr) {}

  ~TexUnit() { __safeRelease(m_pTexture); }

  CTexture *GetTexture() const { return m_pTexture; }

  void SetTexture(CTexture *pTexture) {
    if (pTexture) {
      pTexture->AddRef();
    }

    if (m_pTexture) {
      m_pTexture->Release();
    }

    m_pTexture = pTexture;
  }

  ePixelFormat GetFormat() const {
    ASSERT(m_pTexture);
    return m_pTexture->GetFormat();
  }

  const TexParams &GetParams() const {
    ASSERT(m_pTexture);
    return m_pTexture->Params;
  }

  bool Prepare(Sampler *pSampler, TEXTURESTATES *pStates);

private:
  CTexture *m_pTexture;
};
