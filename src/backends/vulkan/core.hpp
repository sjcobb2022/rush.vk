#pragma once

#include "device.hpp"

#include <vulkan/vulkan.h>

// std
#include <string>
#include <vector>

namespace rush
{

    class Core
    {

    public:
        Core(GLFWwindow *window);
        ~Core();
    
    };
}