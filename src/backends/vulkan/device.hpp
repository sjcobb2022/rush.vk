// #pragma once

// #include <vulkan/vulkan.h>
// #include <GLFW/glfw3.h>

// #include <string>
// #include <vector>

namespace rush
{

//     namespace device
//     {

//         struct deviceInfo
//         {

//             VkInstance Instance;
//             VkDebugUtilsMessengerEXT DebugMessenger;
//             VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
//             GLFWwindow *Window;
//             VkCommandPool CommandPool;

//             VkDevice Device;
//             VkSurfaceKHR Surface;
//             VkQueue GraphicsQueue;
//             VkQueue PresentQueue;

//             VkPhysicalDeviceProperties Properties;

//             const std::vector<const char *> ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

// #if (__APPLE__)
//             const std::vector<const char *> DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
// #else
//             const std::vector<const char *> DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
// #endif
//         };

//         struct SwapChainSupportDetails
//         {
//             VkSurfaceCapabilitiesKHR capabilities;
//             std::vector<VkSurfaceFormatKHR> formats;
//             std::vector<VkPresentModeKHR> presentModes;
//         };

//         struct QueueFamilyIndices
//         {
//             uint32_t graphicsFamily;
//             uint32_t presentFamily;
//             bool graphicsFamilyHasValue = false;
//             bool presentFamilyHasValue = false;
//         };

//         deviceInfo createDevice(GLFWwindow *window);

//         std::vector<const char *> getExtensions(bool enableValidationLayers);
//         void hasGflwRequiredInstanceExtensions();

//         VkInstance createVkInstance(bool enableValidationLayers, deviceInfo *device);

//         VkSurfaceKHR createSurface(bool enableValidationLayers);

//         VkPhysicalDevice choosePhysicalDevice(VkInstance instance_, VkSurfaceKHR surface_, std::vector<const char *> deviceExtensions_, VkPhysicalDeviceProperties *properties_);

//         VkDebugUtilsMessengerEXT createDebugMessenger(deviceInfo *device);

//         void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

//         VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
//                                               const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
//                                               const VkAllocationCallbacks *pAllocator,
//                                               VkDebugUtilsMessengerEXT *pDebugMessenger);

//         VkSurfaceKHR createSurface(GLFWwindow *window, VkInstance instance);

//         // <---- HELPER FUNCTIONS---->
//         // TODO: put in new namespace
//         // maybe make into builder format
//         bool isVkDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface_, std::vector<const char *> deviceExtensions_);
//         bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, std::vector<const char *> deviceExtensions_);
//         SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface_);
//         QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface_);
//         VkCommandPool createCommandPool(VkDevice device_, VkPhysicalDevice physicalDevice_, VkSurfaceKHR surface_);
//         VkDevice createLogicalDevice(deviceInfo *deviceInfo, bool enableValidationLayers = true);

//         void DestroyDebugUtilsMessengerEXT(VkInstance instance,
//                                            VkDebugUtilsMessengerEXT debugMessenger,
//                                            const VkAllocationCallbacks *pAllocator);
        
    //     void cleanup(deviceInfo *device, bool enableValidationLayers = true);
    // }
}