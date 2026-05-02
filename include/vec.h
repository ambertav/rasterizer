#pragma once

#include <compare>

namespace vec {
struct Vec2 {
  int x{};
  int y{};

  auto operator<=>(const Vec2&) const = default;
};
}  // namespace vec