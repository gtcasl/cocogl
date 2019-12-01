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

  CLogger(const char *fileName = nullptr, const char *mode = "w");
  virtual ~CLogger();

  HRESULT Open(const char *fileName, const char *mode = "w");

  HRESULT Write(const char *format, ...);

  HRESULT Write(const char *format, va_list arglist);

  void SetIndent(uint32_t indent) {
    indent_ = indent;
  }

  auto GetIndent() const {
    return indent_;
  }

  void IncrIndent() {
    assert(indent_ < MAX_INDENT);
    ++indent_;
  }

  void DecrIndent() {
    assert(indent_);
    --indent_;
  }

private:
  FILE *file_;
  uint32_t indent_;
};

///////////////////////////////////////////////////////////////////////////////

class CAutoLog {
public:
  CAutoLog(CLogger &logger, const char *func, ...);

  ~CAutoLog();

private:
  CLogger &logger_;
};