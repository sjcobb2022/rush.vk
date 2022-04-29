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
                                                                 void *);
}
