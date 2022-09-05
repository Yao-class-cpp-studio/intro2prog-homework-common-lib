#pragma once

#include "common/opengl/buffer.h"
#include "common/opengl/gui/model.h"
#include "common/opengl/vertex_array.h"

namespace opengl::gui {
class DeviceModel {
 public:
  explicit DeviceModel(Model *model);

  void Update(Model *model);

  void Draw(uint32_t draw_type);

 private:
  std::unique_ptr<opengl::Buffer> vertex_buffer_;
  std::unique_ptr<opengl::Buffer> index_buffer_;
  std::unique_ptr<opengl::VertexArray> vertex_array_;
  uint32_t num_index_{0};
};
}  // namespace opengl::gui
