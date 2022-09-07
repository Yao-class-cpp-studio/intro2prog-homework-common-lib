#include "common/opengl/gui/model.h"

#include <map>

namespace opengl::gui {

#ifdef COMPARE
#undef COMPARE
#endif
namespace {
struct MyCmp {
  bool operator()(const glm::vec2 &v0, const glm::vec2 &v1) const {
#define COMPARE(x) \
  if (v0.x < v1.x) \
    return true;   \
  if (v1.x < v0.x) \
    return false;
    COMPARE(x);
    COMPARE(y);
#undef COMPARE
    return false;
  }
};
}  // namespace

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

Model::Model(const std::vector<glm::vec2> &mesh, const glm::vec3 &color) {
  std::map<glm::vec2, int, MyCmp> vertex_index;
  auto &vertices = vertices_;
  auto &indices = indices_;

  auto find_vertex_index = [&vertex_index, &vertices,
                            color](const glm::vec2 &v) {
    if (!vertex_index.count(v)) {
      vertex_index[v] = int(vertices.size());
      vertices.push_back({{v, 1.0}, color});
    }
    return vertex_index.at(v);
  };

  for (auto &point : mesh) {
    indices.push_back(find_vertex_index(point));
  }
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
