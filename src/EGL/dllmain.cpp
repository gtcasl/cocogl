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
#include "context.hpp"
#include "display.hpp"
#include "driver.hpp"
#include "surface.hpp"

static _EGLDriver g_driver;

const char *SurfaceAttributeToString(EGLint attribute) {
  switch (attribute) {
  case EGL_CONFIG_ID:
    return "EGL_CONFIG_ID";
  case EGL_HEIGHT:
    return "EGL_HEIGHT";
  case EGL_WIDTH:
    return "EGL_WIDTH";
  case EGL_LARGEST_PBUFFER:
    return "EGL_LARGEST_PBUFFER";
  case EGL_TEXTURE_FORMAT:
    return "EGL_TEXTURE_FORMAT";
  case EGL_MIPMAP_TEXTURE:
    return "EGL_MIPMAP_TEXTURE";
  case EGL_MIPMAP_LEVEL:
    return "EGL_MIPMAP_LEVEL";
  }

  return "UNKNOWN";
}

const char *BufferToString(EGLint buffer) {
  switch (buffer) {
  case EGL_BACK_BUFFER:
    return "EGL_BACK_BUFFER";
  case EGL_SINGLE_BUFFER:
    return "EGL_SINGLE_BUFFER";
  }

  return "UNKNOWN";
}

const char *SurfaceTypeToString(EGLint readdraw) {
  switch (readdraw) {
  case EGL_DRAW:
    return "EGL_DRAW";
  case EGL_READ:
    return "EGL_READ";
  }

  return "UNKNOWN";
}

const char *EngineToString(EGLint engine) {
  switch (engine) {
  case EGL_CORE_NATIVE_ENGINE:
    return "EGL_CORE_NATIVE_ENGINE";
  }

  return "UNKNOWN";
}

EGLAPI EGLint EGLAPIENTRY eglGetError() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  EGLint err = g_driver.getError();

  __eglLog(" - eglGetError()=%d\r\n", err);

  return err;
}

EGLAPI void (*EGLAPIENTRY eglGetProcAddress(const char *procName))() {
  __profileAPI(" - %s( procName=%s )\n", __FUNCTION__, procName);

  typedef void (*PfnProcAddress)();

  struct t_procedure {
    const char *name;
    PfnProcAddress pfn;
  };

  static const t_procedure procedures[] = {
      {"glQueryMatrixxOES",
       reinterpret_cast<PfnProcAddress>(glQueryMatrixxOES)},
      {"glPointSizePointerOES",
       reinterpret_cast<PfnProcAddress>(glPointSizePointerOES)},
  };

  if (procName) {
    for (int i = 0; i < __countof(procedures); ++i) {
      if (!strcmp(procedures[i].name, procName)) {
        return procedures[i].pfn;
      }
    }
  }

  return nullptr;
}

EGLAPI EGLenum EGLAPIENTRY eglQueryAPI(void) {
  return EGL_OPENGL_ES_API;
}

EGLAPI EGLBoolean EGLAPIENTRY eglBindAPI(EGLenum api) {
  __profileAPI(" - %s( api=%d )\n", __FUNCTION__, api);
  if (api != EGL_OPENGL_ES_API)
    return EGL_FALSE; 
  return EGL_TRUE;
}

EGLAPI EGLDisplay EGLAPIENTRY eglGetDisplay(EGLNativeDisplayType display_id) {
  __profileAPI(" - %s( display_id=%d )\n", __FUNCTION__, display_id);

  EGLint err;

  uint32_t handle;
  err = g_driver.getDisplay(&handle, display_id);
  if (__eglFailed(err)) {
    __eglError(err, "EGLDriver::getDisplay() failed, err = %d.\r\n", err);
    return EGL_NO_DISPLAY;
  }

  return reinterpret_cast<EGLDisplay>(handle);
}

