#include <SDL.h>

#include <iostream>
#include <string>

#include "colors.h"
#include "primitives.h"
#include "rasterizer.h"
#include "scene.h"

constexpr int DEFAULT_WIDTH{800};
constexpr int DEFAULT_HEIGHT{600};

constexpr float ORBIT_SENSITIVITY{0.01f};
constexpr float ZOOM_SPEED{0.5f};
constexpr float KEY_ORBIT_SPEED{0.1f};

struct WindowConfiguration {
  int width{DEFAULT_WIDTH};
  int height{DEFAULT_HEIGHT};
};

WindowConfiguration parse_arguments(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  WindowConfiguration config{parse_arguments(argc, argv)};

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
    return -1;
  }
  std::cout << "SDL2 works\n";

  SDL_Window* window{};
  SDL_Renderer* renderer{};

  int rc{SDL_CreateWindowAndRenderer(config.width, config.height, 0, &window,
                                     &renderer)};
  if (rc == -1) {
    std::cerr << "SDL_CreateWindowAndRenderer failed: " << SDL_GetError()
              << '\n';
    SDL_Quit();
    return -1;
  }

  SDL_Texture* texture{SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         config.width, config.height)};
  if (texture == nullptr) {
    std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << '\n';
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  Rasterizer rasterizer{static_cast<size_t>(config.width),
                        static_cast<size_t>(config.height)};
  Scene scene{};
  scene.meshes.push_back(primitives::make_cube());

  bool mouse_held{false};

  SDL_Event event;
  bool running{true};

  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          std::cout << "quitting...\n";
          running = false;
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            mouse_held = true;
          }
          break;

        case SDL_MOUSEBUTTONUP:
          if (event.button.button == SDL_BUTTON_LEFT) {
            mouse_held = false;
          }
          break;

        case SDL_MOUSEMOTION:
          if (mouse_held) {
            scene.camera.orbit(0.0f, event.motion.xrel * ORBIT_SENSITIVITY,
                               event.motion.yrel * ORBIT_SENSITIVITY);
          }
          break;

        case SDL_MOUSEWHEEL:
          scene.camera.orbit(-event.wheel.y * ZOOM_SPEED, 0.0f, 0.0f);
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_LEFT:
              scene.camera.orbit(0.0f, -KEY_ORBIT_SPEED, 0.0f);
              break;
            case SDLK_RIGHT:
              scene.camera.orbit(0.0f, KEY_ORBIT_SPEED, 0.0f);
              break;
            case SDLK_UP:
              scene.camera.orbit(0.0f, 0.0f, -KEY_ORBIT_SPEED);
              break;
            case SDLK_DOWN:
              scene.camera.orbit(0.0f, 0.0f, KEY_ORBIT_SPEED);
              break;
            case SDLK_ESCAPE:
              std::cout << "quitting...\n";
              running = false;
              break;
          }
      }
    }

    const FrameBuffer* fb{rasterizer.render(scene)};

    SDL_UpdateTexture(texture, nullptr, fb->data(),
                      config.width * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

WindowConfiguration parse_arguments(int argc, char* argv[]) {
  WindowConfiguration config{};
  if (argc > 1) {
    for (int i{1}; i < argc; ++i) {
      std::string_view arg{argv[i]};

      if (arg == "--width" || arg == "-w") {
        if (i + 1 >= argc) {
          std::cerr << "--width requires a value\n";
          break;
        }
        try {
          int width = std::stoi(argv[++i]);
          if (width < 1) {
            throw std::out_of_range("width must be positive");
          }
          config.width = width;
        } catch (const std::invalid_argument& e) {
          std::cerr << "invalid width value: " << e.what() << '\n';
        } catch (const std::out_of_range& e) {
          std::cerr << "width value out of range: " << e.what() << '\n';
        }
      }
      if (arg == "--height" || arg == "-h") {
        if (i + 1 >= argc) {
          std::cerr << "--height requires a value\n";
          break;
        }
        try {
          int height = std::stoi(argv[++i]);
          if (height < 1) {
            throw std::out_of_range("height must be positive");
          }
          config.height = height;
        } catch (const std::invalid_argument& e) {
          std::cerr << "invalid height value: " << e.what() << '\n';
        } catch (const std::out_of_range& e) {
          std::cerr << "height value out of range: " << e.what() << '\n';
        }
      }
    }
  }
  return config;
}