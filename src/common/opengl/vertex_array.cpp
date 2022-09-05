#include "common/opengl/vertex_array.h"

namespace opengl {

VertexArray::VertexArray(Buffer *vertex_buffer,
                         const std::vector<uint32_t> &attribs,
                         Buffer *index_buffer) {
  OpenGLCall(glCreateVertexArrays, 1, &vertex_array_);
  OpenGLCall(glBindVertexArray, vertex_array_);

  int stride = 0;
  for (auto attrib : attribs) {
    switch (attrib) {
      case GL_R32F:
        stride += 4;
        break;
      case GL_RG32F:
        stride += 8;
        break;
      case GL_RGB32F:
        stride += 12;
        break;
      case GL_RGBA32F:
        stride += 16;
        break;
      default:
        LOG_ERROR("Type {} is NOT supported.", attrib);
        break;
    }
  }
  OpenGLCall(glBindBuffer, GL_ARRAY_BUFFER, vertex_buffer->GetHandle());
  OpenGLCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, index_buffer->GetHandle());
  int offset = 0;
  for (size_t i = 0; i < attribs.size(); i++) {
    auto attrib = attribs[i];
    switch (attrib) {
      case GL_R32F:
        OpenGLCall(glVertexAttribPointer, i, 1, GL_FLOAT, GL_FALSE, stride,
                   (void *)offset);
        offset += 4;
        break;
      case GL_RG32F:
        OpenGLCall(glVertexAttribPointer, i, 2, GL_FLOAT, GL_FALSE, stride,
                   (void *)offset);
        offset += 8;
        break;
      case GL_RGB32F:
        OpenGLCall(glVertexAttribPointer, i, 3, GL_FLOAT, GL_FALSE, stride,
                   (void *)offset);
        offset += 12;
        break;
      case GL_RGBA32F:
        OpenGLCall(glVertexAttribPointer, i, 4, GL_FLOAT, GL_FALSE, stride,
                   (void *)offset);
        offset += 16;
        break;
      default:
        LOG_ERROR("Type {} is NOT supported.", attrib);
        break;
    }
    OpenGLCall(glEnableVertexAttribArray, i);
  }
  OpenGLCall(glBindVertexArray, 0);
  OpenGLCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray() {
  OpenGLCall(glDeleteVertexArrays, 1, &vertex_array_);
}

uint32_t VertexArray::GetHandle() const {
  return vertex_array_;
}
}  // namespace opengl
