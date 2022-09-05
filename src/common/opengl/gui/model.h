#pragma once

#include <vector>

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

  std::vector<ColorVertex> &GetVertices();

  std::vector<uint32_t> &GetIndices();

 private:
  std::vector<ColorVertex> vertices_;
  std::vector<uint32_t> indices_;
};

std::vector<ColorVertex> ComposeVertices(
    const std::vector<glm::vec3> &positions,
    const glm::vec3 &color);

}  // namespace opengl::gui
