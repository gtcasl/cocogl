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
#include "stdafx.h"
#include <unistd.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// Tests
#include "clear_t.hpp"
#include "cube_t.hpp"
#include "fog_t.hpp"
#include "line_t.hpp"
#include "quad_t.hpp"
#include "scene_t.hpp"
#include "stencil_t.hpp"
#include "texture_t.hpp"
#include "triangle_t.hpp"
#include "text_t.hpp"

int testid = 3;
int width = 800;
int height = 600;

static void parse_args(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "w:h:t:?")) != -1) {
    switch (c) {
    case 'w':
      width = atoi(optarg);
      break;
    case 'h':
      height = atoi(optarg);
      break;
    case 't':
      testid = atoi(optarg);
      break;
    case '?':
      printf("CocoGL Demo.\n");
      printf("Usage: [-t: testno] [-h: help]\n");
      printf("Press keys 'pageup', 'pagedown' to change test.\n");
      [[fallthrough]];
    default:
      exit(1);
    }
  }
}

Renderer *test = nullptr;

void cleanup() {
  if (test) {
    test->OnDestroy();
    delete test;
    test = nullptr;
  }
}

static void create_test(int tid, EGLNativeWindowType window) {
  if (test) {
    cleanup();
  }

  switch (tid) {
  case 0:
    test = new ClearTest(window);
    break;
  case 1:
    test = new LineTest(window);
    break;
  case 2:
    test = new TriangleTest(window);
    break;
  case 3:
    test = new CubeTest(window);
    break;
  case 4:
    test = new TextureTest(window);
    break;
  case 5:
    test = new SceneTest(window);
    break;
  case 6:
    test = new FogTest(window);
    break;
  case 7:
    test = new StencilTest(window);
    break;
  case 8:
    test = new QuadTest(window);
    break;
  case 9:
    test = new TextTest(window);
    break;
  default:
    std::cout << "invalid testid=" << testid << std::endl;
    exit(-1);
  }

  if (!test->OnInitialize()) {
    cleanup();
    exit(-1);
  }
}

int main(int argc, char **argv) {
  //--
  parse_args(argc, argv);

  int loop = 1;
  SDL_Event event;

  auto glesWindow = SDL_CreateWindow("CocoGL Demo", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, width, height, 0);

  SDL_SysWMinfo sysInfo;
  SDL_VERSION(&sysInfo.version);
  SDL_GetWindowWMInfo(glesWindow, &sysInfo);
  auto window = static_cast<EGLNativeWindowType>(sysInfo.info.x11.window);

  // create test
  create_test(testid, window);

  uint64_t num_frames = 0;
  float FPS = 0.0f;

  while (loop) {
    auto start_time = std::chrono::high_resolution_clock::now();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          if (testid > 0) {
            create_test(--testid, window);
          }
          break;
        case SDLK_DOWN:
          if (testid < 9) {
            create_test(++testid, window);
          }
          break;
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

    test->Present();

    ++num_frames;
    if (0 == (num_frames % 16)) {
      auto end_time = std::chrono::high_resolution_clock::now();
      auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
      FPS = 16 / elapsed_time.count();

      char text[64];
      snprintf(text, 64, "CocoGL Demo: [%d] %.2f fps", testid, FPS);
      SDL_SetWindowTitle(glesWindow, text);
    }
  }

  std::cout << "FPS: " << FPS << std::endl;
  std::cout << "Total frames: " << num_frames << std::endl;

  // Cleaning
  cleanup();

  SDL_DestroyWindow(glesWindow);

  return 0;
}