#include "common/opengl/gui/button.h"

namespace opengl::gui {

Button::Button(App *app, float left, float top, float right, float bottom) {
  app_ = app;
  app_->RegisterListener(this);
  if (top > bottom)
    std::swap(top, bottom);
  left_ = left;
  top_ = top;
  right_ = right;
  bottom_ = bottom;

  auto screen_to_world = app_->GetScreenToWorld();
  auto left_bottom_pos = screen_to_world * glm::vec3{left, bottom, 1.0};
  auto right_top_pos = screen_to_world * glm::vec3{right, top, 1.0};
  auto mid_pos = (left_bottom_pos + right_top_pos) * 0.5f;

  local_to_world_ = glm::mat3{(right_top_pos.x - left_bottom_pos.x) * 0.5,
                              0.0,
                              0.0,
                              0.0,
                              (right_top_pos.y - left_bottom_pos.y) * 0.5,
                              0.0,
                              mid_pos.x,
                              mid_pos.y,
                              1.0};
}

Button::~Button() {
  app_->UnregisterListener(this);
}

void Button::OnCursorEnter(int enter) {
  if (!enter) {
    if (state_) {
      SetState(0);
    }
  }
}

void Button::OnMouseButton(int mouse_button, int state, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(app_->GetHandle(), &xpos, &ypos);
  auto local_pos = glm::vec2{float(xpos), float(ypos)};
  bool inside = (left_ <= local_pos.x) && (local_pos.x <= right_) &&
                (top_ <= local_pos.y) && (local_pos.y <= bottom_);
  if (mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
    if (state == GLFW_RELEASE) {
      if (inside) {
        if (state_ == 2) {
          OnClick();
        }
        SetState(1);
      }
    } else if (state == GLFW_PRESS) {
      if (inside) {
        SetState(2);
      }
    }
  }
}

void Button::OnCursorPos(double xpos, double ypos) {
  auto local_pos = glm::vec2{float(xpos), float(ypos)};
  bool inside = (left_ <= local_pos.x) && (local_pos.x <= right_) &&
                (top_ <= local_pos.y) && (local_pos.y <= bottom_);
  if (inside) {
    if (!state_) {
      SetState(1);
    }
  } else {
    if (state_) {
      SetState(0);
    }
  }
}

void Button::Resize(float left, float top, float right, float bottom) {
  if (top > bottom)
    std::swap(top, bottom);

  left_ = left;
  top_ = top;
  right_ = right;
  bottom_ = bottom;

  auto screen_to_world = app_->GetScreenToWorld();
  auto left_bottom_pos = screen_to_world * glm::vec3{left, bottom, 1.0};
  auto right_top_pos = screen_to_world * glm::vec3{right, top, 1.0};
  auto mid_pos = (left_bottom_pos + right_top_pos) * 0.5f;

  local_to_world_ = glm::mat3{(right_top_pos.x - left_bottom_pos.x) * 0.5,
                              0.0,
                              0.0,
                              0.0,
                              (right_top_pos.y - left_bottom_pos.y) * 0.5,
                              0.0,
                              mid_pos.x,
                              mid_pos.y,
                              1.0};

  OnResize();
}

glm::mat3 Button::GetLocalToWorld() const {
  return local_to_world_;
}

glm::mat3 Button::GetWorldToLocal() const {
  return glm::inverse(local_to_world_);
}

glm::mat3 Button::GetLocalToScreen() const {
  return app_->GetWorldToScreen() * local_to_world_;
}

glm::mat3 Button::GetScreenToLocal() const {
  return GetWorldToLocal() * app_->GetScreenToWorld();
}

void Button::SetState(int state) {
  state_ = state;
  OnStateChange(state);
}

void Button::OnClick() {
}

void Button::OnResize() {
}

void Button::OnStateChange(int state) {
}

void Button::OnWindowSize(int width, int height) {
  Listener::OnWindowSize(width, height);
}

void Button::Activate() {
  app_->RegisterListener(this);
}

void Button::Deactivate() {
  app_->UnregisterListener(this);
}
}  // namespace opengl::gui
