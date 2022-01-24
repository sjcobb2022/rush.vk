#include "extensions.hpp"

// std headers
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#include <GLFW/glfw3.h>

// #define VMA_IMPLEMENTATION
// #include "vk_mem_alloc.h"

namespace rush
{
    namespace vkext
    {

        std::vector<const char *> getExtensions(bool enableValidationLayers)
        {
            uint32_t glfwExtensionCount = 0;
            const char **glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#if (__APPLE__)
            extensions.push_back("VK_KHR_get_physical_device_properties2");
#endif

            if (true)
            { // TODO: set this to a boolean to disable validation layers
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        };

        void hasGflwRequiredInstanceExtensions()
        {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            std::cout << "available extensions:" << std::endl;
            std::unordered_set<std::string> available;
            for (const auto &extension : extensions)
            {
                std::cout << "\t" << extension.extensionName << std::endl;
                available.insert(extension.extensionName);
            }

            std::cout << "required extensions:" << std::endl;
            auto requiredExtensions = getExtensions(false);
            for (const auto &required : requiredExtensions)
            {
                std::cout << "\t" << required << std::endl;
                if (available.find(required) == available.end())
                {
                    throw std::runtime_error("Missing required glfw extension");
                }
            }
        };

    }
}