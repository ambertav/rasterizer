#pragma once

#include <numbers>

#include "vec.h"

class Camera {
 public:
  Camera(vec::Vec3 t = {0.0f, 0.0f, 0.0f}, float r = 5.0f, float th = 0.0f,
         float p = std::numbers::pi_v<float> / 2.0f, float fv = 45.0f,
         float n = 0.1f, float fr = 1000.0f);

  ~Camera() = default;
  Camera(const Camera&) = default;
  Camera& operator=(const Camera&) = default;
  Camera(Camera&&) = default;
  Camera& operator=(Camera&&) = default;

  vec::Mat4 get_view_matrix() const noexcept;
  vec::Mat4 get_projection_matrix(float aspect_ratio) const noexcept;

  vec::Vec3 get_position() const noexcept;

 private:
  // spherical coordinates
  vec::Vec3 target;
  float rho;
  float theta;  // horizontal angle
  float phi;    // vertical angle
  float fov;
  float near;
  float far;
};
