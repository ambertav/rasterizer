#pragma once

#include <cstdint>

#include "colors.h"
#include "frame_buffer.h"
#include "scene.h"
#include "vec.h"

class Rasterizer {
 public:
  Rasterizer(size_t w, size_t h);

  ~Rasterizer() = default;
  Rasterizer(const Rasterizer&) = default;
  Rasterizer& operator=(const Rasterizer&) = default;
  Rasterizer(Rasterizer&&) = default;
  Rasterizer& operator=(Rasterizer&&) = default;

  [[nodiscard]] const FrameBuffer* render(const Scene& scene) noexcept;

  void draw_line(vec::Vec2 p0, vec::Vec2 p1,
                 uint32_t color = colors::WHITE) noexcept;
  void draw_triangle(vec::Vec2 p0, vec::Vec2 p1, vec::Vec2 p2,
                     uint32_t color = colors::WHITE) noexcept;

  void fill(uint32_t color) noexcept;
  void clear() noexcept;

 private:
  FrameBuffer fb;
};