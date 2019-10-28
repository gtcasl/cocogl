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


class CTimer {
public:
  CTimer() {
    int64_t frequency;
    ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&frequency));
    m_fFrequency = 1.0f / frequency;
    this->Reset();
  }

  void Reset() {
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_startTime));
    m_fFPS = 0;
    m_nFrameCount = 0;
  }

  float GetFPS() {
    ++m_nFrameCount;

    int64_t currentTime;
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&currentTime));

    const float fElapsedTime = (currentTime - m_startTime) * m_fFrequency;
    if (fElapsedTime > 1.0f) {
      m_startTime = currentTime;
      m_fFPS = m_nFrameCount / fElapsedTime;
      m_nFrameCount = 0;
    }

    return m_fFPS;
  }

private:
  float m_fFrequency;
  int64_t m_startTime;
  unsigned m_nFrameCount;
  float m_fFPS;
};
