#include "common/opengl/gui/text_bar.h"

#include <map>

namespace opengl::gui {

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

  model_ = std::make_unique<Model>(triangles, font_color_);
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
