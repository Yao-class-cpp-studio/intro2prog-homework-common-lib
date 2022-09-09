#pragma once

#include "common/opengl/app.h"

namespace opengl::gui {
class Button : public Listener {
 public:
  Button(App *app, float left, float top, float right, float bottom);
  virtual ~Button();
  void OnCursorEnter(int enter) final;
  void OnMouseButton(int mouse_button, int state, int mods) final;
  void OnCursorPos(double xpos, double ypos) final;
  void OnWindowSize(int width, int height) final;
  void Resize(float left, float top, float right, float bottom);
  [[nodiscard]] glm::mat3 GetLocalToWorld() const;
  [[nodiscard]] glm::mat3 GetWorldToLocal() const;
  [[nodiscard]] glm::mat3 GetLocalToScreen() const;
  [[nodiscard]] glm::mat3 GetScreenToLocal() const;
  virtual void OnStateChange(int state);
  virtual void OnClick();
  virtual void OnResize();

  void Activate();
  void Deactivate();

 protected:
  void SetState(int state);
  glm::mat3 local_to_world_{};
  App *app_{nullptr};
  int state_{0};
  float left_{0};
  float top_{0};
  float right_{0};
  float bottom_{0};
};
}  // namespace opengl::gui
