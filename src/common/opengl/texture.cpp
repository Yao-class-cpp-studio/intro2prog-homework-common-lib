#include "common/opengl/texture.h"

namespace opengl {

Texture::Texture(int width, int height, void *data) {
  OpenGLCall(glCreateTextures, GL_TEXTURE_2D, 1, &texture_);
  OpenGLCall(glBindTexture, GL_TEXTURE_2D, texture_);
  OpenGLCall(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
             GL_RGBA, GL_FLOAT, data);
  OpenGLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  OpenGLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  OpenGLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
             GL_CLAMP_TO_EDGE);
  OpenGLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
             GL_CLAMP_TO_EDGE);
  OpenGLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_R,
             GL_CLAMP_TO_EDGE);
  OpenGLCall(glBindTexture, GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
  OpenGLCall(glDeleteTextures, 1, &texture_);
}

uint32_t Texture::GetHandle() const {
  return texture_;
}

}  // namespace opengl
