#include "stdafx.h"

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// Tests
#include "test.hpp"
#include "clear_t.hpp"
#include "cube_t.hpp"
#include "debug_t.hpp"
#include "fog_t.hpp"
#include "lighting_t.hpp"
#include "line_t.hpp"
#include "quad_t.hpp"
#include "stencil_t.hpp"
#include "texture_t.hpp"
#include "triangle_t.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int testid = 2;

EGLDisplay glDisplay;
EGLConfig glConfig;
EGLContext glContext;
EGLSurface glSurface;

SDL_Window *glesWindow = nullptr;

static void init_GLES(void) {
  EGLint egl_config_attr[] = {
      EGL_BUFFER_SIZE,  32, EGL_DEPTH_SIZE,   16,
      EGL_STENCIL_SIZE, 0,  EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE};

  EGLint numConfigs, majorVersion, minorVersion;
  glesWindow = SDL_CreateWindow("COCGL_DEMO", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_RESIZABLE);
  glDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(glDisplay, &majorVersion, &minorVersion);
  eglChooseConfig(glDisplay, egl_config_attr, &glConfig, 1, &numConfigs);
  SDL_SysWMinfo sysInfo;
  SDL_VERSION(&sysInfo.version); // Set SDL version
  SDL_GetWindowWMInfo(glesWindow, &sysInfo);
  glContext = eglCreateContext(glDisplay, glConfig, EGL_NO_CONTEXT, nullptr);
  glSurface = eglCreateWindowSurface(
      glDisplay, glConfig, (EGLNativeWindowType)sysInfo.info.x11.window,
      0); // X11?
  eglMakeCurrent(glDisplay, glSurface, glSurface, glContext);
}

static void cleanup() {
  eglDestroySurface(glDisplay, glSurface);
  eglDestroyContext(glDisplay, glContext);
  eglTerminate(glDisplay);
  SDL_DestroyWindow(glesWindow);
}

static void parse_args(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "t:h?")) != -1) {
    switch (c) {
    case 't':
      testid = atoi(optarg);
      break;
    case '?':
    case 'h':
      printf("CocoGL Demo.\n");
      printf("Usage: [-t: testid] [-h: help]\n");
      [[fallthrough]];
    default:
      exit(1);
    }
  }
}

int main(int argc, char **argv) {
  //--
  parse_args(argc, argv);

  // create test
  CTestBase *test = nullptr;
  switch (testid) {
  case 0:
    test = new CClearTest();
    break;
  case 1:
    test = new CLine_Test();
    break;
  case 2:
    test = new CTriangleTest();
    break;
  case 3:
    test = new CCubeTest();
    break;
  case 4:
    test = new CTextureTest();
    break;
  case 5:
    test = new CLightingTest();
    break;
  case 6:
    test = new CFogTest();
    break;
  case 7:
    test = new CStencilTest();
    break;
  case 8:
    test = new CQuadTest();
    break;
  case 9:
    test = new CDebugTest();
    break;
  default:
    std::cout << "invalid testid=" << testid << std::endl;
    exit(-1);
  }

  int loop = 1;
  SDL_Event event;

  init_GLES();

  if (!test->OnInitialize(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    std::cout << "test initiaklization failed" << std::endl;
    exit(1);
  }

  auto start_time = std::chrono::high_resolution_clock::now();
  uint64_t num_frames = 0;

  while (loop) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          test->OnKeyPrev();
          break;
        case SDLK_RIGHT:
          test->OnKeyNext();
          break;
        }
        break;
      case SDL_QUIT: {
        loop = 0;
        break;
      }
      }
    }

    test->OnRender();
    ++num_frames;
    eglSwapBuffers(glDisplay, glSurface);
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(
      end_time - start_time);
  auto FPS = num_frames / elapsed_time.count();
  std::cout << "FPS: " << FPS << std::endl;

  // Cleaning
  test->OnDestroy();
  delete test;  
  cleanup();

  return 0;
}