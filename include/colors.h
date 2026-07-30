#pragma once

#include <cstdint>

namespace colors {
inline constexpr uint32_t BLACK{0xFF000000};
inline constexpr uint32_t WHITE{0xFFFFFFFF};
inline constexpr uint32_t RED{0xFFFF0000};
inline constexpr uint32_t GREEN{0xFF00FF00};
inline constexpr uint32_t BLUE{0xFF0000FF};

inline uint32_t from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
  return (a << 24) | (r << 16) | (g << 8) | b;
}

inline uint32_t scale_color(uint32_t color, float intensity) {
  intensity = std::clamp(intensity, 0.0f, 1.0f);

  uint8_t r{static_cast<uint8_t>(((color >> 16) & 0xFF) * intensity)};
  uint8_t g{static_cast<uint8_t>(((color >> 8) & 0xFF) * intensity)};
  uint8_t b{static_cast<uint8_t>(((color >> 0) & 0xFF) * intensity)};
  uint8_t a{static_cast<uint8_t>((color >> 24) & 0xFF)};

  return (a << 24) | (r << 16) | (g << 8) | b;
}
}  // namespace colors