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
#include "gdifont.hpp"


template <class T> HRESULT CGDIFont::TDraw(LPCTSTR lpszText) {
  // Check input parameters
  if (NULL == lpszText) {
    return E_INVALIDARG;
  }

  TCHAR c;

  const unsigned fontWidth = m_font.Width;
  const T *const pFontStart = reinterpret_cast<const T *>(m_font.pBits);

  T *pBackBuffer = reinterpret_cast<T *>(m_backBuffer.pBits);
  const unsigned backWidth = m_backBuffer.Width;
  const unsigned height = m_backBuffer.Height;
  const unsigned nextChar = backWidth * height;

  while (0 != (c = *lpszText++)) {
    const GLYPHS glyph = m_glyphs[c];
    const T *pFont = pFontStart + glyph.Left;
    for (unsigned h = height; h--;) {
      for (unsigned i = 0; i < glyph.Width; ++i) {
        pBackBuffer[i] = pFont[i];
      }

      pBackBuffer += backWidth;
      pFont += fontWidth;
    }

    pBackBuffer -= (nextChar - glyph.Width);
  }

  m_textWidth = (pBackBuffer - reinterpret_cast<T *>(m_backBuffer.pBits));

  return S_OK;
}


CGDIFont::CGDIFont() {
  memset(m_glyphs, 0, sizeof(m_glyphs));
  m_lRefCount = 0;
  m_pfnDraw = NULL;
  m_textWidth = 0;
  m_hDC = NULL;
}


CGDIFont::~CGDIFont() {
  if (m_hDC) {
    ::DeleteDC(m_hDC);
  }
}


HRESULT
CGDIFont::CreateFromResource(CGDIFont **ppFont, HDC hDC, HINSTANCE hInstance,
                             LPCTSTR lpszFileName) {
  HRESULT hr;

  if ((NULL == ppFont) || (NULL == lpszFileName)) {
    return E_INVALIDARG;
  }

  CGDIFont *pFont = new CGDIFont();
  if (NULL == pFont) {
    return E_OUTOFMEMORY;
  }

  pFont->AddRef();

  hr = pFont->Initialize(hDC, hInstance, lpszFileName);
  if (FAILED(hr)) {
    pFont->Release();
    return hr;
  }

  *ppFont = pFont;

  return S_OK;
}


