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


class CGDIFont : public CObject {
public:  
  static HRESULT CreateFromResource(CGDIFont **ppFont, HDC hDC,
                                    HINSTANCE hInstance, LPCTSTR lpszFileName);

  HRESULT Draw(LPCTSTR lpszText) { return (this->*m_pfnDraw)(lpszText); }

  HRESULT Present(HDC hDC, unsigned x = 0, unsigned y = 0);

private:
  typedef HRESULT (CGDIFont::*PFN_DRAW)(LPCTSTR lpszText);

  enum {
    FONT_WIDTH = 620,
    FONT_HEIGHT = 12,
    FONT_NUMCHARS = 256,
    FONT_SPACE = 32,
  };

  struct GLYPHS {
    unsigned Left;
    unsigned Width;
  };

  struct Surface {
    HBITMAP hBitmap;
    void *pBits;
    unsigned nBPP;
    unsigned Width;
    unsigned Height;

    Surface() : hBitmap(NULL), pBits(NULL), nBPP(0), Width(0), Height(0) {}

    ~Surface() {
      if (hBitmap)
        ::DeleteObject(hBitmap);
    }
  };

  CGDIFont();
  virtual ~CGDIFont();

  HRESULT Initialize(HDC hDC, HINSTANCE hInstance, LPCTSTR lpszFileName);

  template <class T> HRESULT TDraw(LPCTSTR lpszText);

  GLYPHS m_glyphs[FONT_NUMCHARS];
  PFN_DRAW m_pfnDraw;
  Surface m_font;
  Surface m_backBuffer;
  unsigned m_textWidth;
  HDC m_hDC;
};
