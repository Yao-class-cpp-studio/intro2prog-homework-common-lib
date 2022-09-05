#pragma once

#include "common/opengl/gui/device_model.h"
#include "common/opengl/gui/font_factory.h"
#include "common/opengl/gui/model.h"
#include "common/opengl/program.h"

namespace opengl::gui {
class TextBar : public Listener {
 public:
  enum class AlignMode : uint32_t { kLeft, kMid, kRight };

  TextBar(App *app,
          FontFactory *font_factory,
          const std::u32string &text,
          float font_size,
          glm::vec3 font_color,
          glm::vec2 origin,
          AlignMode align_mode = AlignMode::kLeft);

  ~TextBar();

  void UpdateText(const std::u32string &text);

  [[nodiscard]] glm::mat3 LocalToScreen() const;

  [[nodiscard]] glm::mat3 ScreenToLocal() const;

  [[nodiscard]] glm::mat3 LocalToWorld() const;

  [[nodiscard]] glm::mat3 WorldToLocal() const;

  void Draw();

  void Resize(float font_size, glm::vec2 origin);

 private:
  void BuildMesh();

  App *app_;
  FontFactory *font_factory_;
  float font_size_;
  glm::vec2 origin_{};
  AlignMode align_mode_;
  glm::vec3 font_color_{};
  std::unique_ptr<Program> program_;
  std::unique_ptr<Model> model_;
  std::unique_ptr<DeviceModel> device_model_;
  std::u32string text_;
};
}  // namespace opengl::gui
