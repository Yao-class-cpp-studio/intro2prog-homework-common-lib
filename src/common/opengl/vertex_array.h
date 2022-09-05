#pragma once
#include "common/opengl/app.h"
#include "common/opengl/buffer.h"

namespace opengl {
class VertexArray {
 public:
  VertexArray(Buffer *vertex_buffer,
              const std::vector<uint32_t> &attribs,
              Buffer *index_buffer);
  ~VertexArray();
  [[nodiscard]] uint32_t GetHandle() const;

 private:
  uint32_t vertex_array_{};
};
}  // namespace opengl
