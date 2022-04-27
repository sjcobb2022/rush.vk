#include "core.hpp"

#include "device.hpp"
#include "vkinstance.hpp"

// std headers
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>



// #include "vk_mem_alloc.h"

namespace rush
{

    static VKAPI_ATTR VkBool32 VKAPI_CALL validationLayerDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        // spdlog::
        // util::VkSeveritySpdlog[messageSeverity]("pCallbackData->pMessage");
        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            spdlog::debug(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            spdlog::info(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            spdlog::warn(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            spdlog::error(pCallbackData->pMessage);
        default:
            // only other case is VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT
            spdlog::critical(pCallbackData->pMessage);
            break;
        }

        // std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    Core::Core(GLFWwindow *window)
    {
        // device = rush::device::createDevice(window);
        rush::InstanceBuilder builder;
        auto instance = builder.set_app_name ("Example Vulkan Application")
                        .request_validation_layers ()
                        .build ();
    };

    Core::~Core()
    {
        rush::device::cleanup(&device);
    };

} // namespace rush