#include "keyboard_movement_controller.hpp"

#include "utils.hpp"

// std
#include <limits>

glm::quat volk::clampQuatX(glm::quat q, float x) {
  q.x /= q.w;
  q.y /= q.w;
  q.z /= q.w;
  q.w = 1.0f;

  float angleX = 2.f * glm::atan(q.x);
  angleX = glm::clamp(angleX, -x, x);
  q.x = glm::tan(0.5f * angleX);

  float angleY = 2.f * glm::atan(q.y);
  angleY = glm::mod(angleY, glm::two_pi<float>());
  q.y = glm::tan(0.5f * angleY);

  float angleZ = 2.f * glm::atan(q.z);
  // angleZ = glm::clamp(angleZ, -bounds.z, bounds.z);
  q.z = glm::tan(0.5f * angleZ);

  return glm::normalize(q);
}

namespace volk {

void KeyboardMovementController::moveInPlaneXZ(
    GLFWwindow* window, float dt, LveGameObject& gameObject) {
  TransformComponent gameObjectTransform = gameObject.getComponent<TransformComponent>();

  glm::quat& rotation = gameObjectTransform.rotation;

  const glm::vec3 forwardDir{
      // forward vector:
      // x = 2 * (x*z + w*y)
      // y = 2 * (y*z - w*x)
      // z = 1 - 2 * (x*x + y*y)
      //  glm::cross(
      //  glm::vec3{
      2 * (rotation.x * rotation.z + rotation.w * rotation.y),
      2 * (rotation.y * rotation.z - rotation.w * rotation.x),
      1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y),
      //  glm::vec3{0.f, 0.f, -1.f}
      //  )
  };

  const glm::vec3 rightDir{
      // left vector
      // x = 1 - 2 * (y*y + z*z)
      // y = 2 * (x*y + w*z)
      // z = 2 * (x*z - w*y)
      1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z),
      2 * (rotation.x * rotation.y + rotation.w * rotation.z),
      2 * (rotation.x * rotation.z - rotation.w * rotation.y)
      // glm::cross(forwardDir, upDir)
      // -1.f, 0.f, 0.f
      // forwardDir.z, 0.f, -forwardDir.x
  };

  const glm::vec3 upDir{
      // up vector
      // x = 2 * (x * y - w * z),
      // y = 1 - 2 * (x * x + z * z),
      // z = 2 * (y * z + w * x)
      glm::cross(rightDir, forwardDir)  // can do this instead
                                        // 0.f,
                                        // -1.f,
                                        // 0.f
  };

  // spdlog::info("{} : {} : {}", forwardDir.x, forwardDir.y, forwardDir.z);

  glm::vec3 moveDir{0.f};
  if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
  if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
  if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
  if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
  if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
  if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;
  // spdlog::info("{} : {} : {}", moveDir.x, moveDir.y, moveDir.z);

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    gameObjectTransform.translation += moveSpeed * dt * glm::normalize(moveDir);
  }

  glm::vec3 rotate{0};
  if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
  if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
  if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
  if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObjectTransform.rotate(lookSpeed * dt * glm::normalize(rotate));

    gameObjectTransform.rotation = clampQuatX(gameObjectTransform.rotation, 1.5);

    // TODO
    //  glm::vec3 eulerAngles = glm::eulerAngles(gameObjectTransform.rotation);
    //  limit pitch values between about +/- 85ish degrees
    //  eulerAngles.x = glm::clamp(eulerAngles.x, -1.5f, 1.5f);
    //  eulerAngles.y = glm::mod(eulerAngles.y, glm::two_pi<float>());
    //  gameObjectTransform.rotation = glm::normalize(glm::quat(eulerAngles));
  }

  gameObject.modifyComponent<TransformComponent>(gameObjectTransform);
}

void KeyboardMovementController::moveMouse(
    GLFWwindow* window, float dt, LveGameObject& gameObject, double x, double y) {
  TransformComponent gameObjectTransform = gameObject.getComponent<TransformComponent>();

  // glm::vec3 rotate{0};

  // if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
  //   gameObjectTransform.rotation += mouseLookSpeed * dt * glm::normalize(rotate);
  // }

  // gameObjectTransform.rotation.x = glm::clamp(gameObjectTransform.rotation.x, -1.5f, 1.5f);
  // gameObjectTransform.rotation.y = glm::mod(gameObjectTransform.rotation.y,
  // glm::two_pi<float>());

  // gameObject.modifyComponent<TransformComponent>(gameObjectTransform);
}

}  // namespace volk