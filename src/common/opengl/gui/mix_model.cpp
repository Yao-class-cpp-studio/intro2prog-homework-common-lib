#include "common/opengl/gui/mix_model.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "common/opengl/gui/interpolation.h"
#include "common/opengl/util.h"

namespace opengl::gui {
MixModel::MixModel(const std::vector<std::vector<ColorVertex>> &vertices,
                   const std::vector<uint32_t> &indices) {
  for (size_t i = 1; i < vertices.size(); i++) {
    assert(vertices[0].size() == vertices[i].size());
  }
  vertices_ = vertices;
  mixed_model_ = std::make_unique<Model>(vertices[0], indices);
}

Model *MixModel::GetModel(float alpha, MixStyle mix_style) {
  auto &mixed_vertices = mixed_model_->GetVertices();
  float index;
  alpha = std::modf(alpha, &index);
  int i0 = int(index) % int(vertices_.size());
  int i1 = (i0 + 1) % int(vertices_.size());

  auto &vertices0 = vertices_[i0];
  auto &vertices1 = vertices_[i1];
  switch (mix_style) {
    case MixStyle::kLinear:
      for (size_t i = 0; i < mixed_vertices.size(); i++) {
        mixed_vertices[i].position =
            Mix(vertices0[i].position, vertices1[i].position, alpha);
        mixed_vertices[i].color =
            Mix(vertices0[i].color, vertices1[i].color, alpha);
      }
      break;
    case MixStyle::kAngularClockwise:
      for (size_t i = 0; i < mixed_vertices.size(); i++) {
        auto p0 = glm::vec2{vertices0[i].position};
        auto p1 = glm::vec2{vertices1[i].position};
        auto am0 = glm::vec2{std::atan2(p0.x, p0.y), glm::length(p0)};
        auto am1 = glm::vec2{std::atan2(p1.x, p1.y), glm::length(p1)};
        if (am1.x < am0.x)
          am1.x += float(LAND_PI * 2);
        am1 = Mix(am0, am1, alpha);
        mixed_vertices[i].position =
            glm::vec3(glm::vec2(std::sin(am1.x), std::cos(am1.x)) * am1.y, 1.0);
        mixed_vertices[i].color =
            Mix(vertices0[i].color, vertices1[i].color, alpha);
      }
      break;
  }
  return mixed_model_.get();
}
}  // namespace opengl::gui
