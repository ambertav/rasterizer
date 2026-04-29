#include <SDL.h>

#include <iostream>
#include <string>

#include "colors.h"
#include "frame_buffer.h"

constexpr int DEFAULT_WIDTH{800};
constexpr int DEFAULT_HEIGHT{600};

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

  FrameBuffer fb{static_cast<size_t>(config.width),
                 static_cast<size_t>(config.height)};
  fb.fill(colors::RED);

  SDL_Event event;
  bool running{true};

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        std::cout << "quitting...\n";
        running = false;
      }
    }

    SDL_UpdateTexture(texture, nullptr, fb.data(),
                      fb.get_width() * sizeof(uint32_t));
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