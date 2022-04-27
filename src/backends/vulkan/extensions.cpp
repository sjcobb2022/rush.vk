#include "extensions.hpp"

namespace rush
{

    SystemInfo get_system_info()
    {
        // GET AVAILABLE LAYERS
        bool validation_layers_available = false;
        bool debug_utils_available = false;
        uint32_t layerCount = 0;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> layers(layerCount);
        if (vkEnumerateInstanceLayerProperties(&layerCount, layers.data()) != VK_SUCCESS)
        {
            layers.clear();
            spdlog::critical("Failed to get any layers, aborting");
            throw std::runtime_error("Failed to get any layers, aborting");
        }

        spdlog::debug("Available layers: ");
        for (auto &layer : layers)
        {
            spdlog::debug("\t{}", layer.layerName);
            if (strcmp(layer.layerName, "VK_LAYER_KHRONOS_validation") == 0)
            {
                validation_layers_available = true;
            }
        }

        // GET AVAILABLE EXTENSIONS
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        spdlog::debug("Available extensions: ");
        for (const auto &extension : extensions)
        {
            spdlog::debug("\t{}", extension.extensionName);
            if (strcmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
            {
                debug_utils_available = true;
            }
        }

        for (auto &layer : layers)
        {
            std::vector<VkExtensionProperties> layer_extensions;
            uint32_t layerExtensionCount = 0;
            vkEnumerateInstanceExtensionProperties(layer.layerName, &layerExtensionCount, nullptr);
            layer_extensions.resize(layerExtensionCount);

            if (vkEnumerateInstanceExtensionProperties(layer.layerName, &layerExtensionCount, layer_extensions.data()) == VK_SUCCESS)
            {
                extensions.insert(extensions.end(), layer_extensions.begin(), layer_extensions.end()); // add trailing extensions
            }

            for (auto &ext : layer_extensions)
            {
                if (strcmp(ext.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
                {
                    debug_utils_available = true;
                }
            }
        };

        return SystemInfo{layers, extensions, validation_layers_available, debug_utils_available};
    }

    bool check_extension_supported(
        std::vector<VkExtensionProperties> const &available_extensions,
        const char *extension_name)
    {
        if (!extension_name)
            return false;

        for (const auto &extension_properties : available_extensions)
        {
            if (strcmp(extension_name, extension_properties.extensionName) == 0)
            {
                return true;
            }
        }

        return false;
    }

    bool check_extensions_supported(
        std::vector<VkExtensionProperties> const &available_extensions,
        std::vector<const char *> const &extension_names)
    {
        bool all_found = true;
        for (const auto &extension_name : extension_names)
        {
            bool found = check_extension_supported(available_extensions, extension_name);
            if (!found)
                all_found = false;
        }
        return all_found;
    }
}