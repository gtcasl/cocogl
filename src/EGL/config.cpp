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
// installation. THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES
// OR INDEMNITIES.
//
#include "stdafx.h"
#include "config.hpp"

_EGLConfig::_EGLConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha,
                       EGLint depth, EGLint stencil) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  this->setAttribute(EGL_BUFFER_SIZE, red + green + blue + alpha);
  this->setAttribute(EGL_RED_SIZE, red);
  this->setAttribute(EGL_GREEN_SIZE, green);
  this->setAttribute(EGL_BLUE_SIZE, blue);
  this->setAttribute(EGL_ALPHA_SIZE, alpha);
  this->setAttribute(EGL_DEPTH_SIZE, depth);
  this->setAttribute(EGL_STENCIL_SIZE, stencil);
  this->setAttribute(EGL_CONFIG_CAVEAT, EGL_NONE);
  this->setAttribute(EGL_CONFIG_ID, 0);
  this->setAttribute(EGL_LEVEL, 0);
  this->setAttribute(EGL_MAX_PBUFFER_HEIGHT, MAX_PBUFFER_WIDTH);
  this->setAttribute(EGL_MAX_PBUFFER_PIXELS, MAX_PBUFFER_PIXELS);
  this->setAttribute(EGL_MAX_PBUFFER_WIDTH, MAX_PBUFFER_WIDTH);
  this->setAttribute(EGL_NATIVE_RENDERABLE, EGL_FALSE);
  this->setAttribute(EGL_NATIVE_VISUAL_ID, 0);
  this->setAttribute(EGL_NATIVE_VISUAL_TYPE, 0);
  this->setAttribute(EGL_SAMPLES, 0);
  this->setAttribute(EGL_SAMPLE_BUFFERS, 0);
  this->setAttribute(EGL_SURFACE_TYPE,
                     EGL_WINDOW_BIT | EGL_PIXMAP_BIT | EGL_PBUFFER_BIT);
  this->setAttribute(EGL_TRANSPARENT_TYPE, EGL_NONE);
  this->setAttribute(EGL_TRANSPARENT_BLUE_VALUE, 0);
  this->setAttribute(EGL_TRANSPARENT_GREEN_VALUE, 0);
  this->setAttribute(EGL_TRANSPARENT_RED_VALUE, 0);
  this->setAttribute(alpha ? EGL_BIND_TO_TEXTURE_RGBA : EGL_BIND_TO_TEXTURE_RGB,
                     EGL_TRUE);
  this->setAttribute(EGL_MIN_SWAP_INTERVAL, 1);
  this->setAttribute(EGL_MAX_SWAP_INTERVAL, 1);
  this->setAttribute(EGL_LUMINANCE_SIZE, 0);
  this->setAttribute(EGL_ALPHA_MASK_SIZE, 0);
  this->setAttribute(EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER);
  this->setAttribute(EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT);
}

_EGLConfig::~_EGLConfig() {
  __profileAPI(" - %s()\n", __FUNCTION__);
}

EGLint _EGLConfig::Create(_EGLConfig **ppConfig, EGLint red, EGLint green,
                          EGLint blue, EGLint alpha, EGLint depth,
                          EGLint stencil) {
  __profileAPI(" - %s()\n", __FUNCTION__);

  assert(ppConfig);

  auto pConfig = new _EGLConfig(red, green, blue, alpha, depth, stencil);
  if (nullptr == pConfig) {
    __eglLogError("EGLConfig allocation failed, out of memory.\r\n");
    return EGL_BAD_ALLOC;
  }

  pConfig->addRef();

  *ppConfig = pConfig;

  return EGL_SUCCESS;
}

EGLint _EGLConfig::getAtttribute(EGLint name, EGLint *pValue) const {
  assert(pValue);

  if ((name < ATTRIBUTES_FIRST) || (name > ATTRIBUTES_LAST) ||
      (EGL_NONE == name)) {
    __eglLogError("Invalid attribute name : %d.\r\n", name);
    return EGL_BAD_ATTRIBUTE;
  }

  *pValue = attributes_[name - ATTRIBUTES_FIRST];

  return EGL_SUCCESS;
}

