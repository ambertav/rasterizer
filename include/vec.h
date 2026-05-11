#pragma once

#include <compare>

namespace vec {
struct Vec2 {
  int x{};
  int y{};

  auto operator<=>(const Vec2&) const = default;
};

struct Vec3 {
  float x{};
  float y{};
  float z{};

  auto operator<=>(const Vec3&) const = default;

  Vec3& operator+=(const Vec3& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Vec3& operator-=(const Vec3& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  Vec3& operator*=(float scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  Vec3& operator/=(float scalar) noexcept {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }
};

struct Vec4 {
  float x{};
  float y{};
  float z{};
  float w{};

  auto operator<=>(const Vec4&) const = default;
};

struct Mat4 {
  float data[16]{};

  static Mat4 identity() {
    Mat4 matrix{};

    for (size_t i{}; i < 4; ++i) {
      matrix(i, i) = 1.0f;
    }

    return matrix;
  }

  float& operator()(size_t row, size_t col) { return data[row * 4 + col]; }

  const float& operator()(size_t row, size_t col) const {
    return data[row * 4 + col];
  }
};

inline Vec3 operator+(Vec3 lhs, const Vec3& rhs) noexcept { return lhs += rhs; }

inline Vec3 operator-(Vec3 lhs, const Vec3& rhs) noexcept { return lhs -= rhs; }

inline Vec3 operator*(Vec3 vector, float scalar) noexcept {
  return vector *= scalar;
}

inline Vec3 operator/(Vec3 vector, float scalar) noexcept {
  return vector /= scalar;
}

inline float dot(const Vec3& lhs, const Vec3& rhs) noexcept {
  return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

inline float length(const Vec3& vector) noexcept {
  return std::sqrt(dot(vector, vector));
}

inline Vec3 normalize(const Vec3& vector) noexcept {
  float len{length(vector)};
  if (len == 0.0f) {
    return vector;
  }
  return vector / len;
}

inline Vec3 cross(const Vec3& lhs, const Vec3& rhs) noexcept {
  float x{(lhs.y * rhs.z) - (lhs.z * rhs.y)};
  float y{ -1 * ((lhs.x * rhs.z) - (lhs.z * rhs.x))};
  float z{(lhs.x * rhs.y) - (lhs.y * rhs.x)};

  return {x, y, z};
}

}  // namespace vec