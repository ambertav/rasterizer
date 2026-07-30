#pragma once

#include <numbers>

#include "vec.h"

class Light {
 public:
  Light(float th = std::numbers::pi_v<float>, float p = std::numbers::pi_v<float> / 2.0f);

  ~Light() = default;
  Light(const Light&) = default;
  Light& operator=(const Light&) = default;
  Light(Light&&) = default;
  Light& operator=(Light&&) = default;

  void orbit(float delta_theta, float delta_phi) noexcept;
  [[nodiscard]] vec::Vec3 get_direction() const noexcept;

 private:
  float theta;  // horizontal angle
  float phi;    // vertical angle
};