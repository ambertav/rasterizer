#include "frame_buffer.h"

#include <algorithm>
#include <cassert>
#include <ranges>

#include "colors.h"

FrameBuffer::FrameBuffer(size_t w, size_t h)
    : width(w),
      height(h),
      pixels(w * h, colors::BLACK),
      depth(w * h, std::numeric_limits<float>::max()) {}

void FrameBuffer::set_pixel(size_t x, size_t y, uint32_t color) {
  pixels[index(x, y)] = color;
}

uint32_t FrameBuffer::get_pixel(size_t x, size_t y) const {
  return pixels[index(x, y)];
}

void FrameBuffer::set_depth(size_t x, size_t y, float z) {
  depth[index(x, y)] = z;
}

float FrameBuffer::get_depth(size_t x, size_t y) const {
  return depth[index(x, y)];
}

size_t FrameBuffer::get_width() const noexcept { return width; }

size_t FrameBuffer::get_height() const noexcept { return height; }

void FrameBuffer::fill(uint32_t color) noexcept {
  std::ranges::fill(pixels, color);
  std::ranges::fill(depth, std::numeric_limits<float>::max());
}

void FrameBuffer::clear() noexcept { fill(colors::BLACK); }

const uint32_t* FrameBuffer::data() const noexcept { return pixels.data(); }

size_t FrameBuffer::index(size_t x, size_t y) const {
  assert(x < width && y < height);
  return y * width + x;
}