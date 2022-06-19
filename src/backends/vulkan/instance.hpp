#pragma once

#include "vk_util.hpp"

// libs
#include "rush_pch.hpp"


namespace rush
{
    class PhysicalDeviceBuilder; //fwd declaration

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
        friend class PhysicalDeviceBuilder;
    };

    class InstanceBuilder
    {
    public:
        explicit InstanceBuilder();

        Instance* build() const;

        InstanceBuilder &set_app_name(const char *app_name);
        InstanceBuilder &set_engine_name(const char *engine_name);

        InstanceBuilder &set_app_version(uint32_t major, uint32_t minor, uint32_t patch);
        InstanceBuilder &set_engine_version(uint32_t major, uint32_t minor, uint32_t patch);

        InstanceBuilder &enable_layer(const char *layer);
        InstanceBuilder &enable_extension(const char *ext);

        InstanceBuilder &request_validation_layers(const bool enable_layers = true);
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
            uint32_t application_v = 0;
            const char *engine_name = "Rush Engine";
            uint32_t engine_v = 0;
            uint32_t api_v = VK_API_VERSION_1_0;

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
}