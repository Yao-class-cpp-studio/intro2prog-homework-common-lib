#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "common/geometry/plane/triangle.h"

namespace geometry::plane {
class Mesh {
 public:
  Mesh();
  explicit Mesh(std::vector<glm::vec2> outline);
  std::vector<Triangle> &GetTriangles();
  [[nodiscard]] const std::vector<Triangle> &GetTriangles() const;
  Mesh &operator+=(const Mesh &mesh);
  static Mesh DiscardIntersection(const Mesh &mesh);

 private:
  std::vector<Triangle> triangles;
};
}  // namespace geometry::plane
