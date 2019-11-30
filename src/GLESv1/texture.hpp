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
  const uint8_t *GetBits() const { return pbBits_; }

  uint8_t *GetBits() { return pbBits_; }

  uint8_t GetLogWidth() const { return logWidth_; }

  uint8_t GetLogHeight() const { return logHeight_; }

  uint16_t GetWidth() const { return 1 << logWidth_; }

  uint16_t GetHeight() const { return 1 << logHeight_; }

protected:
  uint8_t *pbBits_;

  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    uint16_t logWidth__logHeight;
    struct {
      uint8_t logWidth_;
      uint8_t logHeight_;
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

  uint32_t GetPitch() const { return pitch_; }

  ePixelFormat GetFormat() const { return static_cast<ePixelFormat>(format_); }

  void GetDesc(GLSurfaceDesc *pSurfaceDesc) const {
    assert(pSurfaceDesc);
    pSurfaceDesc->pBits = pbBits_;
    pSurfaceDesc->Width = this->GetWidth();
    pSurfaceDesc->Height = this->GetHeight();
    pSurfaceDesc->Pitch = pitch_;
    pSurfaceDesc->Format = format_;
  }

  void Destroy();

private:
  void Clear();

  int32_t pitch_;
  uint8_t format_;
  bool bOwnedBuffer_;
};

class CTexture : public CObject {
public:
  TexParams Params;
  bool bGenMipMaps;

  static GLenum Create(CTexture **ppTexture);

  const CSurface2D &GetSurface(uint32_t level) const {
    assert(level < MAX_TEXTURE_LEVELS);
    return surfaces_[level];
  }

  const CSurface2D *GetSurfaces() const { return surfaces_; }

  ePixelFormat GetFormat() const { return surfaces_[0].GetFormat(); }

  uint32_t GetHandle() const { return dwHandle_; }

  void SetHandle(uint32_t dwHandle) { dwHandle_ = dwHandle; }

  GLenum InitializeSurface(uint32_t level, uint32_t width, uint32_t height,
                           ePixelFormat format) {
    if (pBoundSurface_) {
      this->ReleaseSurface(pBoundSurface_);
    }

    return surfaces_[level].Initialize(width, height, format);
  }

  GLenum BindSurface(CGLSurface *pSurface, bool bGenMipMaps);

  GLenum ReleaseSurface(CGLSurface *pSurface);

  GLenum GenerateMipmaps();

  bool Validate();

  bool IsDirty() const { return bIsDirty_; }

  void Invalidate() { bIsDirty_ = true; }

  uint8_t GetLogWidth() const { return surfaces_[0].GetLogWidth(); }

  uint8_t GetLogHeight() const { return surfaces_[0].GetLogHeight(); }

  uint8_t GetMaxMipLevel() const { return maxMipLevel_; }

  void FreeSurfaces();

private:
  CTexture();
  ~CTexture();

  CSurface2D surfaces_[MAX_TEXTURE_LEVELS];
  uint8_t *pbMipBuffer_;
  CGLSurface *pBoundSurface_;
  uint32_t dwHandle_;
  uint8_t maxMipLevel_;
  bool bIsDirty_;
};

struct TexUnit {
public:
  ColorARGB cEnvColor;
  uint8_t EnvMode;
  bool bCoordReplace;

  TexUnit() : pTexture_(nullptr) {}

  ~TexUnit() { __safeRelease(pTexture_); }

  CTexture *GetTexture() const { return pTexture_; }

  void SetTexture(CTexture *pTexture) {
    if (pTexture) {
      pTexture->AddRef();
    }

    if (pTexture_) {
      pTexture_->Release();
    }

    pTexture_ = pTexture;
  }

  ePixelFormat GetFormat() const {
    assert(pTexture_);
    return pTexture_->GetFormat();
  }

  const TexParams &GetParams() const {
    assert(pTexture_);
    return pTexture_->Params;
  }

  bool Prepare(Sampler *pSampler, TEXTURESTATES *pStates);

private:
  CTexture *pTexture_;
};
