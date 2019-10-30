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
#include "stdafx.h"
#include "surface.hpp"
#include "surface.inl"

CGLSurface::CGLSurface() {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  memset(&m_pColorDesc, 0, sizeof(m_pColorDesc));
  memset(&m_pDepthStencilDesc, 0, sizeof(m_pDepthStencilDesc));

  m_pfnColorConv = &CGLSurface::ColorConvNoop;
  m_pfnColorFill = &CGLSurface::ColorFillNoop;
  m_pfnDepthStencilFill = &CGLSurface::ColorFillNoop;

  for (unsigned i = ATTRIBUTES_FIRST; i <= ATTRIBUTES_LAST; ++i) {
    this->SetAttribute(i, 0);
  }
}

CGLSurface::~CGLSurface() { __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__)); }

GLenum CGLSurface::Create(CGLSurface **ppSurface,
                          const GLSurfaceDesc *pColorDesc,
                          const GLSurfaceDesc *pDepthStencilDesc) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  GLenum err;

  ASSERT(ppSurface);

  // Create a new surface object
  CGLSurface *pSurface = new CGLSurface();
  if (NULL == pSurface) {
    __glLogError(_T("CGLSurface allocation failed, out of memory.\r\n"));
    return GL_OUT_OF_MEMORY;
  }

  pSurface->AddRef();

  // Initialize the surface
  err = pSurface->Initialize(pColorDesc, pDepthStencilDesc);
  if (__glFailed(err)) {
    __safeRelease(pSurface);
    __glLogError(_T("CGLSurface::Initialize() failed, err = %d.\r\n"), err);
    return err;
  }

  *ppSurface = pSurface;

  return GL_NO_ERROR;
}

GLenum CGLSurface::Initialize(const GLSurfaceDesc *pColorDesc,
                              const GLSurfaceDesc *pDepthStencilDesc) {
  __profileAPI(_T(" - %s()\n"), _T(__FUNCTION__));

  if (pColorDesc && pColorDesc->pBits) {
    m_pColorDesc = *pColorDesc;

    // Set color fill function
    switch (pColorDesc->Format) {
    case FORMAT_R5G6B5:
      m_pfnColorConv = Format::TConvertTo<FORMAT_R5G6B5>;
      m_pfnColorFill = &CGLSurface::TColorFill<uint16_t>;
      this->SetAttribute(GL_RED_BITS, 5);
      this->SetAttribute(GL_GREEN_BITS, 6);
      this->SetAttribute(GL_BLUE_BITS, 5);
      this->SetAttribute(GL_ALPHA_BITS, 0);
      break;

    /*case FORMAT_R8G8B8:
        m_pfnColorConv = Format::TConvertTo<FORMAT_B8G8R8>;
        m_pfnColorFill = &CGLSurface::TColorFill<uint24>;
        this->SetAttribute(GL_RED_BITS,   8);
        this->SetAttribute(GL_GREEN_BITS, 8);
        this->SetAttribute(GL_BLUE_BITS,  8);
        this->SetAttribute(GL_ALPHA_BITS, 0);
        break;*/

    case FORMAT_A8R8G8B8:
      m_pfnColorConv = Format::TConvertTo<FORMAT_A8B8G8R8>;
      m_pfnColorFill = &CGLSurface::TColorFill<uint32_t>;
      this->SetAttribute(GL_RED_BITS, 8);
      this->SetAttribute(GL_GREEN_BITS, 8);
      this->SetAttribute(GL_BLUE_BITS, 8);
      this->SetAttribute(GL_ALPHA_BITS, 8);
      break;
    default:
      __glLogError(
          _T("CGLSurface::Initialize() failed, invalid color format: %d.\r\n"),
          pColorDesc->Format);
      return GL_INVALID_VALUE;
    }

    if ((pColorDesc->Width & 0x1) // The width is a multiple of 2
        ||
        !__isAligned32(
            pColorDesc->pBits)) // The buffer address is uint32_t aligned
    {
      __glLogError(_T("CGLSurface::Initialize() failed, invalid color buffer ")
                   _T("alignment.\r\n"));
      return GL_INVALID_VALUE;
    }
  }

  if (pDepthStencilDesc && pDepthStencilDesc->pBits) {
    m_pDepthStencilDesc = *pDepthStencilDesc;

    switch (pDepthStencilDesc->Format) {
    case FORMAT_X8S8D16:
      m_pfnDepthStencilFill = &CGLSurface::TColorFill<uint32_t>;
      this->SetAttribute(GL_DEPTH_BITS, 16);
      this->SetAttribute(GL_STENCIL_BITS, 8);
      break;

    case FORMAT_D16:
      m_pfnDepthStencilFill = &CGLSurface::TColorFill<uint16_t>;
      this->SetAttribute(GL_DEPTH_BITS, 16);
      this->SetAttribute(GL_STENCIL_BITS, 0);
      break;

    default:
      __glLogError(_T("CGLSurface::Initialize() failed, invalid depth stencil ")
                   _T("format: %d.\r\n"),
                   pDepthStencilDesc->Format);
      return GL_INVALID_VALUE;
    }

    if ((pDepthStencilDesc->Width & 0x1) // The width is a multiple of 2
        ||
        !__isAligned32(
            pDepthStencilDesc->pBits)) // The buffer address is uint32_t aligned
    {
      __glLogError(_T("CGLSurface::Initialize() failed, invalid depth stencil ")
                   _T("buffer alignment.\r\n"));
      return GL_INVALID_VALUE;
    }
  }

  this->SetAttribute(GL_SUBPIXEL_BITS, SUBPIXEL_BITS);

  return GL_NO_ERROR;
}

