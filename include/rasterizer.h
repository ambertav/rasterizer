#pragma once

#include <array>
#include <cstdint>
#include <vector>

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

  void fill(uint32_t color) noexcept;
  void clear() noexcept;

 private:
  struct ClipVertex {
    vec::Vec4 position;
    vec::Vec3 normal;
    vec::Vec2f uv;
  };

  struct RasterVertex {
    vec::Vec2 position;
    vec::Vec3 normal;
    vec::Vec2f uv;
    float depth;
  };

  static ClipVertex lerp_clip_vertex(const ClipVertex& a, const ClipVertex& b,
                                     float t);
  static std::vector<ClipVertex> clip_near(const std::array<ClipVertex, 3>& clip_vertices);

  void draw_line(vec::Vec2 p0, vec::Vec2 p1,
                 uint32_t color = colors::WHITE) noexcept;

  void draw_triangle(vec::Vec2 p0, vec::Vec2 p1, vec::Vec2 p2,
                     uint32_t color = colors::WHITE) noexcept;
  void fill_triangle(const RasterVertex& v0, const RasterVertex& v1,
                     const RasterVertex& v2,
                     uint32_t color = colors::WHITE) noexcept;

  FrameBuffer fb;
};