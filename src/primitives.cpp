#include "primitives.h"

#include <numbers>

#include "vec.h"

constexpr float pi{std::numbers::pi_v<float>};

namespace primitives {
Mesh make_cube(float size) {
  Mesh mesh{};
  float h{size / 2.0f};

  auto make_face = [&](vec::Vec3 p0, vec::Vec3 p1, vec::Vec3 p2, vec::Vec3 p3,
                       vec::Vec3 n) {
    uint32_t base{static_cast<uint32_t>(mesh.vertices.size())};

    mesh.vertices.emplace_back(p0, n, vec::Vec2f{0.0f, 0.0f});
    mesh.vertices.emplace_back(p1, n, vec::Vec2f{1.0f, 0.0f});
    mesh.vertices.emplace_back(p2, n, vec::Vec2f{1.0f, 1.0f});
    mesh.vertices.emplace_back(p3, n, vec::Vec2f{0.0f, 1.0f});

    mesh.triangles.emplace_back(base, base + 1, base + 2);
    mesh.triangles.emplace_back(base, base + 2, base + 3);
  };

  // +z
  make_face({-h, -h, h}, {h, -h, h}, {h, h, h}, {-h, h, h}, {0, 0, 1});
  // -z
  make_face({h, -h, -h}, {-h, -h, -h}, {-h, h, -h}, {h, h, -h}, {0, 0, -1});
  // +x
  make_face({h, -h, h}, {h, -h, -h}, {h, h, -h}, {h, h, h}, {1, 0, 0});
  // -x
  make_face({-h, -h, -h}, {-h, -h, h}, {-h, h, h}, {-h, h, -h}, {-1, 0, 0});
  // +y
  make_face({-h, h, h}, {h, h, h}, {h, h, -h}, {-h, h, -h}, {0, 1, 0});
  // -y
  make_face({-h, -h, -h}, {h, -h, -h}, {h, -h, h}, {-h, -h, h}, {0, -1, 0});

  return mesh;
}

Mesh make_sphere(float rho, uint32_t rings, uint32_t sectors) {
  Mesh mesh{};

  for (uint32_t r{}; r <= rings; ++r) {
    float phi{pi * r / rings};
    for (uint32_t s{}; s <= sectors; ++s) {
      float theta{2.0f * pi * s / sectors};

      float x{rho * std::sin(phi) * std::cos(theta)};
      float y{rho * std::sin(phi) * std::sin(theta)};
      float z{rho * std::cos(phi)};

      vec::Vec3 position{x, y, z};
      vec::Vec3 normal{vec::normalize(position)};
      vec::Vec2f uv{theta / (2.0f * pi), phi / pi};

      mesh.vertices.emplace_back(position, normal, uv);
    }
  }

  for (uint32_t r{}; r < rings; ++r) {
    for (uint32_t s{}; s < sectors; ++s) {
      uint32_t i0 = r * (sectors + 1) + s;
      uint32_t i1 = i0 + 1;
      uint32_t i2 = i0 + (sectors + 1);
      uint32_t i3 = i2 + 1;

      mesh.triangles.emplace_back(i0, i2, i1);
      mesh.triangles.emplace_back(i1, i2, i3);
    }
  }

  return mesh;
}
}  // namespace primitives