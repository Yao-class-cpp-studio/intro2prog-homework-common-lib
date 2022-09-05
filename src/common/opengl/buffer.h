#pragma once
#include <vector>

#include "common/opengl/app.h"

namespace opengl {
class Buffer {
 public:
  Buffer(size_t size, void *data);
  ~Buffer();
  [[nodiscard]] size_t GetSize() const;
  [[nodiscard]] uint32_t GetHandle() const;
  [[nodiscard]] uint8_t *Map() const;
  void Unmap() const;
  void Resize(size_t new_size) const;
  void UploadData(size_t offset, size_t length, void *host_data) const;

 private:
  uint32_t buffer_;
};
}  // namespace opengl
