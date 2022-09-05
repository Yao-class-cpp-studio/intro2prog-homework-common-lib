#pragma once

#include <memory>

#include "common/opengl/gui/interpolation.h"
#include "common/opengl/gui/model.h"

namespace opengl::gui {

enum class MixStyle : uint32_t { kLinear, kAngularClockwise };

class MixModel {
 public:
  MixModel(const std::vector<std::vector<ColorVertex>> &vertices,
           const std::vector<uint32_t> &indices);
  Model *GetModel(float alpha, MixStyle mix_style);

 private:
  std::vector<std::vector<ColorVertex>> vertices_;
  std::unique_ptr<Model> mixed_model_;
};
}  // namespace opengl::gui
