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


class CMatrixStack : public CObject {
public:
  ~CMatrixStack();

  static GLenum Create(CMatrixStack **ppMatrixStack, uint8_t size);

  void ToIdentity() {
    m_pMatrices[m_curIndex].ToIdentity();
    this->SetIdentity(true);
  }

  void SetMatrix(const MATRIX44 &matrix) {
    m_pMatrices[m_curIndex] = matrix;
    this->SetIdentity(false);
  }

  const MATRIX44 &GetMatrix() const { return m_pMatrices[m_curIndex]; }

  bool Push() {
    if (m_curIndex + 1 < m_size) {
      m_pMatrices[m_curIndex + 1] = m_pMatrices[m_curIndex];
      m_identityMask = (m_identityMask & ~(2 << m_curIndex)) |
                       ((m_identityMask & (1 << m_curIndex)) << 1);
      ++m_curIndex;
      return true;
    }

    return false;
  }

  bool Pop() {
    if (m_curIndex > 0) {
      --m_curIndex;
      return true;
    }

    return false;
  }

  bool IsIdentity() const { return m_identityMask & (1 << m_curIndex); }

  unsigned GetSize() const { return m_size; }

  template <class T> void TGetTop(T *pDst) const {
    ASSERT(pDst);

    const floatf *const pSrc = m_pMatrices[m_curIndex]._m;
    for (unsigned i = 0; i < 16; ++i) {
      pDst[i] = Math::TCast<T>(pSrc[i]);
    }
  }

private:
  GLenum Initialize(uint8_t size);

  CMatrixStack();

  void SetIdentity(bool bValue) {
    if (bValue) {
      m_identityMask |= (1 << m_curIndex);
    } else {
      m_identityMask &= ~(1 << m_curIndex);
    }
  }

  MATRIX44 *m_pMatrices;
  uint16_t m_identityMask;
  uint8_t m_curIndex;
  uint8_t m_size;
};
