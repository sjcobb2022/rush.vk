
#pragma once
#include "rush_pch.hpp"

namespace rush
{

    struct c_Transform
    {
        glm::vec3 translation;
        glm::vec3 scale;
        glm::quat rotation;

        glm::mat4 mat4();

        glm::mat3 normalMatrix();
    };

}