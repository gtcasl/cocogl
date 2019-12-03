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

class Logger {
public:
  enum {
    MAX_INDENT = 64,
  };

  Logger(const char *fileName = nullptr, const char *mode = "w");
  virtual ~Logger();

  HRESULT open(const char *fileName, const char *mode = "w");

  HRESULT write(const char *format, ...);

  HRESULT write(const char *format, va_list arglist);

  void setIndent(uint32_t indent) { indent_ = indent; }

  auto getIndent() const { return indent_; }

  void incrIndent() {
    assert(indent_ < MAX_INDENT);
    ++indent_;
  }

  void decrIndent() {
    assert(indent_);
    --indent_;
  }

private:
  FILE *file_;
  uint32_t indent_;
};

///////////////////////////////////////////////////////////////////////////////

class AutoLog {
public:
  AutoLog(Logger &logger, const char *func, ...);
  ~AutoLog();

private:
  Logger &logger_;
};