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
  virtual long AddRef() const { return ++m_lRefCount; }

  virtual long Release() const {
    ASSERT(m_lRefCount > 0);
    const long lRefCount = --m_lRefCount;
    if (0 == lRefCount) {
      delete this;
    }

    return lRefCount;
  }

protected:
  CObject() : m_lRefCount(0) {}

  virtual ~CObject() {}

private:
  mutable std::atomic<long> m_lRefCount;
};
