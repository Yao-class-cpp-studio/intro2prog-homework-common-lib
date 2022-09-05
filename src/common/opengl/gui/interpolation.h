#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>

namespace opengl::gui {

template <class Ty>
Ty Mix(const Ty &v0, const Ty &v1, float alpha) {
  return v0 * (1.0f - alpha) + v1 * alpha;
}

float PowerInterpolation(float x, float index);

float CosineInterpolation(float x);

}  // namespace opengl::gui
