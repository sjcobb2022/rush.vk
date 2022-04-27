#pragma once

// libs
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

// std
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <set>
#include <unordered_set>

// forward decleration
const char *to_string_message_severity(VkDebugUtilsMessageSeverityFlagBitsEXT s);
const char *to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s);

namespace rush
{

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

    struct Instance
    {
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
        VkAllocationCallbacks *allocation_callbacks = VK_NULL_HANDLE;

    private:
        bool supports_properties2_ext = false;
        uint32_t instance_version = VK_API_VERSION_1_0;
        uint32_t api_version = VK_API_VERSION_1_0;

        friend class InstanceBuilder;
    };

    class InstanceBuilder
    {
    public:
        explicit InstanceBuilder();

        Instance &build() const;

        InstanceBuilder &set_app_name(const char *app_name);
        InstanceBuilder &set_engine_name(const char *engine_name);

        InstanceBuilder &set_app_version(uint32_t major, uint32_t minor, uint32_t patch);
        InstanceBuilder &set_engine_version(uint32_t major, uint32_t minor, uint32_t patch);

        InstanceBuilder &enable_layer(const char *layer);
        InstanceBuilder &enable_extension(const char *ext);

        InstanceBuilder &request_validation_layers(const bool enable_layers);
        InstanceBuilder &enable_validation_layers(const bool enable_layers);

        /**
         * @brief Set the debug callback function
         *
         * @param callback
         * @return InstanceBuilder&
         */
        InstanceBuilder &set_debug_callback(PFN_vkDebugUtilsMessengerCallbackEXT callback);

        InstanceBuilder &add_validation_feature_enable(VkValidationFeatureEnableEXT enable);

        InstanceBuilder &set_allocation_callbacks(VkAllocationCallbacks *callbacks);

    private:
        struct Info
        {
            const char *application_name;
            uint32_t application_v;
            const char *engine_name;
            uint32_t engine_v;
            uint32_t api_v;

            std::vector<const char *> layers;
            std::vector<const char *> extensions;
            VkInstanceCreateFlags flags = static_cast<VkInstanceCreateFlags>(0);

            PFN_vkDebugUtilsMessengerCallbackEXT debug_callback = default_debug_callback;

            void *debug_user_data_pointer = nullptr;

            VkAllocationCallbacks *allocation_callbacks = VK_NULL_HANDLE;

            bool request_validation_layers = false;
            bool enable_validation_layers = false;
            bool use_debug_messenger = false;

            VkDebugUtilsMessageSeverityFlagsEXT debug_message_severity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

            VkDebugUtilsMessageTypeFlagsEXT debug_message_type =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

            PFN_vkGetInstanceProcAddr fp_vkGetInstanceProcAddr = nullptr;

        } info;
    };

    VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                          const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                          void *pUserData);

    void destroy_debug_utils_messenger(
        VkInstance const instance, VkDebugUtilsMessengerEXT const messenger, VkAllocationCallbacks *allocation_callbacks = nullptr);

}