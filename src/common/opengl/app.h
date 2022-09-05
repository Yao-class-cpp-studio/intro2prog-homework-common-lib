#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <set>

#include "common/opengl/listener.h"
#include "common/opengl/util.h"

namespace opengl {
class App {
 public:
  App();

  App(int width, int height, const char *title);

  virtual ~App();

  [[nodiscard]] int GetWidth() const;

  [[nodiscard]] int GetHeight() const;

  [[nodiscard]] GLFWwindow *GetHandle() const;

  [[nodiscard]] glm::mat3 GetWorldToScreen() const;

  [[nodiscard]] glm::mat3 GetScreenToWorld() const;

  virtual void OnInit();

  virtual void OnLoop();

  virtual void OnTerminate();

  void Run();

  void RegisterListener(Listener *listener);

  void UnregisterListener(Listener *listener);

 protected:
  GLFWwindow *window_{nullptr};
  std::set<Listener *> listeners_{};
};
}  // namespace opengl
