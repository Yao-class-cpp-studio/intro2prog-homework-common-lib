#pragma once

#include <vector>

#include "common/geometry/plane/mesh.h"
#include "glm/glm.hpp"

namespace opengl::gui {

struct ColorVertex {
  glm::vec3 position;
  glm::vec3 color;
};

class Model {
 public:
  Model();

  Model(const std::vector<glm::vec3> &positions,
        const glm::vec3 &color,
        const std::vector<uint32_t> &indices);

  Model(const std::vector<ColorVertex> &vertices,
        const std::vector<uint32_t> &indices);

  Model(const std::vector<glm::vec2> &mesh, const glm::vec3 &color);

  std::vector<ColorVertex> &GetVertices();
  [[nodiscard]] const std::vector<ColorVertex> &GetVertices() const;

  std::vector<uint32_t> &GetIndices();
  [[nodiscard]] const std::vector<uint32_t> &GetIndices() const;

 private:
  std::vector<ColorVertex> vertices_;
  std::vector<uint32_t> indices_;
};

std::vector<ColorVertex> ComposeVertices(
    const std::vector<glm::vec3> &positions,
    const glm::vec3 &color);

Model MergeModel(const Model &model0, const Model &model1);

}  // namespace opengl::gui
