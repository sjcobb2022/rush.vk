#include "vk_util.hpp"

const char *to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s)
{
    if (s == 7)
        return "General | Validation | Performance";
    if (s == 6)
        return "Validation | Performance";
    if (s == 5)
        return "General | Performance";
    if (s == 4 /*VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/)
        return "Performance";
    if (s == 3)
        return "General | Validation";
    if (s == 2 /*VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT*/)
        return "Validation";
    if (s == 1 /*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT*/)
        return "General";
    return "Unknown";
}

namespace rush
{

    VertexInputDescription Vertex::get_vertex_description()
    {
        VertexInputDescription description;
        VkVertexInputBindingDescription binding = {};

        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        description.bindings.push_back(binding);

        VkVertexInputAttributeDescription positionAttribute = {};

        positionAttribute.binding = 0;
        positionAttribute.location = 0;
        positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        positionAttribute.offset = offsetof(Vertex, position);

        VkVertexInputAttributeDescription normalAttribute = {};
        normalAttribute.binding = 0;
        normalAttribute.location = 1;
        normalAttribute.format = VK_FORMAT_R8G8_UNORM; // VK_FORMAT_R32G32B32_SFLOAT;
        normalAttribute.offset = offsetof(Vertex, oct_normal);

        VkVertexInputAttributeDescription colorAttribute = {};
        colorAttribute.binding = 0;
        colorAttribute.location = 2;
        colorAttribute.format = VK_FORMAT_R8G8B8_UNORM; // VK_FORMAT_R32G32B32_SFLOAT;
        colorAttribute.offset = offsetof(Vertex, color);

        VkVertexInputAttributeDescription uvAttribute = {};
        uvAttribute.binding = 0;
        uvAttribute.location = 3;
        uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
        uvAttribute.offset = offsetof(Vertex, uv);

        description.attributes.push_back(positionAttribute);
        description.attributes.push_back(normalAttribute);
        description.attributes.push_back(colorAttribute);
        description.attributes.push_back(uvAttribute);
        return description;
    }

    VkResult create_debug_utils_messenger(VkInstance instance,
                                          PFN_vkDebugUtilsMessengerCallbackEXT debug_callback,
                                          VkDebugUtilsMessageSeverityFlagsEXT severity,
                                          VkDebugUtilsMessageTypeFlagsEXT type,
                                          void *user_data_pointer,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger,
                                          VkAllocationCallbacks *allocation_callbacks)
    {

        if (debug_callback == nullptr)
            debug_callback = default_debug_callback;
        VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
        messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        messengerCreateInfo.pNext = nullptr;
        messengerCreateInfo.messageSeverity = severity;
        messengerCreateInfo.messageType = type;
        messengerCreateInfo.pfnUserCallback = debug_callback;
        messengerCreateInfo.pUserData = user_data_pointer;

        auto createMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugUtilsMessengerEXT");

        if (createMessengerFunc != nullptr)
        {
            return createMessengerFunc(instance, &messengerCreateInfo, allocation_callbacks, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void destroyDebugUtilsMessengerEXT(
        VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, VkAllocationCallbacks *allocation_callbacks)
    {

        auto deleteMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugUtilsMessengerEXT");

        if (deleteMessengerFunc != nullptr)
        {
            deleteMessengerFunc(instance, debugMessenger, allocation_callbacks);
        }
    }
}
