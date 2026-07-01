#pragma once

#include <vector>

#include "vec.h"

struct Vertex {
  vec::Vec3 position;
  vec::Vec3 normal;
  vec::Vec2f uv;

  Vertex() = default;
  Vertex(vec::Vec3 p, vec::Vec3 n, vec::Vec2f u)
      : position(p), normal(n), uv(u) {}
};

struct Triangle {
  uint32_t i0, i1, i2;

  Triangle() = default;
  Triangle(uint32_t zero, uint32_t one, uint32_t two)
      : i0(zero), i1(one), i2(two) {}
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<Triangle> triangles;
};