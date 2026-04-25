#include <SDL.h>

#include <iostream>

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
    return 1;
  }
  std::cout << "SDL2 works\n";
  SDL_Quit();
  return 0;
}