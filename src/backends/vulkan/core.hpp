#pragma once

#include "device.hpp"

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

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

    private:

        void createInstanceVk(bool enableValidationLayers = true);
        VkInstance m_Instance;
        // rush::device::deviceInfo device;
    };
}