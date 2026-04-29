#pragma once

#include <cstdint>
#include <vector>

class FrameBuffer {
 public:
  FrameBuffer(size_t w, size_t h);

  ~FrameBuffer() = default;
  FrameBuffer(const FrameBuffer&) = default;
  FrameBuffer& operator=(const FrameBuffer&) = default;
  FrameBuffer(FrameBuffer&&) = default;
  FrameBuffer& operator=(FrameBuffer&&) = default;

  uint32_t get_pixel(size_t x, size_t y) const;
  size_t get_width() const noexcept;
  size_t get_height() const noexcept;

  void set_pixel(size_t x, size_t y, uint32_t color);

  void fill(uint32_t color) noexcept;
  void clear() noexcept;

  const uint32_t* data() const noexcept;

 private:
  size_t index(size_t x, size_t y) const;

  std::vector<uint32_t> pixels;
  size_t width;
  size_t height;
};