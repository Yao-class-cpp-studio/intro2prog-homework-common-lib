#include "common/opengl/gui/interpolation.h"

namespace opengl::gui {

float PowerInterpolation(float x, float index) {
  if (x < 0.5f)
    return std::pow(x * 2.0f, index) * 0.5f;
  else
    return 1.0f - std::pow((1.0f - x) * 2.0f, index) * 0.5f;
}

float CosineInterpolation(float x) {
  return (std::cos(x * float(M_PI)) + 1.0f) * 0.5f;
}
}  // namespace opengl::gui
