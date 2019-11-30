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

#include <atomic>

class IObject {
public:
  virtual long AddRef() const = 0;
  virtual long Release() const = 0;

protected:
  IObject() {}
  virtual ~IObject() {}
};

class CObject : public IObject {
public:
  virtual long AddRef() const { return ++refcount_; }

  virtual long Release() const {
    assert(refcount_ > 0);
    auto refcount = --refcount_;
    if (0 == refcount) {
      delete this;
    }
    return refcount;
  }

protected:
  CObject() : refcount_(0) {}
  virtual ~CObject() {}

private:
  mutable std::atomic<long> refcount_;
};
