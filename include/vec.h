#pragma once

#include <array>
#include <cmath>

namespace vec {
struct Vec2 {
  std::array<int, 2> data{};

  int& x() noexcept { return data[0]; }
  int& y() noexcept { return data[1]; }

  int x() const noexcept { return data[0]; }
  int y() const noexcept { return data[1]; }

  int& operator[](size_t i) noexcept { return data[i]; }
  const int& operator[](size_t i) const noexcept { return data[i]; }

  bool operator==(const Vec2&) const = default;
};

struct Vec2f {
  std::array<float, 2> data{};

  float& x() noexcept { return data[0]; }
  float& y() noexcept { return data[1]; }

  float x() const noexcept { return data[0]; }
  float y() const noexcept { return data[1]; }

  float& operator[](size_t i) noexcept { return data[i]; }
  const float& operator[](size_t i) const noexcept { return data[i]; }

  bool operator==(const Vec2f&) const = default;
};

struct Vec3 {
  std::array<float, 3> data{};

  float& x() noexcept { return data[0]; }
  float& y() noexcept { return data[1]; }
  float& z() noexcept { return data[2]; }

  float x() const noexcept { return data[0]; }
  float y() const noexcept { return data[1]; }
  float z() const noexcept { return data[2]; }

  float& operator[](size_t i) noexcept { return data[i]; }
  const float& operator[](size_t i) const noexcept { return data[i]; }

  bool operator==(const Vec3&) const = default;

  Vec3& operator+=(const Vec3& other) noexcept {
    for (size_t i{}; i < 3; ++i) {
      data[i] += other[i];
    }
    return *this;
  }

  Vec3& operator-=(const Vec3& other) noexcept {
    for (size_t i{}; i < 3; ++i) {
      data[i] -= other[i];
    }
    return *this;
  }

  Vec3& operator*=(float scalar) noexcept {
    for (size_t i{}; i < 3; ++i) {
      data[i] *= scalar;
    }
    return *this;
  }

  Vec3& operator/=(float scalar) noexcept {
    for (size_t i{}; i < 3; ++i) {
      data[i] /= scalar;
    }
    return *this;
  }
};

struct Vec4 {
  std::array<float, 4> data{};

  float& x() noexcept { return data[0]; }
  float& y() noexcept { return data[1]; }
  float& z() noexcept { return data[2]; }
  float& w() noexcept { return data[3]; }

  float x() const noexcept { return data[0]; }
  float y() const noexcept { return data[1]; }
  float z() const noexcept { return data[2]; }
  float w() const noexcept { return data[3]; }

  float& operator[](size_t i) noexcept { return data[i]; }
  const float& operator[](size_t i) const noexcept { return data[i]; }

  bool operator==(const Vec4&) const = default;

  Vec4& operator+=(const Vec4& other) noexcept {
    for (size_t i{}; i < 4; ++i) {
      data[i] += other[i];
    }
    return *this;
  }

  Vec4& operator-=(const Vec4& other) noexcept {
    for (size_t i{}; i < 4; ++i) {
      data[i] -= other[i];
    }
    return *this;
  }

  Vec4& operator*=(float scalar) noexcept {
    for (size_t i{}; i < 4; ++i) {
      data[i] *= scalar;
    }
    return *this;
  }

  Vec4& operator/=(float scalar) noexcept {
    for (size_t i{}; i < 4; ++i) {
      data[i] /= scalar;
    }
    return *this;
  }
};

struct Mat4 {
  std::array<float, 16> data{};

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

  void set_row(size_t row, const Vec4& vector) noexcept {
    for (size_t i{}; i < 4; ++i) {
      operator()(row, i) = vector[i];
    }
  }
};

// Vec3 operators
inline Vec3 operator+(Vec3 lhs, const Vec3& rhs) noexcept { return lhs += rhs; }
inline Vec3 operator-(Vec3 lhs, const Vec3& rhs) noexcept { return lhs -= rhs; }
inline Vec3 operator*(Vec3 vector, float scalar) noexcept {
  return vector *= scalar;
}
inline Vec3 operator*(float scalar, Vec3 vector) noexcept {
  return vector *= scalar;
}
inline Vec3 operator/(Vec3 vector, float scalar) noexcept {
  return vector /= scalar;
}

// Vec4 operators
inline Vec4 operator+(Vec4 lhs, const Vec4& rhs) noexcept { return lhs += rhs; }
inline Vec4 operator-(Vec4 lhs, const Vec4& rhs) noexcept { return lhs -= rhs; }
inline Vec4 operator*(Vec4 vector, float scalar) noexcept {
  return vector *= scalar;
}
inline Vec4 operator*(float scalar, Vec4 vector) noexcept {
  return vector *= scalar;
}
inline Vec4 operator/(Vec4 vector, float scalar) noexcept {
  return vector /= scalar;
}

// Mat4 operators
inline Mat4 operator*(const Mat4& lhs, const Mat4& rhs) noexcept {
  Mat4 result{};
  for (size_t i{}; i < 4; ++i) {
    for (size_t j{}; j < 4; ++j) {
      for (size_t k{}; k < 4; ++k) {
        result(i, j) += lhs(i, k) * rhs(k, j);
      }
    }
  }

  return result;
}

inline Vec4 operator*(const Mat4& matrix, const Vec4& vector) noexcept {
  Vec4 result{};

  for (size_t i{}; i < 4; ++i) {
    float sum{};
    for (size_t j{}; j < 4; ++j) {
      sum += matrix(i, j) * vector[j];
    }

    result[i] = sum;
  }

  return result;
}

// Vec3 functions
inline float dot(const Vec3& lhs, const Vec3& rhs) noexcept {
  return (lhs.x() * rhs.x()) + (lhs.y() * rhs.y()) + (lhs.z() * rhs.z());
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
  float x{(lhs.y() * rhs.z()) - (lhs.z() * rhs.y())};
  float y{-((lhs.x() * rhs.z()) - (lhs.z() * rhs.x()))};
  float z{(lhs.x() * rhs.y()) - (lhs.y() * rhs.x())};

  return {x, y, z};
}

}  // namespace vec