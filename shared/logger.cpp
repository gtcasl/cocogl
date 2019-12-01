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

CLogger::CLogger(LPCTSTR lpszFileName, LPCTSTR lpszMode)
    : file_(nullptr), indent_(0) {
  if (lpszFileName) {
    auto hr = this->Open(lpszFileName, lpszMode);
    if (FAILED(hr)) {
      _tprintf(_T("error: couldn't open log file: %s"), lpszFileName);
    }
  }
}

CLogger::~CLogger() {
  if (file_) {
    fclose(file_);
  }
}

HRESULT CLogger::Open(LPCTSTR lpszFileName, LPCTSTR lpszMode) {
  if ((nullptr == lpszFileName) || (nullptr == lpszMode))
    return E_INVALIDARG;

  file_ = _tfopen(lpszFileName, lpszMode);
  if (nullptr == file_)
    return E_FAIL;

  return S_OK;
}

HRESULT CLogger::Write(const TCHAR *pszFormat, ...) {
  if (nullptr == file_)
    return E_FAIL;

  if ((nullptr == pszFormat) || (0 == *pszFormat))
    return S_OK;

  uint32_t indent = indent_;
  while (indent--) {
    _ftprintf(file_, _T("  "));
  }

  va_list arglist;
  va_start(arglist, pszFormat);

  _vftprintf(file_, pszFormat, arglist);

  va_end(arglist);

  return S_OK;
}

HRESULT CLogger::Write(const TCHAR *pszFormat, va_list arglist) {
  if (nullptr == file_)
    return E_FAIL;

  if ((nullptr == pszFormat) || (0 == *pszFormat))
    return S_OK;

  uint32_t indent = indent_;
  while (indent--) {
    _ftprintf(file_, _T("  "));
  }

  _vftprintf(file_, pszFormat, arglist);

  return S_OK;
}

///////////////////////////////////////////////////////////////////////////////

CAutoLog::CAutoLog(CLogger &logger, const TCHAR *pszFunc, ...)
    : logger_(logger) {
  va_list arglist;
  va_start(arglist, pszFunc);
  logger.Write(pszFunc, arglist);
  logger.IncrIndent();
  va_end(arglist);
}

CAutoLog::~CAutoLog() { 
  logger_.DecrIndent(); 
}