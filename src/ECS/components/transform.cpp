#include "transform.hpp"

namespace rush
{

    glm::mat4 c_Transform::mat4()
    {
        return (
            glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation) *
            glm::scale(glm::mat4(1.0f), scale));
    }

    glm::mat3 c_Transform::normalMatrix()
    {
        return glm::toMat3(rotation) * glm::mat3(glm::scale(glm::mat4{1.0f}, 1.0f / scale));
    }

}