#include "common/opengl/framebuffer.h"

namespace opengl {

Framebuffer::Framebuffer(uint32_t texture) {
  OpenGLCall(glCreateFramebuffers, 1, &frame_buffer_);
  OpenGLCall(glBindFramebuffer, GL_FRAMEBUFFER, frame_buffer_);
  OpenGLCall(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
             GL_TEXTURE_2D, texture, 0);
  OpenGLCall(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
  OpenGLCall(glDeleteFramebuffers, 1, &frame_buffer_);
}

uint32_t Framebuffer::GetHandle() {
  return frame_buffer_;
}

}  // namespace opengl
