#include "common/opengl/app.h"

namespace opengl {

class Texture {
 public:
  Texture(int width, int height, void *data);
  ~Texture();
  [[nodiscard]] uint32_t GetHandle() const;

 private:
  uint32_t texture_{0};
};

}  // namespace opengl
