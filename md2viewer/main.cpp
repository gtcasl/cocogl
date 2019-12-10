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

//--
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

//--
#include "renderer.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int width = SCREEN_WIDTH;
int height = SCREEN_HEIGHT;
char* background = nullptr;
char* model = nullptr;

static void parse_args(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "w:h:b:?")) != -1) {
    switch (c) {
    case 'w':
      width = atoi(optarg);
      break;
    case 'h':
      height = atoi(optarg);
      break;
    case 'b':
      background = optarg;
      break;
    case '?':
      printf("MD2Viewer Demo.\n");
      printf("Usage: filename [-w width][-h height][-b background]\n");
      [[fallthrough]];
    default:
      exit(1);
    }
  }
  //--
  if (optind < argc) {
    model = argv[optind++];
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

  // create renderer
  auto renderer = new Renderer(window);
  if (!renderer->OnInitialize(model, background)) {
    exit(-1);
  }

  uint64_t num_frames = 0;
  float FPS = 0.0f;

  while (loop) {
    auto start_time = std::chrono::high_resolution_clock::now();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          renderer->OnKeyPrev();
          break;
        case SDLK_RIGHT:
          renderer->OnKeyNext();
          break;
        }
        break;
      case SDL_QUIT: {
        loop = 0;
        break;
      }
      }
    }

    renderer->OnRender();

    renderer->Present();

    ++num_frames;
    if (0 == (num_frames % 16)) {
      auto end_time = std::chrono::high_resolution_clock::now();
      auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
      FPS = 16 / elapsed_time.count();

      char text[64];
      snprintf(text, 64, "MD2Viewer: %s - %.2f fps", renderer->getAnimation(), FPS);
      SDL_SetWindowTitle(glesWindow, text);
    }
  }

  std::cout << "FPS: " << FPS << std::endl;
  std::cout << "Total frames: " << num_frames << std::endl;

  // Cleaning
  renderer->OnDestroy();
  delete renderer;

  SDL_DestroyWindow(glesWindow);

  return 0;
}