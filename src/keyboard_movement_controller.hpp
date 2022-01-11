#pragma once

#include "volk_game_object.hpp"
#include "lve_window.hpp"

//lib
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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

  void moveMouse(GLFWwindow* window, float dt, LveGameObject& gameObject, double x, double y);

  void moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject);

  // void mouseMoveIn(GLFWwindow* window, float dt, LveGameObject& gameObject, double dxM, double dyM);

  KeyMappings keys{};
  float moveSpeed{3.f};
  float lookSpeed{1.0f};
  float mouseLookSpeed{1.5f};
};
}  // namespace lve