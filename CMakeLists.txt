cmake_minimum_required(VERSION 3.20)
project(rasterizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# =====================================
# SDL2 
# =====================================

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.30.3
  GIT_SHALLOW    TRUE
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST   OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2)

# =====================================
# app executable
# =====================================

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(renderer ${SOURCES})
target_include_directories(renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${sdl2_SOURCE_DIR}/include
)
target_link_libraries(renderer PRIVATE SDL2-static)
set_target_properties(renderer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})