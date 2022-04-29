#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

#include <vector>

namespace rush
{

    struct SystemInfo
    {
        std::vector<VkLayerProperties> layers;
        std::vector<VkExtensionProperties> extensions;

        bool validation_layers_available;
        bool debug_utils_available;
    };

    SystemInfo get_system_info();

    bool check_extension_supported(std::vector<VkExtensionProperties> const &available_extensions, const char *extension_name);

    bool check_extensions_supported(std::vector<VkExtensionProperties> const &available_extensions,std::vector<const char *> const &extension_names);

};