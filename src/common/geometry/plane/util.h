#pragma once

#include <glm/glm.hpp>

namespace geometry::plane {
constexpr float eps = 1e-8;

template <class Ty>
bool IsZero(const Ty &val) {
  return glm::length(val) < eps;
}

template <class Ty>
bool Equal(const Ty &v0, const Ty &v1) {
  return IsZero(v0 - v1);
}

bool Between(float x0, float x1, float y);
}  // namespace geometry::plane
