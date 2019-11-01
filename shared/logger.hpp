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

class CLogger {
public:
  enum {
    MAX_INDENT = 64,
  };

  CLogger(LPCTSTR lpszFileName = nullptr, LPCTSTR lpszMode = _T("w"));
  virtual ~CLogger();

  HRESULT Open(LPCTSTR lpszFileName, LPCTSTR lpszMode = _T("w"));

  HRESULT Write(const TCHAR *pszFormat, ...);

  HRESULT Write(const TCHAR *pszFormat, va_list arglist);

  void SetIndent(uint32_t indent);

  uint32_t GetIndent() const;

  void IncrIndent();
  void DecrIndent();

private:
  FILE *m_pFile;
  uint32_t m_indent;
};