EGLAPI EGLBoolean EGLAPIENTRY eglInitialize(EGLDisplay display, EGLint *pMajor,
                                            EGLint *pMinor) {
  __profileAPI(" - %s( display=%d, pMajor=0x%p, pMinor=0x%p )\n", __FUNCTION__,
               display, pMajor, pMinor);

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Initialize the display
  EGLint err = pDisplay->initialize(pMajor, pMinor);
  if (__eglFailed(err)) {
    __eglError(err, "Display::initialize() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglTerminate(EGLDisplay display) {
  __profileAPI(" - %s( display=%d )\n", __FUNCTION__, display);

  // Remove the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // terminate the display
  pDisplay->terminate();

  return EGL_TRUE;
}

EGLAPI const char *EGLAPIENTRY eglQueryString(EGLDisplay display, EGLint name) {
  __profileAPI(" - %s( display=%d, name=%d )\n", __FUNCTION__, display, name);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return nullptr;
  }

  const char *value;

  // Query the display string
  err = pDisplay->queryString(&value, name);
  if (__eglFailed(err)) {
    __eglError(err, "Display::queryString() failed, err = %d.\r\n", err);
    return nullptr;
  }

  return value;
}

EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigs(EGLDisplay display,
                                            EGLConfig *pConfigs,
                                            EGLint config_size,
                                            EGLint *pNum_config) {
  __profileAPI(" - %s( display=%d, pConfigs=0x%p, config_size=%d, "
               "pNum_config=0x%p )\n",
               __FUNCTION__, display, pConfigs, config_size, pNum_config);

  return eglChooseConfig(display, nullptr, pConfigs, config_size, pNum_config);
}

EGLAPI EGLBoolean EGLAPIENTRY eglChooseConfig(EGLDisplay display,
                                              const EGLint *pAttrib_list,
                                              EGLConfig *pConfigs,
                                              EGLint config_size,
                                              EGLint *pNum_config) {
  __profileAPI(" - %s( display=%d, pAttrib_list=0x%p, pConfigs=0x%p, "
               "config_size=%d, pNum_config=0x%p )\n",
               __FUNCTION__, display, pAttrib_list, pConfigs, config_size,
               pNum_config);

  EGLint err;

  // Validate num_config pointer
  if (nullptr == pNum_config) {
    __eglError(EGL_BAD_PARAMETER, "num_config is nullptr.\r\n");
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Choose the display configurations
  err = pDisplay->chooseConfig(pAttrib_list, pConfigs, config_size, pNum_config);
  if (__eglFailed(err)) {
    __eglError(err, "Display::chooseConfig() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigAttrib(EGLDisplay display,
                                                 EGLConfig config,
                                                 EGLint attribute,
                                                 EGLint *pValue) {
  __profileAPI(" - %s( display=%d, config=%d, attribute=%d, pValue=0x%p )\n",
               __FUNCTION__, display, config, attribute, pValue);

  EGLint err;

  // Verify value parameter
  if (nullptr == pValue) {
    __eglError(EGL_BAD_PARAMETER, "value is nullptr.\r\n");
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the config object from the handle table
  auto pConfig = pDisplay->getObject<_EGLConfig *>(config);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, "Invalid config handle.\r\n");
    return EGL_FALSE;
  }

  // Retrieve the attriubute value
  err = pConfig->getAtttribute(attribute, pValue);
  if (__eglFailed(err)) {
    __eglError(err, "Config::getAttribute() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreateWindowSurface(EGLDisplay display, 
                       EGLConfig config,
                       EGLNativeWindowType hWnd, 
                       const EGLint *pAttrib_list) {
  __profileAPI(" - %s( display=%d, config=%d, hWnd=0x%p, pAttrib_list=0x%p )\n",
               __FUNCTION__, display, config, hWnd, pAttrib_list);

  EGLint err;

  // Verify that the attribute list is nullptr or empty
  if (pAttrib_list && (*pAttrib_list != EGL_NONE)) {
    __eglError(EGL_BAD_ATTRIBUTE,
               "pAttrib_list should be nullptr or empty.\r\n");
    return EGL_NO_SURFACE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_NO_SURFACE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_NO_SURFACE;
  }

  // Find the config object from the handle table
  auto pConfig = pDisplay->getObject<_EGLConfig *>(config);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, "Invalid config handle.\r\n");
    return EGL_NO_SURFACE;
  }

  // Check if the config support this surface type
  if (0 == (pConfig->getAttribute(EGL_SURFACE_TYPE) & EGL_WINDOW_BIT)) {
    __eglError(EGL_BAD_MATCH,
               "The config doesn't support this surface type.\r\n");
    return EGL_NO_SURFACE;
  }

  // Create a window surface
  _EGLSurface *pSurface;
  err = _EGLSurface::CreateWND(&pSurface, pDisplay, EGL_WINDOW_BIT, pConfig, hWnd);
  if (__eglFailed(err)) {
    __eglError(err, "_EGLSurface::Create() failed, err = %d.\r\n", err);
    return EGL_NO_SURFACE;
  }

  // Create the new surface handle
  uint32_t handle;
  err = pDisplay->registerObject(&handle, pSurface, HANDLE_SURFACE);
  if (__eglFailed(err)) {
    __eglError(err, "EGLDriver::registerObject() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return EGL_NO_SURFACE;
  }

  return reinterpret_cast<EGLSurface>(handle);
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePixmapSurface(EGLDisplay display, EGLConfig config,
                       EGLNativePixmapType pixmap, const EGLint *pAttrib_list) {
  __profileAPI(
      " - %s( display=%d, config=%d, pixmap=0x%x, pAttrib_list=0x%p )\n",
      __FUNCTION__, display, config, pixmap, pAttrib_list);

  EGLint err;

  // Verify that the attribute list is nullptr or empty
  if (pAttrib_list && (*pAttrib_list != EGL_NONE)) {
    __eglError(EGL_BAD_ATTRIBUTE,
               "pAttrib_list should be nullptr or empty.\r\n");
    return EGL_NO_SURFACE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_NO_SURFACE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_NO_SURFACE;
  }

  // Find the config object from the handle table
  auto pConfig = pDisplay->getObject<_EGLConfig *>(config);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, "Invalid config handle.\r\n");
    return EGL_NO_SURFACE;
  }

  // Check if the config support this surface type
  if (0 == (pConfig->getAttribute(EGL_SURFACE_TYPE) & EGL_PIXMAP_BIT)) {
    __eglError(EGL_BAD_MATCH,
               "The config doesn't support this surface type.\r\n");
    return EGL_NO_SURFACE;
  }

  // Create a window surface
  _EGLSurface *pSurface;
  err = _EGLSurface::CreatePXM(&pSurface, pDisplay, EGL_PIXMAP_BIT, pConfig,
                               pixmap);
  if (__eglFailed(err)) {
    __eglError(err, "_EGLSurface::Create() failed, err = %d.\r\n", err);
    return EGL_NO_SURFACE;
  }

  // Create the new surface handle
  uint32_t handle;
  err = pDisplay->registerObject(&handle, pSurface, HANDLE_SURFACE);
  if (__eglFailed(err)) {
    __eglError(err, "EGLDriver::registerObject() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return EGL_NO_SURFACE;
  }

  return reinterpret_cast<EGLSurface>(handle);
}

EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferSurface(
    EGLDisplay display, EGLConfig config, const EGLint *pAttrib_list) {
  __profileAPI(" - %s( display=%d, config=%d, pAttrib_list=0x%p )\n",
               __FUNCTION__, display, config, pAttrib_list);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_NO_SURFACE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_NO_SURFACE;
  }

  // Find the config object from the handle table
  auto pConfig = pDisplay->getObject<_EGLConfig *>(config);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, "Invalid config handle.\r\n");
    return EGL_NO_SURFACE;
  }

  // Check if the config support this surface type
  if (0 == (pConfig->getAttribute(EGL_SURFACE_TYPE) & EGL_PBUFFER_BIT)) {
    __eglError(EGL_BAD_MATCH,
               "The config doesn't support this surface type.\r\n");
    return EGL_NO_SURFACE;
  }

  EGLint width = 0;
  EGLint height = 0;
  EGLint largestPBuffer = EGL_FALSE;
  EGLint texTarget = EGL_NO_TEXTURE;
  EGLint texFormat = EGL_NO_TEXTURE;
  EGLint bGenMipMaps = EGL_FALSE;

  if (pAttrib_list) {
    while (EGL_NONE != *pAttrib_list) {
      EGLint name = *pAttrib_list++;
      EGLint value = *pAttrib_list++;

      switch (name) {
      case EGL_WIDTH:
        width = value;
        break;

      case EGL_HEIGHT:
        height = value;
        break;

      case EGL_LARGEST_PBUFFER:
        largestPBuffer = value;
        break;

      case EGL_TEXTURE_TARGET:
        texTarget = value;
        break;

      case EGL_TEXTURE_FORMAT:
        texFormat = value;
        break;

      case EGL_MIPMAP_TEXTURE:
        bGenMipMaps = value;
        break;

      default:
        __eglError(EGL_BAD_ATTRIBUTE, "Invalid pixel buffer attribute: %d\r\n",
                   name);
        return EGL_NO_SURFACE;
      }
    }
  }

  // Create a window surface
  _EGLSurface *pSurface;
  err = _EGLSurface::CreatePBF(&pSurface, pDisplay, EGL_PBUFFER_BIT, pConfig,
                               width, height, largestPBuffer, texTarget,
                               texFormat, bGenMipMaps);
  if (__eglFailed(err)) {
    __eglError(err, "_EGLSurface::Create() failed, err = %d.\r\n", err);
    return EGL_NO_SURFACE;
  }

  // Create the new surface handle
  uint32_t handle;
  err = pDisplay->registerObject(&handle, pSurface, HANDLE_SURFACE);
  if (__eglFailed(err)) {
    __eglError(err, "EGLDriver::registerObject() failed, err = %d.\r\n", err);
    __safeRelease(pSurface);
    return EGL_NO_SURFACE;
  }

  return reinterpret_cast<EGLSurface>(handle);
}

EGLAPI EGLBoolean EGLAPIENTRY eglDestroySurface(EGLDisplay display,
                                                EGLSurface surface) {
  __profileAPI(" - %s( display=%d, surface=%d )\n", __FUNCTION__, display,
               surface);

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Remove the surface object from the handle table
  auto pSurface = pDisplay->unregisterObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, "Invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  // Delete the surface
  pSurface->release();

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglQuerySurface(EGLDisplay display,
                                              EGLSurface surface,
                                              EGLint attribute,
                                              EGLint *pValue) {
  __profileAPI(" - %s( display=%d, surface=%d, attribute=%s, pValue=0x%p )\n",
               __FUNCTION__, display, surface,
               SurfaceAttributeToString(attribute), pValue);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = pDisplay->getObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, "Invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  // Query the surface attributes
  err = pSurface->getAttribute(pValue, attribute);
  if (__eglFailed(err)) {
    __eglError(err, "Surface::getAttribute() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglSurfaceAttrib(EGLDisplay display,
                                               EGLSurface surface,
                                               EGLint attribute, EGLint value) {
  __profileAPI(" - %s( display=%d, surface=%d, attribute=%d, value=%d )\n",
               __FUNCTION__, display, surface,
               SurfaceAttributeToString(attribute), value);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY,
               "eglSurfaceAttrib() failed, invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(
        EGL_NOT_INITIALIZED,
        "eglSurfaceAttrib() failed, the display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = pDisplay->getObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE,
               "eglSurfaceAttrib() failed, invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  err = pSurface->setAttribute(attribute, value);
  if (__eglFailed(err)) {
    __eglError(err, "Surface::setAttribute() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglBindTexImage(EGLDisplay display,
                                              EGLSurface surface,
                                              EGLint buffer) {
  __profileAPI(" - %s( display=%d, surface=%d, buffer=%s )\n", __FUNCTION__,
               display, surface, BufferToString(buffer));

  EGLint err;

  // Validate the buffer
  if (EGL_BACK_BUFFER != buffer) {
    __eglError(EGL_BAD_PARAMETER,
               "eglBindTexImage() failed, invalid buffer parameter: %d.\r\n",
               buffer);
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY,
               "eglBindTexImage() failed, invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(
        EGL_NOT_INITIALIZED,
        "eglBindTexImage() failed, the display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = pDisplay->getObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE,
               "eglBindTexImage() failed, invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  if (surface == eglGetCurrentSurface(EGL_DRAW)) {
    // Perform an implicit GL flush
    glFlush();
  }

  // Bind the surface to the current texture
  err = pSurface->bindTexture();
  if (__eglFailed(err)) {
    __eglLogError("Surface::bindTexture() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglReleaseTexImage(EGLDisplay display,
                                                 EGLSurface surface,
                                                 EGLint buffer) {
  __profileAPI(" - %s( display=%d, surface=%d, buffer=%s )\n", __FUNCTION__,
               display, surface, BufferToString(buffer));

  EGLint err;

  // Validate the buffer
  if (EGL_BACK_BUFFER != buffer) {
    __eglError(EGL_BAD_PARAMETER,
               "eglBindTexImage() failed, invalid buffer parameter: %d.\r\n",
               buffer);
    return EGL_FALSE;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = pDisplay->getObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, "Invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  // Release the bound texture
  err = pSurface->releaseTexBound();
  if (__eglFailed(err)) {
    __eglError(err, "Surface::releaseTexBound() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglSwapInterval(EGLDisplay display,
                                              EGLint interval) {
  __profileAPI(" - %s( display=%d, interval=%d )\n", __FUNCTION__, display,
               interval);
  __unused(interval);

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY,
               "eglSwapInterval() failed, invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(
        EGL_NOT_INITIALIZED,
        "eglSwapInterval() failed, the display was not initialized.\r\n");
    return EGL_FALSE;
  }

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    auto pSurface = pContext->getDrawSurface();
    if (pSurface) {
      // We only support a fixed swap interval value of 1
      // Do nothing
      return EGL_TRUE;
    } else {
      __eglError(EGL_BAD_SURFACE,
                 "eglSwapInterval() failed, there is no surface bound to "
                 "the current context.\r\n");
      return EGL_FALSE;
    }
  } else {
    __eglError(EGL_BAD_CONTEXT,
               "eglSwapInterval() failed, there is no current context on "
               "the calling thread.\r\n");
    return EGL_FALSE;
  }
}

EGLAPI EGLContext EGLAPIENTRY eglCreateContext(EGLDisplay display,
                                               EGLConfig config,
                                               EGLContext share_context,
                                               const EGLint *pAttrib_list) {
  __profileAPI(" - %s( display=%d, config=%d, share_context=%d, "
               "pAttrib_list=0x%p )\n",
               __FUNCTION__, display, config, share_context, pAttrib_list);

  EGLint err;

  // Verify that the attribute list is nullptr or empty
  if (pAttrib_list && (*pAttrib_list != EGL_NONE)) {
    __eglError(EGL_BAD_ATTRIBUTE,
               "pAttrib_list should be nullptr or empty.\r\n");
    return EGL_NO_CONTEXT;
  }

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_NO_CONTEXT;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_NO_CONTEXT;
  }

  // Find the config object from the handle table
  auto pConfig = pDisplay->getObject<_EGLConfig *>(config);
  if (nullptr == pConfig) {
    __eglError(EGL_BAD_CONFIG, "Invalid config handle.\r\n");
    return EGL_NO_CONTEXT;
  }

  _EGLContext *pCtxShared = nullptr;

  if (EGL_NO_CONTEXT != share_context) {
    pCtxShared = pDisplay->getObject<_EGLContext *>(share_context);
    if (nullptr == pCtxShared) {
      __eglError(EGL_BAD_CONTEXT, "Invalid shared context handle.\r\n");
      return EGL_NO_CONTEXT;
    }
  }

  _EGLContext *pContext;
  err = _EGLContext::Create(&pContext, pDisplay, pConfig, pCtxShared);
  if (__eglFailed(err)) {
    __eglError(err, "_EGLContext::Create() failed, err = %d.\r\n", err);
    return EGL_NO_CONTEXT;
  }

  uint32_t handle;
  err = pDisplay->registerObject(&handle, pContext, HANDLE_CONTEXT);
  if (__eglFailed(err)) {
    __eglError(err, "_EGLDriver::registerObject() failed, err = %d.\r\n", err);
    __safeRelease(pContext);
    return EGL_NO_CONTEXT;
  }

  return reinterpret_cast<EGLContext>(handle);
}

EGLAPI EGLBoolean EGLAPIENTRY eglDestroyContext(EGLDisplay display,
                                                EGLContext context) {
  __profileAPI(" - %s( display=%d, context=%d )\n", __FUNCTION__, display,
               context);

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Remove the context object from the handle table
  auto pContext = pDisplay->unregisterObject<_EGLContext *>(context);
  if (nullptr == pContext) {
    __eglError(EGL_BAD_CONTEXT, "Invalid context handle.\r\n");
    return EGL_FALSE;
  }

  // Delete the context
  pContext->release();

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglMakeCurrent(EGLDisplay display,
                                             EGLSurface draw, EGLSurface read,
                                             EGLContext context) {
  EGLint err;

  __profileAPI(" - %s( display=%d, draw=%d, read=%d, context=%d )\n",
               __FUNCTION__, display, draw, read, context);

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  _EGLSurface *pSurfDraw = nullptr;
  if (draw != EGL_NO_SURFACE) {
    // Find the draw surface object from the handle table
    pSurfDraw = pDisplay->getObject<_EGLSurface *>(draw);
    if (nullptr == pSurfDraw) {
      __eglError(EGL_BAD_SURFACE, "Invalid draw surface handle.\r\n");
      return EGL_FALSE;
    }
  }

  _EGLSurface *pSurfRead = nullptr;
  if (read != EGL_NO_SURFACE) {
    // Find the read surface object from the handle table
    pSurfRead = pDisplay->getObject<_EGLSurface *>(read);
    if (nullptr == pSurfRead) {
      __eglError(EGL_BAD_SURFACE, "Invalid read surface handle.\r\n");
      return EGL_FALSE;
    }
  }

  _EGLContext *pContext = nullptr;
  if (context != EGL_NO_CONTEXT) {
    // Find the context object from the handle table
    pContext = pDisplay->getObject<_EGLContext *>(context);
    if (nullptr == pContext) {
      __eglError(EGL_BAD_SURFACE, "Invalid context handle.\r\n");
      return EGL_FALSE;
    }
  }

  auto curThreadID = std::this_thread::get_id();

  if (pContext) {
    if (pContext->hasBindings() && pContext->getThreadID() != curThreadID) {
      __eglError(EGL_BAD_ACCESS,
                 "The context is current to some other thread.\r\n");
      return EGL_FALSE;
    }

    if ((nullptr == pSurfDraw) || (nullptr == pSurfRead)) {
      __eglError(EGL_BAD_MATCH,
                 "The context and surface handles are not compatible.\r\n");
      return EGL_FALSE;
    }

    if (pSurfDraw->getConfig() != pContext->getConfig()) {
      __eglError(
          EGL_BAD_MATCH,
          "The context and draw surface configurations do not match.\r\n");
      return EGL_FALSE;
    }

    if (pSurfRead->getConfig() != pContext->getConfig()) {
      __eglError(
          EGL_BAD_MATCH,
          "The context and read surface configurations do not match.\r\n");
      return EGL_FALSE;
    }    
  } else {
    if (pSurfDraw || pSurfRead) {
      __eglError(EGL_BAD_MATCH,
                 "The context and surface handles are not compatible.\r\n");
      return EGL_FALSE;
    }
  }

  // Set the current EGL context
  err = g_driver.makeCurrent(pContext, curThreadID, pSurfDraw, pSurfRead);
  if (__eglFailed(err)) {
    __eglError(err, "EGLDriver::makeCurrent() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLContext EGLAPIENTRY eglGetCurrentContext() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    auto pDisplay = pContext->getDisplay();
    return reinterpret_cast<EGLContext>(pDisplay->getHandle(pContext));
  }

  return EGL_NO_CONTEXT;
}

EGLAPI EGLSurface EGLAPIENTRY eglGetCurrentSurface(EGLint readdraw) {
  __profileAPI(" - %s( readdraw=%s )\n", __FUNCTION__,
               SurfaceTypeToString(readdraw));

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    auto pDisplay = pContext->getDisplay();
    switch (readdraw) {
    case EGL_DRAW:
      return reinterpret_cast<EGLSurface>(pDisplay->getHandle(
          pContext->getDrawSurface()));

    case EGL_READ:
      return reinterpret_cast<EGLSurface>(pDisplay->getHandle(
          pContext->getReadSurface()));
    }
  }

  return EGL_NO_SURFACE;
}

EGLAPI EGLDisplay EGLAPIENTRY eglGetCurrentDisplay() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    return reinterpret_cast<EGLDisplay>(
        g_driver.getHandle(pContext->getDisplay()));
  }

  return EGL_NO_DISPLAY;
}

EGLAPI EGLBoolean EGLAPIENTRY eglQueryContext(EGLDisplay display,
                                              EGLContext context,
                                              EGLint attribute,
                                              EGLint *pValue) {
  __profileAPI(" - %s( display=%d, context=%d, attribute=%d, pValue=0x%p )\n",
               __FUNCTION__, context, attribute, pValue);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the context object from the handle table
  auto pContext = pDisplay->getObject<_EGLContext *>(context);
  if (nullptr == pContext) {
    __eglError(EGL_BAD_CONTEXT, "Invalid context handle.\r\n");
    return EGL_FALSE;
  }

  // Query the context attributes
  err = pContext->getAttribute(attribute, pValue);
  if (__eglFailed(err)) {
    __eglError(err, "Context::getAttribute() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglWaitGL() {
  __profileAPI(" - %s()\n", __FUNCTION__);

  glFinish();

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglWaitNative(EGLint engine) {
  __profileAPI(" - %s( engine=%s )\n", __FUNCTION__, EngineToString(engine));

  if (EGL_CORE_NATIVE_ENGINE != engine) {
    __eglError(EGL_BAD_PARAMETER,
               "eglWaitNative() failed, invalid engine parameter: %d.\r\n",
               engine);
    return EGL_FALSE;
  }

  // Validate the current context draw surface
  auto pContext = g_driver.getCurrentContext();
  if (pContext) {
    auto pSurface = pContext->getDrawSurface();
    if (!pSurface) {
      __eglError(EGL_BAD_CURRENT_SURFACE,
                 "eglWaitNative() failed, render surface not valid.\r\n");
      return EGL_FALSE;
    }
  }

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay display,
                                             EGLSurface surface) {
  __profileAPI(" - %s( display=%d, surface=%d )\n", __FUNCTION__, display,
               surface);

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = pDisplay->getObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, "Invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  // Only operate on window surfaces
  if (EGL_WINDOW_BIT != pSurface->getType()) {
    __eglError(EGL_BAD_SURFACE, "Not a window rendering surface.\r\n");
    return EGL_FALSE;
  }

  // Perform implicit glFlush
  glFlush();

  // Blit the surface to the display
  pSurface->present();

#ifdef COCOGL_SNAPSHOTS
  static int s_fileno = 0;
  char filePath[360];
  if (s_fileno++ < COCOGL_SNAPSHOTS) {
    snprintf(filePath, 360, "snapshot%d.png", s_fileno);
    pSurface->saveImage(filePath);
  }
#endif

  return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY eglCopyBuffers(EGLDisplay display,
                                             EGLSurface surface,
                                             EGLNativePixmapType target) {
  __profileAPI(" - %s( display=%d, surface=%d, target=0x%x )\n", __FUNCTION__,
               display, surface, target);

  EGLint err;

  // Find the display object from the handle table
  auto pDisplay = g_driver.getObject<_EGLDisplay *>(display);
  if (nullptr == pDisplay) {
    __eglError(EGL_BAD_DISPLAY, "Invalid display handle.\r\n");
    return EGL_FALSE;
  }

  // Verify that the display was initialized
  if (!pDisplay->isInitialized()) {
    __eglError(EGL_NOT_INITIALIZED, "The display was not initialized.\r\n");
    return EGL_FALSE;
  }

  // Find the surface object from the handle table
  auto pSurface = pDisplay->getObject<_EGLSurface *>(surface);
  if (nullptr == pSurface) {
    __eglError(EGL_BAD_SURFACE, "Invalid surface handle.\r\n");
    return EGL_FALSE;
  }

  // Copy the surface bits
  err = pSurface->copyBuffer(target);
  if (__eglFailed(err)) {
    __eglError(err, "Context::getAttribute() failed, err = %d.\r\n", err);
    return EGL_FALSE;
  }

  // Perform implicit glFlush before returning
  glFlush();

  return EGL_TRUE;
}
