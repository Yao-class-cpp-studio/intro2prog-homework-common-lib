#include "common/opengl/buffer.h"

#include <cstring>

namespace opengl {

Buffer::Buffer(size_t size, void *data) {
  buffer_ = 0;
  OpenGLCall(glCreateBuffers, 1, &buffer_);
  OpenGLCall(glNamedBufferData, buffer_, static_cast<GLsizeiptr>(size), data,
             GL_DYNAMIC_DRAW);
}

size_t Buffer::GetSize() const {
  int size;
  OpenGLCall(glGetNamedBufferParameteriv, buffer_, GL_BUFFER_SIZE, &size);
  return size;
}

uint32_t Buffer::GetHandle() const {
  return buffer_;
}

Buffer::~Buffer() {
  OpenGLCall(glDeleteBuffers, 1, &buffer_);
}

void Buffer::Resize(size_t new_size) const {
  OpenGLCall(glNamedBufferData, buffer_, new_size, nullptr, GL_DYNAMIC_DRAW);
}

uint8_t *Buffer::Map() const {
  auto ptr = glMapNamedBuffer(buffer_, GL_READ_WRITE);
  return static_cast<uint8_t *>(ptr);
}

void Buffer::Unmap() const {
  OpenGLCall(glUnmapNamedBuffer, buffer_);
}

void Buffer::UploadData(size_t offset, size_t length, void *host_data) const {
  if (offset + length > GetSize())
    Resize(offset + length);
  OpenGLCall(glNamedBufferSubData, buffer_, static_cast<long>(offset),
             static_cast<long>(length), host_data);
}
}  // namespace opengl