EGLint _EGLConfig::matches(const EGLint *pAttrib_list, bool *pbResult) const {
  EGLint err;

  assert(pbResult);

  *pbResult = false;

  if (pAttrib_list) {
    while (EGL_NONE != *pAttrib_list) {
      EGLint name = *pAttrib_list++;
      EGLint refValue = *pAttrib_list++;

      EGLint curValue;
      err = this->getAtttribute(name, &curValue);
      if (__eglFailed(err)) {
        __eglLogError("Config::getAtttribute() failed, err = %d.\r\n", err);
        return err;
      }

      switch (name) {
      case EGL_BUFFER_SIZE:
      case EGL_SAMPLES:
      case EGL_SAMPLE_BUFFERS:
        if (refValue != EGL_DONT_CARE) {
          if (refValue < 0) {
            __eglLogError("Invalid attribute value : %d.\r\n", refValue);
            return EGL_BAD_ATTRIBUTE;
          }

          if (refValue > curValue) {
            return EGL_SUCCESS;
          }
        }
        break;

      case EGL_ALPHA_SIZE:
      case EGL_BLUE_SIZE:
      case EGL_GREEN_SIZE:
      case EGL_RED_SIZE:
      case EGL_DEPTH_SIZE:
      case EGL_STENCIL_SIZE:
        if (refValue != EGL_DONT_CARE) {
          if (refValue < 0) {
            __eglLogError("Invalid attribute value : %d.\r\n", refValue);
            return EGL_BAD_ATTRIBUTE;
          }

          if ((0 != refValue) && (refValue > curValue)) {
            return EGL_SUCCESS;
          }
        }
        break;

      case EGL_CONFIG_CAVEAT:
        if (refValue != EGL_DONT_CARE) {
          if ((refValue != EGL_NONE) && (refValue != EGL_SLOW_CONFIG) &&
              (refValue != EGL_NON_CONFORMANT_CONFIG)) {
            __eglLogError("Invalid attribute value : %d.\r\n", refValue);
            return EGL_BAD_ATTRIBUTE;
          }

          if (refValue != curValue) {
            return EGL_SUCCESS;
          }
        }
        break;

      case EGL_CONFIG_ID:
        if (refValue != EGL_DONT_CARE) {
          if (refValue == curValue) {
            *pbResult = true;
          }

          return EGL_SUCCESS;
        }
        break;

      case EGL_LEVEL:
        if (refValue != curValue) {
          return EGL_SUCCESS;
        }
        break;

      case EGL_NATIVE_RENDERABLE:
      case EGL_BIND_TO_TEXTURE_RGB:
      case EGL_BIND_TO_TEXTURE_RGBA:
        if (refValue != EGL_DONT_CARE) {
          if ((refValue != EGL_TRUE) && (refValue != EGL_FALSE)) {
            __eglLogError("Invalid attribute value : %d.\r\n", refValue);
            return EGL_BAD_ATTRIBUTE;
          }

          if (refValue != curValue) {
            return EGL_SUCCESS;
          }
        }
        break;

      case EGL_SURFACE_TYPE:
        if (refValue & ~(EGL_WINDOW_BIT | EGL_PBUFFER_BIT | EGL_PIXMAP_BIT)) {
          __eglLogError("Invalid attribute value : %d.\r\n", refValue);
          return EGL_BAD_ATTRIBUTE;
        }

        if (refValue != (refValue & curValue)) {
          return EGL_SUCCESS;
        }
        break;

      case EGL_TRANSPARENT_TYPE:
        if ((refValue != EGL_NONE) && (refValue != EGL_TRANSPARENT_RGB)) {
          __eglLogError("Invalid attribute value : %d.\r\n", refValue);
          return EGL_BAD_ATTRIBUTE;
        }

        if (refValue != curValue) {
          return EGL_SUCCESS;
        }
        break;

      case EGL_TRANSPARENT_BLUE_VALUE:
      case EGL_TRANSPARENT_GREEN_VALUE:
      case EGL_TRANSPARENT_RED_VALUE:
        if (refValue != EGL_DONT_CARE) {
          if (refValue < 0) {
            __eglLogError("Invalid attribute value : %d.\r\n", refValue);
            return EGL_BAD_ATTRIBUTE;
          }

          if (refValue != curValue) {
            return EGL_SUCCESS;
          }
        }
        break;

      case EGL_MIN_SWAP_INTERVAL:
        if (refValue != EGL_DONT_CARE) {
          if (refValue > curValue) {
            return EGL_SUCCESS;
          }
        }
        break;

      case EGL_MAX_SWAP_INTERVAL:
        if (refValue != EGL_DONT_CARE) {
          if (refValue < curValue) {
            return EGL_SUCCESS;
          }
        }
        break;

      default:
        __eglLogError("Invalid attribute name : %d.\r\n", name);
        return EGL_BAD_ATTRIBUTE;
      }
    }
  }

  *pbResult = true;

  return EGL_SUCCESS;
}

int _EGLConfig::compare(const _EGLConfig *pConfigA,
                        const _EGLConfig *pConfigB) {
  assert(pConfigA && pConfigB);

  static const EGLint sortedList[] = {
      EGL_CONFIG_CAVEAT,
      EGL_RED_SIZE,
      EGL_GREEN_SIZE,
      EGL_BLUE_SIZE,
      EGL_ALPHA_SIZE,
      EGL_BUFFER_SIZE,
      EGL_SAMPLE_BUFFERS,
      EGL_SAMPLES,
      EGL_DEPTH_SIZE,
      EGL_STENCIL_SIZE,
      EGL_NATIVE_VISUAL_TYPE,
      EGL_LUMINANCE_SIZE,
      EGL_ALPHA_MASK_SIZE,
      EGL_CONFIG_ID,
  };

  for (int i = 0; i < __countof(sortedList); ++i) {
    EGLint name = sortedList[i];
    EGLint valueA = pConfigA->getAttribute(name);
    EGLint valueB = pConfigB->getAttribute(name);

    switch (name) {
    case EGL_CONFIG_CAVEAT:
    case EGL_BUFFER_SIZE:
    case EGL_SAMPLE_BUFFERS:
    case EGL_SAMPLES:
    case EGL_DEPTH_SIZE:
    case EGL_STENCIL_SIZE:
    case EGL_ALPHA_MASK_SIZE:
    case EGL_NATIVE_VISUAL_TYPE:
    case EGL_CONFIG_ID:
      if (valueA < valueB) {
        return -1;
      } else if (valueA > valueB) {
        return 1;
      }
      break;

    case EGL_RED_SIZE:
    case EGL_GREEN_SIZE:
    case EGL_BLUE_SIZE:
    case EGL_ALPHA_SIZE:
    case EGL_LUMINANCE_SIZE:
      if (valueA > valueB) {
        return -1;
      } else if (valueA < valueB) {
        return 1;
      }
      break;
    }
  }

  return 0;
}
