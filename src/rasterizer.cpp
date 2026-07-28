#include "rasterizer.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>

namespace {
constexpr float K_NEAR_EPSILON{1e-5f};
float edge_function(const vec::Vec2& a, const vec::Vec2& b, float px,
                    float py) {
  return (px - a.x()) * static_cast<float>(b.y() - a.y()) -
         (py - a.y()) * static_cast<float>(b.x() - a.x());
}

}  // namespace

Rasterizer::Rasterizer(size_t w, size_t h) : fb(w, h) {}

const FrameBuffer* Rasterizer::render(const Scene& scene) noexcept {
  clear();

  float aspect_ratio{static_cast<float>(fb.get_width()) /
                     static_cast<float>(fb.get_height())};

  vec::Mat4 view{scene.camera.get_view_matrix()};
  vec::Mat4 projection{scene.camera.get_projection_matrix(aspect_ratio)};
  vec::Mat4 view_projection{projection * view};

  // transform into clip space
  auto to_clip = [&](const Vertex& vertex) -> ClipVertex {
    return ClipVertex{
        view_projection * vec::Vec4{vertex.position.x(), vertex.position.y(),
                                    vertex.position.z(), 1.0f},
        vertex.normal, vertex.uv};
  };

  // viewport transform
  auto to_screen = [&](const ClipVertex& clip_vertex) -> RasterVertex {
    vec::Vec4 ndc{clip_vertex.position / clip_vertex.position.w()};

    int x{static_cast<int>((ndc.x() + 1.0f) * 0.5f * fb.get_width())};
    int y{static_cast<int>((1.0f - ndc.y()) * 0.5f * fb.get_height())};

    // // clamp to framebuffer boundaries
    // x = std::clamp(x, 0, static_cast<int>(fb.get_width()) - 1);
    // y = std::clamp(y, 0, static_cast<int>(fb.get_height()) - 1);

    return RasterVertex{vec::Vec2{x, y}, clip_vertex.normal, clip_vertex.uv,
                        ndc.z()};
  };

  for (const auto& mesh : scene.meshes) {
    for (const auto& triangle : mesh.triangles) {
      std::array<ClipVertex, 3> clip_vertices{
          to_clip(mesh.vertices[triangle.i0]),
          to_clip(mesh.vertices[triangle.i1]),
          to_clip(mesh.vertices[triangle.i2]),
      };

      std::vector<ClipVertex> clipped{clip_near(clip_vertices)};
      if (clipped.size() < 3) {
        continue;
      }

      for (size_t i{1}; i + 1 < clipped.size(); ++i) {
        RasterVertex rv0{to_screen(clipped[0])};
        RasterVertex rv1{to_screen(clipped[i])};
        RasterVertex rv2{to_screen(clipped[i + 1])};

        fill_triangle(rv0, rv1, rv2);
      }
    }
  }

  return &fb;
}

void Rasterizer::fill(uint32_t color) noexcept { fb.fill(color); }

void Rasterizer::clear() noexcept { fb.clear(); }

Rasterizer::ClipVertex Rasterizer::lerp_clip_vertex(const ClipVertex& a,
                                                    const ClipVertex& b,
                                                    float t) {
  ClipVertex cv{};
  for (size_t i{}; i < 4; ++i) {
    cv.position[i] = a.position[i] + (t * (b.position[i] - a.position[i]));
  }

  for (size_t i{}; i < 3; ++i) {
    cv.normal[i] = a.normal[i] + (t * (b.normal[i] - a.normal[i]));
  }

  for (size_t i{}; i < 2; ++i) {
    cv.uv[i] = a.uv[i] + (t * (b.uv[i] - a.uv[i]));
  }

  return cv;
}

std::vector<Rasterizer::ClipVertex> Rasterizer::clip_near(
    const std::array<ClipVertex, 3>& clip_vertices) {
  std::vector<ClipVertex> clipped{};
  clipped.reserve(4);

  for (size_t i{}; i < 3; ++i) {
    const auto& current{clip_vertices[i]};
    const auto& previous{clip_vertices[(i + 2) % 3]};

    bool current_inside{current.position.w() > K_NEAR_EPSILON};
    bool previous_inside{previous.position.w() > K_NEAR_EPSILON};

    if (current_inside != previous_inside) {
      float t{(K_NEAR_EPSILON - previous.position.w()) /
              (current.position.w() - previous.position.w())};
      clipped.push_back(lerp_clip_vertex(previous, current, t));
    }

    if (current_inside) {
      clipped.push_back(current);
    }
  }

  return clipped;
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

void Rasterizer::fill_triangle(const RasterVertex& v0, const RasterVertex& v1,
                               const RasterVertex& v2,
                               uint32_t color) noexcept {
  // bounding box clamped to framebuffer
  int minimum_x{std::max(
      0, std::min({v0.position.x(), v1.position.x(), v2.position.x()}))};
  int maximum_x{
      std::min(static_cast<int>(fb.get_width()) - 1,
               std::max({v0.position.x(), v1.position.x(), v2.position.x()}))};
  int minimum_y{std::max(
      0, std::min({v0.position.y(), v1.position.y(), v2.position.y()}))};
  int maximum_y{
      std::min(static_cast<int>(fb.get_height()) - 1,
               std::max({v0.position.y(), v1.position.y(), v2.position.y()}))};

  // area to normalize barycentric weight
  float area{edge_function(v0.position, v1.position,
                           static_cast<float>(v2.position.x()),
                           static_cast<float>(v2.position.y()))};

  if (area == 0.0f) {
    return;
  }

  for (int y{minimum_y}; y <= maximum_y; ++y) {
    for (int x{minimum_x}; x <= maximum_x; ++x) {
      float px{x + 0.5f};
      float py{y + 0.5f};

      float w0{edge_function(v1.position, v2.position, px, py)};
      float w1{edge_function(v2.position, v0.position, px, py)};
      float w2{edge_function(v0.position, v1.position, px, py)};

      bool inside{area > 0.0f ? (w0 >= 0 && w1 >= 0 && w2 >= 0)
                              : (w0 <= 0 && w1 <= 0 && w2 <= 0)};
      if (!inside) {
        continue;
      }

      // normalize to barycentric coordinates
      w0 /= area;
      w1 /= area;
      w2 /= area;

      float z{(w0 * v0.depth) + (w1 * v1.depth) + (w2 * v2.depth)};

      size_t sx{static_cast<size_t>(x)};
      size_t sy{static_cast<size_t>(y)};

      if (z < fb.get_depth(sx, sy)) {
        fb.set_depth(sx, sy, z);
        fb.set_pixel(sx, sy, color);
      }
    }
  }
}