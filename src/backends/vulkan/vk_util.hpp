#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

const char *to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s);

namespace rush
{

     VkResult create_debug_utils_messenger(VkInstance instance,
                                          PFN_vkDebugUtilsMessengerCallbackEXT debug_callback,
                                          VkDebugUtilsMessageSeverityFlagsEXT severity,
                                          VkDebugUtilsMessageTypeFlagsEXT type,
                                          void *user_data_pointer,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger,
                                          VkAllocationCallbacks *allocation_callbacks);

    void destroy_debug_utils_messenger(VkInstance const instance, VkDebugUtilsMessengerEXT const messenger, VkAllocationCallbacks *allocation_callbacks = nullptr);

    template <typename T>
    void setup_pNext_chain(T &structure, std::vector<VkBaseOutStructure *> const &structs)
    {
        structure.pNext = nullptr;
        if (structs.size() <= 0)
            return;
        for (size_t i = 0; i < structs.size() - 1; i++)
        {
            structs.at(i)->pNext = structs.at(i + 1);
        }
        structure.pNext = structs.at(0);
    }

    inline VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                                 void *)
    {
        const char *s_message_type = to_string_message_type(messageType);

        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            spdlog::debug(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            spdlog::info(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            spdlog::warn(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            spdlog::error(" {} :: {} ", s_message_type, pCallbackData->pMessage);
        default:
            // only other case is VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT
            spdlog::critical(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        }

        return VK_FALSE;
    }
}
