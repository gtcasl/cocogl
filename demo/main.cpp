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
#include "debug_t.hpp"
#include "fog_t.hpp"
#include "line_t.hpp"
#include "quad_t.hpp"
#include "scene_t.hpp"
#include "stencil_t.hpp"
#include "texture_t.hpp"
#include "triangle_t.hpp"

int testid = 2;
int width = 640;
int height = 480;

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
      [[fallthrough]];
    default:
      exit(1);
    }
  }
}

static Renderer* create_test(int tid, EGLNativeWindowType window) {
  Renderer *test = nullptr;
  switch (tid) {
  case 0:
    test = new ClearTest();
    break;
  case 1:
    test = new LineTest();
    break;
  case 2:
    test = new TriangleTest();
    break;
  case 3:
    test = new CubeTest();
    break;
  case 4:
    test = new TextureTest();
    break;
  case 5:
    test = new SceneTest();
    break;
  case 6:
    test = new FogTest();
    break;
  case 7:
    test = new StencilTest();
    break;
  case 8:
    test = new QuadTest();
    break;
  case 9:
    test = new DebugTest();
    break;
  default:
    std::cout << "invalid testid=" << testid << std::endl;
    exit(-1);
  }
  if (!test->OnInitialize(window)) {
    std::cout << "test initiaklization failed" << std::endl;
    delete test;
    exit(1);
  }
  return test;
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
  auto test = create_test(testid, window);

  auto start_time = std::chrono::high_resolution_clock::now();
  uint64_t num_frames = 0;

  while (loop) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_PAGEUP:          
          if (testid > 0) {
            test->OnDestroy();
            delete test;
            test = create_test(--testid, window);
          }
          break;
        case SDLK_PAGEDOWN:
          if (testid < 9) {
            test->OnDestroy();
            delete test;
            test = create_test(++testid, window);
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
    ++num_frames;    
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(
      end_time - start_time);
  auto FPS = num_frames / elapsed_time.count();
  std::cout << "FPS: " << FPS << std::endl;

  // Cleaning
  test->OnDestroy();  
  delete test;

  SDL_DestroyWindow(glesWindow);

  return 0;
}