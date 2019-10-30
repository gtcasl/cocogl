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

struct PrimitiveData;
struct RasterData;
struct Register;

typedef void (*PFN_Scanline)(const RasterData &rasterData, int y, int lx,
                             int rx);

class IRasterOp : public CObject {
public:
  virtual PFN_Scanline GetScanline() const = 0;

  virtual unsigned GetCbSize() const = 0;

#ifdef COCOGL_RASTER_PROFILE
public:
  void StartProfile(unsigned numPixels);
  void EndProfile(float elapsedTime);
  void LogProfile(const RASTERID &rasterID);
  const ProfileCounter &GetProfile() const { return m_profile; }

private:
  ProfileCounter m_profile;
#endif
};

class CRasterCache : public CObject {
public:
  ~CRasterCache() {
#ifndef NDEBUG    
    if (m_slowRasterIDs.GetSize()) {
      __glLog(_T("*** BEGIN MAKE_SCANLINE().\r\n"));
      for (TList<RASTERID>::Iter iter = m_slowRasterIDs.GetBegin(),
                                 iterEnd = m_slowRasterIDs.GetEnd();
           iter != iterEnd; ++iter) {
        __glLog(_T("MAKE_SCANLINE(%d,%d,%d,%d),\r\n"), iter->Flags.Value,
                iter->States.Value, iter->Textures[0].Value,
                iter->Textures[1].Value);
      }
      __glLog(_T("*** END MAKE_SCANLINE().\r\n"));
    }
#endif
#ifdef COCOGL_RASTER_PROFILE
    {
      // Copy the cache list
      Cache::List sortedList;
      if (SUCCEEDED(m_map.ToList(&sortedList))) {
        // Sort the list
        sortedList.Sort(ProfileCompare);

        // Dump the profile result
        for (Cache::Iter iter = sortedList.GetBegin(),
                         iterEnd = sortedList.GetEnd();
             iter != iterEnd; ++iter) {
          iter->Second->LogProfile(iter->First);
        }
      }
    }
#endif

    // Delete cache entries
    for (Cache::Iter iter = m_map.GetBegin(), iterEnd = m_map.GetEnd();
         iter != iterEnd;) {
      Cache::Iter iterCur = iter++;
      __safeRelease(iterCur->Second);
    }
  }

  bool Lookup(IRasterOp **ppRasterOp, const RASTERID &rasterID) {
    return m_map.Lookup(rasterID, ppRasterOp);
  }

  HRESULT Insert(const RASTERID &rasterID, IRasterOp *pRasterOp) {
    unsigned cbTotalSize = m_cbTotalSize;
    if (cbTotalSize > MAX_CACHE_SIZE) {
      const unsigned compactSize = cbTotalSize / COMPACT_RATIO;
      if (cbTotalSize > compactSize) {
        for (Cache::Iter iter = m_map.GetRBegin(); cbTotalSize > compactSize;) {
          Cache::Iter iterCur = iter--;

          cbTotalSize -= iterCur->Second->GetCbSize();

          __safeRelease(iterCur->Second);

          m_map.Erase(iterCur->First);
        }

        m_cbTotalSize = cbTotalSize;
      }
    }

    const unsigned cbSize = pRasterOp->GetCbSize();
    m_cbTotalSize += cbSize;

    return m_map.Insert(rasterID, pRasterOp);
  }

  static GLenum Create(CRasterCache **ppRasterCache) {
    __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

    ASSERT(ppRasterCache);

    // Create a new raster cache
    CRasterCache *pRasterCache = new CRasterCache();
    if (nullptr == pRasterCache) {
      __glLogError(_T("CRasterCache allocation failed, out of memory.\r\n"));
      return GL_OUT_OF_MEMORY;
    }

    pRasterCache->AddRef();
    *ppRasterCache = pRasterCache;

    return GL_NO_ERROR;
  }

  HRESULT TrackSlowRasterID(const RASTERID &rasterID) {
    for (TList<RASTERID>::Iter iter = m_slowRasterIDs.GetBegin(),
                               iterEnd = m_slowRasterIDs.GetEnd();
         iter != iterEnd; ++iter) {
      if (*iter == rasterID) {
        return S_OK;
      }
    }

    return m_slowRasterIDs.PushBack(rasterID);
  }

  std::mutex &GetCS() { return m_CS; }

private:
  typedef TMap<RASTERID, IRasterOp *, 1> Cache;

#ifdef COCOGL_RASTER_PROFILE

  static bool ProfileCompare(const Cache::List::Type &lhs,
                             const Cache::List::Type &rhs) {
    return (lhs.Second->GetProfile() < rhs.Second->GetProfile());
  }

#endif

  enum {
    MAX_CACHE_SIZE = 0x20000,
    COMPACT_RATIO = 4,
  };

  CRasterCache() { m_cbTotalSize = 0; }

  Cache m_map;

  TList<RASTERID> m_slowRasterIDs;

  mutable std::mutex m_CS;

  unsigned m_cbTotalSize;
};

struct Register {
  enum { DIM = 3 };
  fixedRX m[DIM];
};

struct RasterData {
  fixed4 fRefX;
  fixed4 fRefY;

  Register Registers[MAX_REGISTERS];
  Sampler Samplers[MAX_TEXTURES];

  uint8_t *pColorBits;
  unsigned ColorPitch;

  uint8_t *pDepthStencilBits;
  unsigned DepthStencilPitch;

  ColorARGB cFogColor;

  uint8_t AlphaRef;
  uint8_t StencilRef;
  uint8_t StencilMask;
  uint8_t StencilWriteMask;

  unsigned ColorWriteMask;

  uint8_t ColorFormat;
  uint8_t DepthStencilFormat;
  uint16_t __padding;

  IRasterOp *pRasterOp;

  RasterData() : pRasterOp(nullptr) {}
};