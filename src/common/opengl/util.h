#pragma once

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <string>
#include <string_view>

#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)

#define LAND_PI 3.14159265358979323846

namespace opengl {
template <class glFunc, class... glArgs>
void SafeGLFunctionCall(const char *code,
                        const char *file,
                        int line,
                        glFunc &&func,
                        glArgs &&...args) {
  while (glGetError() != GL_NO_ERROR)
    ;
  func(args...);
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    LOG_ERROR("({}:{}) OpenGL ({}): {}", file, line, error, code);
  }
}

#define OpenGLCall(GLFUNC, ...) \
  opengl::SafeGLFunctionCall(#GLFUNC, __FILE__, __LINE__, GLFUNC, __VA_ARGS__)

std::vector<uint8_t> ReadFileBinary(std::string_view file_path);
}  // namespace opengl
