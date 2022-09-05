#pragma once

namespace opengl {
class Listener {
 public:
  virtual void OnCursorEnter(int enter);
  virtual void OnCursorPos(double xpos, double ypos);
  virtual void OnMouseButton(int mouse_button, int state, int mods);
  virtual void OnWindowSize(int width, int height);
};
}  // namespace opengl
