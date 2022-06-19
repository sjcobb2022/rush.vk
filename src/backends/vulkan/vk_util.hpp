#pragma once

#include "rush_pch.hpp"

const char *to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s);

namespace rush
{
    const uint32_t QUEUE_INDEX_MAX_VALUE = 65536;

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
            // spdlog::debug(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            // spdlog::info(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            // spdlog::warn(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            // spdlog::error(" {} :: {} ", s_message_type, pCallbackData->pMessage);
        default:
            // only other case is VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT
            // spdlog::critical(" {} :: {} ", s_message_type, pCallbackData->pMessage);
            break;
        }

        // has  to return vk false otherwise it errors.

        return VK_FALSE;
    }

    struct GenericFeaturesPNextNode
    {

        static const uint32_t field_capacity = 256;

        GenericFeaturesPNextNode();

        template <typename T>
        GenericFeaturesPNextNode(T const &features) noexcept
        {
            memset(fields, UINT8_MAX, sizeof(VkBool32) * field_capacity);
            memcpy(this, &features, sizeof(T));
        }

        static bool match(GenericFeaturesPNextNode const &requested, GenericFeaturesPNextNode const &supported) noexcept;

        VkStructureType sType = static_cast<VkStructureType>(0);
        void *pNext = nullptr;
        VkBool32 fields[field_capacity];
    };
}
