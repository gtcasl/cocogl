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
#include <stdarg.h>

CLogger::CLogger() {
  m_pFile = NULL;
  m_indent = 0;
}

CLogger::~CLogger() {
  if (m_pFile) {
    fclose(m_pFile);
  }
}

CLogger &CLogger::Instance() {
  static CLogger instance;
  return instance;
}

void CLogger::SetIndent(unsigned indent) { m_indent = indent; }

unsigned CLogger::GetIndent() const { return m_indent; }

void CLogger::IncrIndent() {
  ASSERT(m_indent < MAX_INDENT);
  ++m_indent;
}

void CLogger::DecrIndent() {
  ASSERT(m_indent);
  --m_indent;
}

HRESULT CLogger::Open(LPCTSTR lpszFileName, LPCTSTR lpszMode) {
  if ((NULL == lpszFileName) || (NULL == lpszMode)) {
    return E_INVALIDARG;
  }

  m_pFile = _tfopen(lpszFileName, lpszMode);
  if (NULL == m_pFile) {
    return E_FAIL;
  }

  return S_OK;
}

HRESULT CLogger::Write(const TCHAR *pszFormat, ...) {
  if ((NULL == pszFormat) || (0 == *pszFormat)) {
    return S_OK;
  }

  unsigned indent = m_indent;
  while (indent--) {
    _ftprintf(m_pFile, _T("  "));
  }

  va_list arglist;
  va_start(arglist, pszFormat);

  _vftprintf(m_pFile, pszFormat, arglist);

  va_end(arglist);

  fflush(m_pFile);

  return S_OK;
}

HRESULT CLogger::Write(const TCHAR *pszFormat, va_list arglist) {
  if ((NULL == pszFormat) || (0 == *pszFormat)) {
    return S_OK;
  }

  unsigned indent = m_indent;
  while (indent--) {
    _ftprintf(m_pFile, _T("  "));
  }

  _vftprintf(m_pFile, pszFormat, arglist);

  fflush(m_pFile);

  return S_OK;
}

///////////////////////////////////////////////////////////////////////////////

CProfiler::CProfiler(const TCHAR *pszFunc, ...) {
  va_list arglist;
  va_start(arglist, pszFunc);
  CLogger::Instance().Write(pszFunc, arglist);
  CLogger::Instance().IncrIndent();
  va_end(arglist);
}

CProfiler::~CProfiler() { 
  CLogger::Instance().DecrIndent(); 
}