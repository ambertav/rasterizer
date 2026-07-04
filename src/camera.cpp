#include "camera.h"

#include <algorithm>
#include <cmath>

Camera::Camera(vec::Vec3 t, float r, float th, float p, float fv, float n,
               float fr)
    : target(t), rho(r), theta(th), phi(p), fov(fv), near(n), far(fr) {}

void Camera::orbit(float delta_rho, float delta_theta,
                   float delta_phi) noexcept {
  rho = std::max(0.1f, rho + delta_rho);  // non-negative
  theta += delta_theta;
  phi = std::clamp(phi + delta_phi, 0.01f,
                   std::numbers::pi_v<float> - 0.01f);  // no flip
}

vec::Mat4 Camera::get_view_matrix() const noexcept {
  /*
  look-at construction to transform world space into camera space
  Z-up, right-handed convention

  forward -  direction from target to camera (position - target)
  right - perpendicular to forward and world up (world_up x forward)
  up - perpendicular to forward and right, reorthogonalized accounting for
  camera tilt (forward x right)

  final view matrix is:
    [ right.x   right.y   right.z   -dot(right,   position) ]
    [ up.x      up.y      up.z      -dot(up,       position) ]
    [ forward.x forward.y forward.z -dot(forward,  position) ]
    [ 0         0         0          1                       ]
  */

  vec::Vec3 position{get_position()};
  vec::Vec3 world_up{0.0f, 0.0f, 1.0f};

  vec::Vec3 forward{vec::normalize(position - target)};

  // fallback when camera at pole (forward nearly paralell to Z-up)
  if (std::abs(forward.z()) > 0.999f) {
    world_up = {1.0f, 0.0f, 0.0f};
  }

  vec::Vec3 right{vec::normalize(vec::cross(world_up, forward))};
  vec::Vec3 up{vec::normalize(vec::cross(forward, right))};

  vec::Vec4 axes[4]{
      {right.x(), right.y(), right.z(), -vec::dot(right, position)},
      {up.x(), up.y(), up.z(), -vec::dot(up, position)},
      {forward.x(), forward.y(), forward.z(), -vec::dot(forward, position)},
      {0.0f, 0.0f, 0.0f, 1.0f}};

  vec::Mat4 view_matrix{};
  for (size_t i{}; i < 4; ++i) {
    view_matrix.set_row(i, axes[i]);
  }

  return view_matrix;
}

vec::Mat4 Camera::get_projection_matrix(float aspect_ratio) const noexcept {
  float fov_rad{fov * (std::numbers::pi_v<float> / 180.0f)};
  float t{std::tan(fov_rad / 2.0f)};

  vec::Mat4 projection_matrix{};
  projection_matrix(0, 0) = 1.0f / (aspect_ratio * t);
  projection_matrix(1, 1) = 1.0f / t;
  projection_matrix(2, 2) = -(far / (far - near));
  projection_matrix(2, 3) = -(far * near) / (far - near);
  projection_matrix(3, 2) = -1.0f;

  return projection_matrix;
}

vec::Vec3 Camera::get_position() const noexcept {
  float x{rho * std::sin(phi) * std::cos(theta)};
  float y{rho * std::sin(phi) * std::sin(theta)};
  float z{rho * std::cos(phi)};
  return {target.x() + x, target.y() + y, target.z() + z};
}