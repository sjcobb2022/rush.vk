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

    private:
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#if (__APPLE__)
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#else
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

        void createInstanceVk(bool enableValidationLayers = true);
        VkInstance m_Instance;
        rush::device::deviceInfo device;
    };
}