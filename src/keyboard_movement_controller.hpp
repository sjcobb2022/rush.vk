#pragma once

#include "volk_game_object.hpp"
#include "lve_window.hpp"

//lib
#include <glm/glm.hpp>

namespace volk {
class KeyboardMovementController {
 public:
  struct KeyMappings {
    int moveLeft = GLFW_KEY_A;
    int moveRight = GLFW_KEY_D;
    int moveForward = GLFW_KEY_W;
    int moveBackward = GLFW_KEY_S;
    int moveUp = GLFW_KEY_SPACE;
    int moveDown = GLFW_KEY_LEFT_SHIFT;
    int lookLeft = GLFW_KEY_LEFT;
    int lookRight = GLFW_KEY_RIGHT;
    int lookUp = GLFW_KEY_UP;
    int lookDown = GLFW_KEY_DOWN;
  };

  void moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject);

  // void mouseMoveIn(GLFWwindow* window, float dt, LveGameObject& gameObject, double dxM, double dyM);

  KeyMappings keys{};
  float moveSpeed{3.f};
  float lookSpeed{1.5f};
  glm::vec3 rotation_state{0};
};
}  // namespace lve