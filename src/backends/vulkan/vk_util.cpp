#include "vk_util.hpp"

// const char *to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s)
// {
//     if (s == 7)
//         return "General | Validation | Performance";
//     if (s == 6)
//         return "Validation | Performance";
//     if (s == 5)
//         return "General | Performance";
//     if (s == 4 /*VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/)
//         return "Performance";
//     if (s == 3)
//         return "General | Validation";
//     if (s == 2 /*VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT*/)
//         return "Validation";
//     if (s == 1 /*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT*/)
//         return "General";
//     return "Unknown";
// }

namespace rush
{

    // VkResult create_debug_utils_messenger(VkInstance instance,
    //                                       PFN_vkDebugUtilsMessengerCallbackEXT debug_callback,
    //                                       VkDebugUtilsMessageSeverityFlagsEXT severity,
    //                                       VkDebugUtilsMessageTypeFlagsEXT type,
    //                                       void *user_data_pointer,
    //                                       VkDebugUtilsMessengerEXT *pDebugMessenger,
    //                                       VkAllocationCallbacks *allocation_callbacks)
    // {

    //     if (debug_callback == nullptr)
    //         debug_callback = default_debug_callback;
    //     VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
    //     messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    //     messengerCreateInfo.pNext = nullptr;
    //     messengerCreateInfo.messageSeverity = severity;
    //     messengerCreateInfo.messageType = type;
    //     messengerCreateInfo.pfnUserCallback = debug_callback;
    //     messengerCreateInfo.pUserData = user_data_pointer;

    //     auto createMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    //         instance,
    //         "vkCreateDebugUtilsMessengerEXT");

    //     if (createMessengerFunc != nullptr)
    //     {
    //         return createMessengerFunc(instance, &messengerCreateInfo, allocation_callbacks, pDebugMessenger);
    //     }
    //     else
    //     {
    //         return VK_ERROR_EXTENSION_NOT_PRESENT;
    //     }
    // }

    // void destroyDebugUtilsMessengerEXT(
    //     VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, VkAllocationCallbacks *allocation_callbacks)
    // {

    //     auto deleteMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    //         instance,
    //         "vkDestroyDebugUtilsMessengerEXT");

    //     if (deleteMessengerFunc != nullptr)
    //     {
    //         deleteMessengerFunc(instance, debugMessenger, allocation_callbacks);
    //     }
    // }

}
