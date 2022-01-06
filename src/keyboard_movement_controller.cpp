#include "keyboard_movement_controller.hpp"

// std
#include <limits>

namespace lve {

void KeyboardMovementController::moveInPlaneXZ(
    GLFWwindow* window, float dt, LveGameObject& gameObject) {
  TransformComponent gameObjectTransform = gameObject.getComponent<TransformComponent>();

  glm::vec3 rotate{0};
  if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
  if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
  if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
  if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

  // rotate.y += dx;
  // rotate.x -= dy;


  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObjectTransform.rotation += lookSpeed * dt * glm::normalize(rotate);
  }
  // spdlog::info("{}  :  {}", dx, dy);

  // horizontalAngle += lookSpeed * dt * dx;
  // verticalAngle += lookSpeed * dy * dy;

  // glm::vec3 rotation = {
  // glm::cos(verticalAngle) * glm::sin(horizontalAngle),
  // glm::sin(verticalAngle),
  // glm::cos(verticalAngle) * glm::cos(horizontalAngle)
  // };

  // glm::vec3 forwardDir{
  //     glm::cos(verticalAngle) * glm::sin(horizontalAngle),
  //     glm::sin(verticalAngle),
  //     glm::cos(verticalAngle) * glm::cos(horizontalAngle)};

  // glm::vec3 rightDir = glm::vec3(
  //     glm::sin(horizontalAngle - 3.14f / 2.0f),
  //     0,
  //     glm::cos(horizontalAngle - 3.14f / 2.0f));

  // glm::vec3 upDir = glm::cross(rightDir, forwardDir);

  // const glm::vec3 rightDir =
  // glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));

  // const glm::vec3 upDir = glm::cross(rightDir, forwardDir);

  // spdlog::info("{}", direction.x);

  // limit pitch values between about +/- 85ish degrees
  gameObjectTransform.rotation.x = glm::clamp(gameObjectTransform.rotation.x, -1.5f, 1.5f);
  gameObjectTransform.rotation.y = glm::mod(gameObjectTransform.rotation.y, glm::two_pi<float>());

  float yaw = gameObjectTransform.rotation.y;
  const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
  const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
  const glm::vec3 upDir{0.f, -1.f, 0.f};

  glm::vec3 moveDir{0.f};
  if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
  if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
  if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
  if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
  if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
  if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    gameObjectTransform.translation += moveSpeed * dt * glm::normalize(moveDir);
  }

  gameObject.modifyComponent<TransformComponent>(gameObjectTransform);
}
}  // namespace lve