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
  virtual long addRef() const = 0;
  virtual long release() const = 0;

protected:
  IObject() {}
  virtual ~IObject() {}
};

class Object : public IObject {
public:
  virtual long addRef() const {
    return ++refcount_;
  }

  virtual long release() const {
    assert(refcount_ > 0);
    auto refcount = --refcount_;
    if (0 == refcount) {
      delete this;
    }
    return refcount;
  }

protected:
  Object()
      : refcount_(0) {}
  virtual ~Object() {}

private:
  mutable std::atomic<long> refcount_;
};
