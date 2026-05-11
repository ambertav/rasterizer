#include "camera.h"

Camera::Camera(vec::Vec3 t = {0.0f, 0.0f, 0.0f}, float r = 5.0f,
               float th = 0.0f, float p = M_PI / 2.0f, float fv = 45.0f,
               float n = 0.1f, float fr = 1000.0f)
    : target(t), rho(r), theta(th), phi(p), fov(fv), near(n), far(fr) {}

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

  vec::Vec3 world_up{0.0f, 0.0f, 1.0f};
  vec::Vec3 position{get_position()};

  vec::Vec3 forward{vec::normalize(position - target)};
  vec::Vec3 right{vec::normalize(vec::cross(world_up, forward))};
  vec::Vec3 up{vec::normalize(vec::cross(forward, right))};

  vec::Vec4 axes[4]{
      {right.x, right.y, right.z, -vec::dot(right, position)},
      {up.x, up.y, up.z, -vec::dot(up, position)},
      {forward.x, forward.y, forward.z, -vec::dot(forward, position)},
      {0.0f, 0.0f, 0.0f, 1.0f}};

  vec::Mat4 view_matrix{};
  for (size_t i{}; i < 4; ++i) {
    view_matrix.set_row(i, axes[i]);
  }

  return view_matrix;
}

vec::Mat4 Camera::get_projection_matrix() const noexcept {}

vec::Vec3 Camera::get_position() const noexcept {
  float x{rho * std::sin(phi) * std::cos(theta)};
  float y{rho * std::sin(phi) * std::sin(theta)};
  float z{rho * std::cos(phi)};
  return {target.x + x, target.y + y, target.z + z};
}
