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

Logger::Logger(const char *fileName, const char *mode)
    : file_(nullptr), indent_(0) {
  if (fileName) {
    auto hr = this->open(fileName, mode);
    if (FAILED(hr)) {
      fprintf(stderr, "error: couldn't open log file: %s", fileName);
    }
  }
}

Logger::~Logger() {
  if (file_) {
    fclose(file_);
  }
}

HRESULT Logger::open(const char *fileName, const char *mode) {
  if ((nullptr == fileName) || (nullptr == mode))
    return E_INVALIDARG;

  file_ = fopen(fileName, mode);
  if (nullptr == file_)
    return E_FAIL;

  return S_OK;
}

HRESULT Logger::write(const char *format, ...) {
  if (nullptr == file_)
    return E_FAIL;

  if ((nullptr == format) || (0 == *format))
    return S_OK;

  uint32_t indent = indent_;
  while (indent--) {
    fprintf(file_, "  ");
  }

  va_list arglist;
  va_start(arglist, format);

  vfprintf(file_, format, arglist);

  va_end(arglist);

  return S_OK;
}

HRESULT Logger::write(const char *format, va_list arglist) {
  if (nullptr == file_)
    return E_FAIL;

  if ((nullptr == format) || (0 == *format))
    return S_OK;

  uint32_t indent = indent_;
  while (indent--) {
    fprintf(file_, "  ");
  }

  vfprintf(file_, format, arglist);

  return S_OK;
}

///////////////////////////////////////////////////////////////////////////////

AutoLog::AutoLog(Logger &logger, const char *func, ...)
    : logger_(logger) {
  va_list arglist;
  va_start(arglist, func);
  logger.write(func, arglist);
  logger.incrIndent();
  va_end(arglist);
}

AutoLog::~AutoLog() { 
  logger_.decrIndent(); 
}