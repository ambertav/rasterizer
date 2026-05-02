#include "rasterizer.h"

#include <cmath>
#include <utility>

Rasterizer::Rasterizer(size_t w, size_t h) : fb(w, h) {}

const FrameBuffer* Rasterizer::get_frame_buffer() const noexcept { return &fb; }

void Rasterizer::draw_line(vec::Vec2 p0, vec::Vec2 p1,
                           uint32_t color) noexcept {
  if (p0 == p1) {
    fb.set_pixel(p0.x, p0.y, color);
    return;
  }

  bool is_steep{std::abs(p1.y - p0.y) > std::abs(p1.x - p0.x)};

  if (is_steep) {
    std::swap(p0.x, p0.y);
    std::swap(p1.x, p1.y);
  }

  if (p0.x > p1.x) {
    std::swap(p0, p1);
  }

  int dx{std::abs(p1.x - p0.x)};
  int dy{std::abs(p1.y - p0.y)};

  int error{2 * dy - dx};
  int y_step{p0.y < p1.y ? 1 : -1};
  int y{p0.y};

  for (int x{p0.x}; x <= p1.x; ++x) {
    if (is_steep) {
      fb.set_pixel(y, x, color);
    } else {
      fb.set_pixel(x, y, color);
    }

    if (error > 0) {
      y += y_step;
      error -= 2 * dx;
    }
    error += 2 * dy;
  }
}

void Rasterizer::fill(uint32_t color) noexcept { fb.fill(color); }

void Rasterizer::clear() noexcept { fb.clear(); }