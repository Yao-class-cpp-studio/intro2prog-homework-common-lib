#include "common/opengl/app.h"

#include "common/opengl/util.h"

namespace opengl {

App::App() : App(1280, 720, "OpenGL App") {
}

App::App(int width, int height, const char *title) {
  if (!glfwInit()) {
    LOG_ERROR("GLFW init failed.");
    return;
  }

  glfwWindowHint(GLFW_SAMPLES, 9);

  window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!window_) {
    LOG_ERROR("GLFWCreateWindow failed.");
    return;
  }

  glfwMakeContextCurrent(window_);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERROR("glad init failed.");
    return;
  }

  glfwSetWindowUserPointer(window_, this);

  glfwSetCursorPosCallback(
      window_, [](GLFWwindow *window, double xpos, double ypos) {
        auto *app = static_cast<App *>(glfwGetWindowUserPointer(window));
        if (!app->listeners_.empty()) {
          for (auto listener : app->listeners_) {
            listener->OnCursorPos(xpos, ypos);
          }
        }
      });

  glfwSetCursorEnterCallback(window_, [](GLFWwindow *window, int val) {
    auto *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (!app->listeners_.empty()) {
      for (auto listener : app->listeners_) {
        listener->OnCursorEnter(val);
      }
    }
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow *window, int mouse_button, int state, int mods) {
        auto *app = static_cast<App *>(glfwGetWindowUserPointer(window));
        if (!app->listeners_.empty()) {
          for (auto listener : app->listeners_) {
            listener->OnMouseButton(mouse_button, state, mods);
          }
        }
      });

  glfwSetWindowSizeCallback(
      window_, [](GLFWwindow *window, int width, int height) {
        auto *app = static_cast<App *>(glfwGetWindowUserPointer(window));
        if (!app->listeners_.empty()) {
          for (auto listener : app->listeners_) {
            listener->OnWindowSize(width, height);
          }
        }
        LOG_INFO("{} {}", width, height);
      });

  OpenGLCall(glEnable, GL_MULTISAMPLE);
  OpenGLCall(glEnable, GL_BLEND);
  OpenGLCall(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

App::~App() {
  glfwTerminate();
}

int App::GetWidth() const {
  int width;
  glfwGetWindowSize(window_, &width, nullptr);
  return width;
}

int App::GetHeight() const {
  int height;
  glfwGetWindowSize(window_, nullptr, &height);
  return height;
}

GLFWwindow *App::GetHandle() const {
  return window_;
}

void App::OnInit() {
}

void App::OnLoop() {
}

void App::OnTerminate() {
}

void App::Run() {
  OnInit();
  while (!glfwWindowShouldClose(window_)) {
    OnLoop();
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }
  OnTerminate();
}

glm::mat3 App::GetWorldToScreen() const {
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  return {float(width) * 0.5f,
          0.0f,
          0.0,
          0.0f,
          -float(height) * 0.5f,
          0.0,
          float(width) * 0.5f,
          float(height) * 0.5f,
          1.0f};
}

glm::mat3 App::GetScreenToWorld() const {
  return glm::inverse(GetWorldToScreen());
}

void App::RegisterListener(Listener *listener) {
  listeners_.insert(listener);
}

void App::UnregisterListener(Listener *listener) {
  if (listeners_.count(listener)) {
    listeners_.erase(listener);
  } else {
    LOG_ERROR("Unregister non-existed listener.");
  }
}
}  // namespace opengl
