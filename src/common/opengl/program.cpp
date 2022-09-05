#include "common/opengl/program.h"

namespace opengl {
;

Program::Program(std::string_view vert_shader_file,
                 std::string_view frag_shader_file)
    : Program(ReadFileBinary(vert_shader_file),
              ReadFileBinary(frag_shader_file)) {
}

namespace {
uint32_t CompileShaderFromCode(const std::vector<uint8_t> &shader_code,
                               uint32_t shader_stage) {
  auto shader = glCreateShader(shader_stage);
  int length = static_cast<int>(shader_code.size());
  const char *code = reinterpret_cast<const char *>(shader_code.data());
  OpenGLCall(glShaderSource, shader, 1, &code, &length);
  OpenGLCall(glCompileShader, shader);
  int success;
  OpenGLCall(glGetShaderiv, shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    OpenGLCall(glGetShaderiv, shader, GL_INFO_LOG_LENGTH, &length);
    std::string compilation_info;
    compilation_info.resize(length);
    OpenGLCall(glGetShaderInfoLog, shader, length, &length,
               compilation_info.data());
    LOG_ERROR("Shader compilation error: \n{}", compilation_info);
  }
  return shader;
}
}  // namespace

Program::Program(const std::vector<uint8_t> &vert_shader_code,
                 const std::vector<uint8_t> &frag_shader_code) {
  auto vert_shader = CompileShaderFromCode(vert_shader_code, GL_VERTEX_SHADER);
  auto frag_shader =
      CompileShaderFromCode(frag_shader_code, GL_FRAGMENT_SHADER);
  program_ = glCreateProgram();
  OpenGLCall(glAttachShader, program_, vert_shader);
  OpenGLCall(glAttachShader, program_, frag_shader);
  OpenGLCall(glLinkProgram, program_);
  int success;
  OpenGLCall(glGetProgramiv, program_, GL_LINK_STATUS, &success);
  if (!success) {
    OpenGLCall(glGetProgramiv, program_, GL_INFO_LOG_LENGTH, &success);
    std::string link_info;
    link_info.resize(success);
    OpenGLCall(glGetProgramInfoLog, program_, success, &success,
               link_info.data());
    LOG_ERROR("Program link error: \n{}", link_info);
  }
  OpenGLCall(glDeleteShader, vert_shader);
  OpenGLCall(glDeleteShader, frag_shader);
}

uint32_t Program::GetHandle() const {
  return program_;
}

Program::~Program() {
  OpenGLCall(glDeleteProgram, program_);
}
}  // namespace opengl
