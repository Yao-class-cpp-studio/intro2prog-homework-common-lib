#include "common/opengl/app.h"
#include "common/opengl/texture.h"

namespace opengl {
class Framebuffer {
 public:
  Framebuffer(uint32_t texture);
  ~Framebuffer();
  uint32_t GetHandle();

 private:
  uint32_t frame_buffer_{0};
};
}  // namespace opengl