GLenum CGLSurface::SaveBitmap(LPCTSTR lpszFilename) {
  // Open the file for writing bytes
  FILE *const pFile = _tfopen(lpszFilename, _T("wb"));
  if (NULL == pFile) {
    return GL_INVALID_OPERATION;
  }

  struct BitmapInfo {
    BITMAPINFOHEADER bmiHeader;
    uint32_t bmiColors[3];
  } bmpInfo;

  const uint8_t nBPP = Format::GetInfo(m_pColorDesc.Format).BytePerPixel;

  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = m_pColorDesc.Width;
  bmpInfo.bmiHeader.biHeight = m_pColorDesc.Height;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biBitCount = nBPP << 3;
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage = m_pColorDesc.Height * m_pColorDesc.Pitch;
  bmpInfo.bmiHeader.biXPelsPerMeter = 0;
  bmpInfo.bmiHeader.biYPelsPerMeter = 0;
  bmpInfo.bmiHeader.biClrUsed = 0;
  bmpInfo.bmiHeader.biClrImportant = 0;

  if (2 == nBPP) {
    bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;
    bmpInfo.bmiColors[0] = 0xF800;
    bmpInfo.bmiColors[1] = 0x07E0;
    bmpInfo.bmiColors[2] = 0x001F;
  }

  const unsigned bitSize = bmpInfo.bmiHeader.biSizeImage;
  const unsigned infoSize = sizeof(BitmapInfo);
  const unsigned size = sizeof(BITMAPFILEHEADER) + infoSize + bitSize;

  // Write the file header
  BITMAPFILEHEADER header;
  header.bfType = 'MB';
  header.bfSize = size;
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits = sizeof(BITMAPFILEHEADER) + infoSize;

  if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), pFile) <
      sizeof(BITMAPFILEHEADER)) {
    fclose(pFile);
    return GL_INVALID_OPERATION;
  }

  if (fwrite(&bmpInfo, 1, infoSize, pFile) < infoSize) {
    fclose(pFile);
    return GL_INVALID_OPERATION;
  }

  if (fwrite(m_pColorDesc.pBits, 1, bitSize, pFile) < bitSize) {
    fclose(pFile);
    return GL_INVALID_OPERATION;
  }

  fclose(pFile);

  return GL_NO_ERROR;
}
