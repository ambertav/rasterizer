#include "rasterizer.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>

Rasterizer::Rasterizer(size_t w, size_t h) : fb(w, h) {}

const FrameBuffer* Rasterizer::render(const Scene& scene) noexcept {
  clear();

  float aspect_ratio{static_cast<float>(fb.get_width()) /
                     static_cast<float>(fb.get_height())};

  vec::Mat4 view{scene.camera.get_view_matrix()};
  vec::Mat4 projection{scene.camera.get_projection_matrix(aspect_ratio)};
  vec::Mat4 view_projection{projection * view};

  // viewport transform
  auto to_screen = [&](const vec::Vec4& ndc) -> vec::Vec2 {
    int x{static_cast<int>((ndc.x() + 1.0f) * 0.5f * fb.get_width())};
    int y{static_cast<int>((1.0f - ndc.y()) * 0.5f * fb.get_height())};

    // clamp to framebuffer boundaries
    x = std::clamp(x, 0, static_cast<int>(fb.get_width()) - 1);
    y = std::clamp(y, 0, static_cast<int>(fb.get_height()) - 1);

    return {x, y};
  };

  for (const auto& mesh : scene.meshes) {
    for (const auto& triangle : mesh.triangles) {
      const vec::Vec3& v0{mesh.vertices[triangle.i0].position};
      const vec::Vec3& v1{mesh.vertices[triangle.i1].position};
      const vec::Vec3& v2{mesh.vertices[triangle.i2].position};

      // transform into clip space
      vec::Vec4 c0{view_projection * vec::Vec4{v0.x(), v0.y(), v0.z(), 1.0f}};
      vec::Vec4 c1{view_projection * vec::Vec4{v1.x(), v1.y(), v1.z(), 1.0f}};
      vec::Vec4 c2{view_projection * vec::Vec4{v2.x(), v2.y(), v2.z(), 1.0f}};

      if (c0.w() <= 0.0f || c1.w() <= 0.0f || c2.w() <= 0.0f) {
        continue;
      }

      // NDC
      vec::Vec4 ndc0{c0 / c0.w()};
      vec::Vec4 ndc1{c1 / c1.w()};
      vec::Vec4 ndc2{c2 / c2.w()};

      draw_triangle(to_screen(ndc0), to_screen(ndc1), to_screen(ndc2));
    }
  }

  return &fb;
}

void Rasterizer::draw_line(vec::Vec2 p0, vec::Vec2 p1,
                           uint32_t color) noexcept {
  if (p0 == p1) {
    fb.set_pixel(p0.x(), p0.y(), color);
    return;
  }

  bool is_steep{std::abs(p1.y() - p0.y()) > std::abs(p1.x() - p0.x())};

  if (is_steep) {
    std::swap(p0.x(), p0.y());
    std::swap(p1.x(), p1.y());
  }

  if (p0.x() > p1.x()) {
    std::swap(p0, p1);
  }

  int dx{std::abs(p1.x() - p0.x())};
  int dy{std::abs(p1.y() - p0.y())};

  int p{2 * dy - dx};
  int y_step{p0.y() < p1.y() ? 1 : -1};
  int y{p0.y()};

  for (int x{p0.x()}; x <= p1.x(); ++x) {
    if (is_steep) {
      fb.set_pixel(y, x, color);
    } else {
      fb.set_pixel(x, y, color);
    }

    if (p > 0) {
      y += y_step;
      p -= 2 * dx;
    }
    p += 2 * dy;
  }
}

void Rasterizer::draw_triangle(vec::Vec2 p0, vec::Vec2 p1, vec::Vec2 p2,
                               uint32_t color) noexcept {
  draw_line(p0, p1, color);
  draw_line(p1, p2, color);
  draw_line(p2, p0, color);
}

void Rasterizer::fill(uint32_t color) noexcept { fb.fill(color); }

void Rasterizer::clear() noexcept { fb.clear(); }