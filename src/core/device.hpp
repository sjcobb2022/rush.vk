#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

namespace rush
{

    namespace device
    {

        struct deviceInfo
        {

            VkInstance Instance;
            VkDebugUtilsMessengerEXT DebugMessenger;
            VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
            GLFWwindow *Window;
            VkCommandPool CommandPool;

            VkDevice Device;
            VkSurfaceKHR Surface;
            VkQueue GraphicsQueue;
            VkQueue PresentQueue;

            const std::vector<const char *> ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

#if (__APPLE__)
            const std::vector<const char *> DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#else
            const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif
        };

        deviceInfo createDevice(GLFWwindow *window);

        std::vector<const char *> getExtensions(bool enableValidationLayers);
        void hasGflwRequiredInstanceExtensions();


        VkInstance createVkInstance(bool enableValidationLayers, const std::vector<const char *> ValidationLayers = {});


        VkSurfaceKHR createSurface(bool enableValidationLayers);

        VkDebugUtilsMessengerEXT createDebugMessenger(VkInstance instance);

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                              const VkAllocationCallbacks *pAllocator,
                                              VkDebugUtilsMessengerEXT *pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks *pAllocator);

        VkSurfaceKHR createSurface(GLFWwindow *window, VkInstance instance);

    }
}