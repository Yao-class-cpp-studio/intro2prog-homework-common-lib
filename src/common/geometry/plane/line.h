#pragma once
#include <glm/glm.hpp>
#include <iostream>

#include "common/geometry/plane/segment.h"

namespace geometry::plane {
struct Line {
  glm::vec2 n;
  float d;
  explicit Line(const Segment &segment);
  bool operator==(const Line &line) const;
  float operator()(const glm::vec2 &v) const;
};

glm::vec2 Intersection(const Line &line0, const Line &line1);
}  // namespace geometry::plane
