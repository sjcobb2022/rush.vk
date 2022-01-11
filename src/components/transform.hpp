
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace volk {
struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::quat rotation{glm::vec3{0.f}};

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix

  glm::mat4 mat4();

  glm::mat3 normalMatrix();

  void rotate(float x, float y, float z);

  void rotate(glm::vec3 eulerAngles);
};
}  // namespace volk