#pragma once

#include <cstdint>

#include "frame_buffer.h"
#include "vec.h"

class Rasterizer {
 public:
  Rasterizer(size_t w, size_t h);

  ~Rasterizer() = default;
  Rasterizer(const Rasterizer&) = default;
  Rasterizer& operator=(const Rasterizer&) = default;
  Rasterizer(Rasterizer&&) = default;
  Rasterizer& operator=(Rasterizer&&) = default;

  const FrameBuffer* get_frame_buffer() const noexcept;

  void draw_line(vec::Vec2 p0, vec::Vec2 p1, uint32_t color) noexcept;
  void fill(uint32_t color) noexcept;
  void clear() noexcept;

 private:
  FrameBuffer fb;
};