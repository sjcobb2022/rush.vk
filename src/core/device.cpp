#include "device.hpp"

#include <spdlog/spdlog.h>

// #define VMA_IMPLEMENTATION
// #include "vk_mem_alloc.h"

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <set>
#include <unordered_set>

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

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

rush::device::deviceInfo rush::device::createDevice(GLFWwindow *window)
{

    // spdlog::info("creates something i guess");

    rush::device::deviceInfo device;

    bool enableValidationLayers = true;

    device.Instance = rush::device::createVkInstance(enableValidationLayers, &device);

    if (enableValidationLayers)
    {
        device.DebugMessenger = rush::device::createDebugMessenger(&device);
    }

    device.Surface = rush::device::createSurface(window, device.Instance);

    device.PhysicalDevice = rush::device::choosePhysicalDevice(device.Instance, device.Surface, device.DeviceExtensions, &device.Properties);

    device.Device = rush::device::createLogicalDevice(&device);

    device.CommandPool = rush::device::createCommandPool(device.Device, device.PhysicalDevice, device.Surface);

    return device;

    // TODO: add to global deletion queue or something
};

VkInstance rush::device::createVkInstance(bool enableValidationLayers, rush::device::deviceInfo *device)
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Rush App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = rush::device::getExtensions(enableValidationLayers);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(device->ValidationLayers.size());
        createInfo.ppEnabledLayerNames = device->ValidationLayers.data();

        debugCreateInfo = {};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        debugCreateInfo.pfnUserCallback = validationLayerDebugCallback;
        debugCreateInfo.pUserData = nullptr;

        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VkInstance Instance;

    if (vkCreateInstance(&createInfo, nullptr, &Instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create vk instance");
    }

    rush::device::hasGflwRequiredInstanceExtensions();

    return Instance;
}

VkSurfaceKHR rush::device::createSurface(GLFWwindow *window, VkInstance instance)
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to craete window surface");
    }

    return surface;
}

VkPhysicalDevice rush::device::choosePhysicalDevice(VkInstance instance_, VkSurfaceKHR surface_, std::vector<const char *> deviceExtensions_, VkPhysicalDeviceProperties *properties_)
{
    VkPhysicalDevice physicalDevice;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    spdlog::debug("Device count {}", deviceCount);

    std::vector<VkPhysicalDevice> devices(deviceCount);

    vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        if (rush::device::isVkDeviceSuitable(device, surface_, deviceExtensions_))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    vkGetPhysicalDeviceProperties(physicalDevice, properties_);

    spdlog::debug("physical device: {}", properties_->deviceName);

    return physicalDevice;
}

// <----- EXTENSIONS ----->
std::vector<const char *> rush::device::getExtensions(bool enableValidationLayers)
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#if (__APPLE__)
    extensions.push_back("VK_KHR_get_physical_device_properties2");
#endif

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
};

void rush::device::hasGflwRequiredInstanceExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    spdlog::debug("available extensions: ");
    std::unordered_set<std::string> available;
    for (const auto &extension : extensions)
    {
        // std::cout << "\t" << extension.extensionName << std::endl;
        spdlog::debug("\t {}", extension.extensionName);
        available.insert(extension.extensionName);
    }

    spdlog::debug("required extensions: ");

    auto requiredExtensions = getExtensions(false);

    for (const auto &required : requiredExtensions)
    {
        spdlog::debug("\t {}", required);
        if (available.find(required) == available.end())
        {
            throw std::runtime_error("Missing required glfw extension");
        }
    }
};

// <----- DEBUG MESSENGER ----->
void rush::device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = validationLayerDebugCallback;
    createInfo.pUserData = nullptr; // Optional
}

VkDebugUtilsMessengerEXT rush::device::createDebugMessenger(deviceInfo *device)
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    rush::device::populateDebugMessengerCreateInfo(createInfo);

    VkDebugUtilsMessengerEXT debugMessenger;
    if (rush::device::CreateDebugUtilsMessengerEXT(device->Instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }

    return debugMessenger;
}

VkResult rush::device::CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance,
        "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void rush::device::DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance,
        "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

bool rush::device::isVkDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface_, std::vector<const char *> deviceExtensions_)
{
    rush::device::QueueFamilyIndices indices = rush::device::findQueueFamilies(physicalDevice, surface_);

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions_);

    bool swapChainAdequate = false;

    if (extensionsSupported)
    {
        rush::device::SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface_);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

    return indices.presentFamilyHasValue && indices.graphicsFamilyHasValue && extensionsSupported && swapChainAdequate &&
           supportedFeatures.samplerAnisotropy;
}

bool rush::device::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, std::vector<const char *> deviceExtensions_)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        physicalDevice,
        nullptr,
        &extensionCount,
        availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions_.begin(), deviceExtensions_.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

rush::device::QueueFamilyIndices rush::device::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface_)
{
    rush::device::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;

    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
            indices.graphicsFamilyHasValue = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface_, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
            indices.presentFamilyHasValue = true;
        }
        if (indices.presentFamilyHasValue && indices.presentFamilyHasValue)
        {
            break;
        }

        i++;
    }

    return indices;
}

rush::device::SwapChainSupportDetails rush::device::querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface_)
{
    rush::device::SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface_, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice,
            surface_,
            &presentModeCount,
            details.presentModes.data());
    }

    return details;
}

VkDevice rush::device::createLogicalDevice(deviceInfo *deviceInfo, bool enableValidationLayers)
{
    VkDevice device_;

    rush::device::QueueFamilyIndices indices = findQueueFamilies(deviceInfo->PhysicalDevice, deviceInfo->Surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceInfo->DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceInfo->DeviceExtensions.data();

    // might not really be necessary anymore because device specific validation layers
    // have been deprecated
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(deviceInfo->ValidationLayers.size());
        createInfo.ppEnabledLayerNames = deviceInfo->ValidationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(deviceInfo->PhysicalDevice, &createInfo, nullptr, &device_) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device_, indices.graphicsFamily, 0, &deviceInfo->GraphicsQueue);
    vkGetDeviceQueue(device_, indices.presentFamily, 0, &deviceInfo->PresentQueue);

    return device_;
}

VkCommandPool rush::device::createCommandPool(VkDevice device_, VkPhysicalDevice physicalDevice_, VkSurfaceKHR surface_)
{
    VkCommandPool commandPool;

    rush::device::QueueFamilyIndices queueFamilyIndices = rush::device::findQueueFamilies(physicalDevice_, surface_);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    poolInfo.flags =
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    return commandPool;
}

void rush::device::cleanup(deviceInfo *device, bool enableValidationLayers)
{
    vkDestroyCommandPool(device->Device, device->CommandPool, nullptr);
    vkDestroyDevice(device->Device, nullptr);

    if (enableValidationLayers)
    {
        rush::device::DestroyDebugUtilsMessengerEXT(device->Instance, device->DebugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(device->Instance, device->Surface, nullptr);
    vkDestroyInstance(device->Instance, nullptr);
    device = nullptr;
}