HRESULT
CGDIFont::Initialize(HDC hDC, HINSTANCE hInstance, LPCTSTR lpszFileName) {
  HRESULT hr = S_OK;

  ASSERT(lpszFileName);

  struct BitmapInfo {
    BITMAPINFOHEADER bmiHeader;
    uint32_t bmiColors[3];
  } bmpInfo;

  BITMAP bmpFont;
  HBITMAP hBmpFont = NULL;
  HDC hFontDC = NULL;

  // Load the bitmap font from the resource
  hBmpFont = ::LoadBitmap(hInstance, lpszFileName);
  if (NULL == hBmpFont) {
    hr = HRESULT_FROM_WIN32(::GetLastError());
    goto L_EXIT;
  }

  // Create a compatible device
  hFontDC = ::CreateCompatibleDC(hDC);
  if (NULL == hFontDC) {
    hr = HRESULT_FROM_WIN32(::GetLastError());
    goto L_EXIT;
  }

  // Select the font bitmap into the device
  (HBITMAP)::SelectObject(hFontDC, hBmpFont);

  // Retrieve the font BITMAP properties
  if (0 == ::GetObject(hBmpFont, sizeof(BITMAP), &bmpFont)) {
    hr = HRESULT_FROM_WIN32(::GetLastError());
    goto L_EXIT;
  }

  m_font.Width = bmpFont.bmWidth;
  m_font.Height = bmpFont.bmHeight - 1;
  m_font.nBPP = ::GetDeviceCaps(hFontDC, BITSPIXEL) >> 3;

  // Select the draw routine
  switch (m_font.nBPP) {
  case 1:
    m_pfnDraw = &CGDIFont::TDraw<uint8_t>;
    break;
  case 2:
    m_pfnDraw = &CGDIFont::TDraw<uint16_t>;
    break;
  case 4:
    m_pfnDraw = &CGDIFont::TDraw<uint32_t>;
    break;
  default:
    hr = E_INVALIDARG;
    goto L_EXIT;
  }

  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = m_font.Width;
  bmpInfo.bmiHeader.biHeight = m_font.Height;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biBitCount = (uint16_t)(m_font.nBPP << 3);
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage = m_font.Width * m_font.Height * m_font.nBPP;
  bmpInfo.bmiHeader.biXPelsPerMeter = 0;
  bmpInfo.bmiHeader.biYPelsPerMeter = 0;
  bmpInfo.bmiHeader.biClrUsed = 0;
  bmpInfo.bmiHeader.biClrImportant = 0;

  if (2 == m_font.nBPP) {
    bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;
    bmpInfo.bmiColors[0] = 0xF800;
    bmpInfo.bmiColors[1] = 0x07E0;
    bmpInfo.bmiColors[2] = 0x001F;
  }

  // Create the font DIB bitmap
  m_font.hBitmap =
      ::CreateDIBSection(hDC, reinterpret_cast<BITMAPINFO *>(&bmpInfo),
                         DIB_RGB_COLORS, &m_font.pBits, NULL, 0);
  if (NULL == m_font.hBitmap) {
    hr = EGL_BAD_ALLOC;
    goto L_EXIT;
  }

  // Create the back buffer device context
  m_hDC = ::CreateCompatibleDC(hDC);
  if (NULL == m_hDC) {
    hr = HRESULT_FROM_WIN32(::GetLastError());
    goto L_EXIT;
  }

  // Select the DIB font into the back buffer device context
  (HBITMAP)::SelectObject(m_hDC, m_font.hBitmap);

  // Copy the source font bitmap into the DIB font
  if (!::BitBlt(m_hDC, 0, 0, m_font.Width, m_font.Height, hFontDC, 0, 1,
                SRCCOPY)) {
    hr = HRESULT_FROM_WIN32(::GetLastError());
    goto L_EXIT;
  }

  m_backBuffer.Width = ::GetDeviceCaps(hDC, HORZRES);
  m_backBuffer.Height = m_font.Height;
  m_backBuffer.nBPP = m_font.nBPP;

  // Create the DIB section for the back buffer bitmap
  bmpInfo.bmiHeader.biWidth = m_backBuffer.Width;
  bmpInfo.bmiHeader.biSizeImage =
      m_backBuffer.Width * m_backBuffer.Height * m_backBuffer.nBPP;

  // Create the DBI section for the back buffer
  m_backBuffer.hBitmap =
      ::CreateDIBSection(m_hDC, reinterpret_cast<BITMAPINFO *>(&bmpInfo),
                         DIB_RGB_COLORS, &m_backBuffer.pBits, NULL, 0);
  if (NULL == m_backBuffer.hBitmap) {
    hr = EGL_BAD_ALLOC;
    goto L_EXIT;
  }

  // Select the back buffer DIB into its device context
  (HBITMAP)::SelectObject(m_hDC, m_backBuffer.hBitmap);

  // Build the font glypth table
  const COLORREF sepColor = ::GetPixel(hFontDC, 0, 0);
  for (unsigned x = 0, c = FONT_SPACE;
       (x < m_font.Width) && (c < FONT_NUMCHARS);) {
    COLORREF color = ::GetPixel(hFontDC, x, 0);
    if (sepColor != color) {
      m_glyphs[c].Left = x;

      while ((sepColor != color) && (x < m_font.Width)) {
        ++x;
        color = ::GetPixel(hFontDC, x, 0);
      }

      m_glyphs[c].Width = x - m_glyphs[c].Left;

      ++c;
    } else {
      ++x;
    }
  }

L_EXIT:
  if (hBmpFont) {
    ::DeleteObject(hBmpFont);
  }

  if (hFontDC) {
    ::DeleteDC(hFontDC);
  }

  return hr;
}


HRESULT
CGDIFont::Present(HDC hDC, unsigned x, unsigned y) {
  if (!::BitBlt(hDC, x, y, m_textWidth, m_backBuffer.Height, m_hDC, 0, 0,
                SRCCOPY)) {
    return HRESULT_FROM_WIN32(::GetLastError());
  }

  return S_OK;
}
