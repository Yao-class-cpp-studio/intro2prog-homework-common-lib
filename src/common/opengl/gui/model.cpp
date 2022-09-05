#include "common/opengl/gui/model.h"

namespace opengl::gui {
Model::Model() = default;

Model::Model(const std::vector<glm::vec3> &positions,
             const glm::vec3 &color,
             const std::vector<uint32_t> &indices)
    : Model(ComposeVertices(positions, color), indices) {
}

Model::Model(const std::vector<ColorVertex> &vertices,
             const std::vector<uint32_t> &indices) {
  vertices_ = vertices;
  indices_ = indices;
}

std::vector<ColorVertex> &Model::GetVertices() {
  return vertices_;
}

std::vector<uint32_t> &Model::GetIndices() {
  return indices_;
}

std::vector<ColorVertex> ComposeVertices(
    const std::vector<glm::vec3> &positions,
    const glm::vec3 &color) {
  std::vector<ColorVertex> vertices;
  vertices.reserve(positions.size());
  for (auto &position : positions) {
    vertices.push_back({position, color});
  }
  return vertices;
}
}  // namespace opengl::gui
