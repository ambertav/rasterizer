#include "light.h"

#include <algorithm>

Light::Light(float th, float p) : theta(th), phi(p) {}

void Light::orbit(float delta_theta, float delta_phi) noexcept {
  theta += delta_theta;
  phi = std::clamp(phi + delta_phi, 0.01f, std::numbers::pi_v<float> - 0.01f);
}

vec::Vec3 Light::get_direction() const noexcept {
  float x{std::sin(phi) * std::cos(theta)};
  float y{std::sin(phi) * std::sin(theta)};
  float z{std::cos(phi)};

  return vec::normalize(vec::Vec3{x, y, z});
}