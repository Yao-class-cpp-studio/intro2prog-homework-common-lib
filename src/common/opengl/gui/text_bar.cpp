#include "common/opengl/gui/text_bar.h"

#include <map>

namespace opengl::gui {

#ifdef COMPARE
#undef COMPARE
#endif
namespace {
struct MyCmp {
  bool operator()(const glm::vec2 &v0, const glm::vec2 &v1) const {
#define COMPARE(x) \
  if (v0.x < v1.x) \
    return true;   \
  if (v1.x < v0.x) \
    return false;
    COMPARE(x);
    COMPARE(y);
#undef COMPARE
    return false;
  }
};
}  // namespace

TextBar::TextBar(App *app,
                 FontFactory *font_factory,
                 const std::u32string &text,
                 float font_size,
                 glm::vec3 font_color,
                 glm::vec2 origin,
                 AlignMode align_mode) {
  app_ = app;
  font_factory_ = font_factory;
  font_color_ = font_color;
  font_size_ = font_size;
  origin_ = origin;
  align_mode_ = align_mode;
  model_ = std::make_unique<Model>();
  device_model_ = std::make_unique<DeviceModel>(model_.get());
  program_ = std::make_unique<Program>("../assets/shaders/color_geom.vert",
                                       "../assets/shaders/color_geom.frag");
  app_->RegisterListener(this);
  UpdateText(text);
}

TextBar::~TextBar() {
  app_->UnregisterListener(this);
}

void TextBar::UpdateText(const std::u32string &text) {
  text_ = text;
  BuildMesh();
}

glm::mat3 TextBar::LocalToScreen() const {
  return glm::mat3{1.0f, 0.0f,      0.0f,      0.0f, -1.0f,
                   0.0f, origin_.x, origin_.y, 1.0f} *
         glm::mat3{
             font_size_, 0.0f, 0.0f, 0.0f, font_size_, 0.0f, 0.0f, 0.0f, 1.0f,
         };
}

glm::mat3 TextBar::ScreenToLocal() const {
  return glm::inverse(LocalToScreen());
}

glm::mat3 TextBar::LocalToWorld() const {
  return app_->GetScreenToWorld() * LocalToScreen();
}

glm::mat3 TextBar::WorldToLocal() const {
  return ScreenToLocal() * app_->GetWorldToScreen();
}

void TextBar::BuildMesh() {
  float advance = 0.0;
  auto triangles = font_factory_->GetStringMesh(text_, 1, &advance);
  switch (align_mode_) {
    case AlignMode::kLeft:
      break;
    case AlignMode::kMid:
      for (auto &triangle : triangles) {
        triangle.x -= advance * 0.5f;
      }
      break;
    case AlignMode::kRight:
      for (auto &triangle : triangles) {
        triangle.x -= advance;
      }
      break;
  }
  std::map<glm::vec2, int, MyCmp> vertex_index;
  std::vector<ColorVertex> vertices;
  std::vector<uint32_t> indices;

  auto font_color = font_color_;

  auto find_vertex_index = [&vertex_index, &vertices,
                            font_color](const glm::vec2 &v) {
    if (!vertex_index.count(v)) {
      vertex_index[v] = int(vertices.size());
      vertices.push_back({{v, 1.0}, font_color});
    }
    return vertex_index.at(v);
  };

  for (auto &triangle : triangles) {
    indices.push_back(find_vertex_index(triangle));
  }

  model_ = std::make_unique<Model>(vertices, indices);
  device_model_->Update(model_.get());
  auto local_to_world = LocalToWorld();
  OpenGLCall(glProgramUniformMatrix3fv, program_->GetHandle(),
             glGetUniformLocation(program_->GetHandle(), "transformation"), 1,
             false, (float *)&local_to_world);
}

void TextBar::Draw() {
  OpenGLCall(glUseProgram, program_->GetHandle());
  device_model_->Draw(GL_TRIANGLES);
}

void TextBar::Resize(float font_size, glm::vec2 origin) {
  font_size_ = font_size;
  origin_ = origin;
  auto local_to_world = LocalToWorld();
  OpenGLCall(glProgramUniformMatrix3fv, program_->GetHandle(),
             glGetUniformLocation(program_->GetHandle(), "transformation"), 1,
             false, (float *)&local_to_world);
}

}  // namespace opengl::gui
