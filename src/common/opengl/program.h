#pragma once
#include "common/opengl/app.h"

namespace opengl {
class Program {
 public:
  Program(std::string_view vert_shader_file, std::string_view frag_shader_file);
  Program(const std::vector<uint8_t> &vert_shader_code,
          const std::vector<uint8_t> &frag_shader_code);
  ~Program();
  [[nodiscard]] uint32_t GetHandle() const;

 private:
  uint32_t program_;
};
}  // namespace opengl
