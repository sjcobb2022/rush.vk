#include "physical_device.hpp"

namespace rush
{

    // Helper for robustly executing the two-call pattern
    template <typename T, typename F, typename... Ts>
    auto get_vector(std::vector<T> &out, F &&f, Ts &&...ts) -> VkResult
    {
        uint32_t count = 0;
        VkResult err;
        do
        {
            err = f(ts..., &count, nullptr);
            if (err != VK_SUCCESS)
            {
                return err;
            };
            out.resize(count);
            err = f(ts..., &count, out.data());
            out.resize(count);
        } while (err == VK_INCOMPLETE);
        return err;
    }

    template <typename T, typename F, typename... Ts>
    auto get_vector_noerror(F &&f, Ts &&...ts) -> std::vector<T>
    {
        uint32_t count = 0;
        std::vector<T> results;
        f(ts..., &count, nullptr);
        results.resize(count);
        f(ts..., &count, results.data());
        results.resize(count);
        return results;
    }

    GenericFeaturesPNextNode::GenericFeaturesPNextNode() { memset(fields, UINT8_MAX, sizeof(VkBool32) * field_capacity); }

    bool GenericFeaturesPNextNode::match(GenericFeaturesPNextNode const &requested, GenericFeaturesPNextNode const &supported) noexcept
    {
        assert(requested.sType == supported.sType && "Non-matching sTypes in features nodes!");
        for (uint32_t i = 0; i < field_capacity; i++)
        {
            if (requested.fields[i] && !supported.fields[i])
                return false;
        }
        return true;
    }

    std::vector<std::string> check_device_extension_support(
        std::vector<std::string> const &available_extensions, std::vector<std::string> const &desired_extensions)
    {
        std::vector<std::string> extensions_to_enable;
        for (const auto &avail_ext : available_extensions)
        {
            for (auto &req_ext : desired_extensions)
            {
                if (avail_ext == req_ext)
                {
                    extensions_to_enable.push_back(req_ext);
                    break;
                }
            }
        }
        return extensions_to_enable;
    }

    uint32_t get_first_queue_index(std::vector<VkQueueFamilyProperties> const &families, VkQueueFlags desired_flags)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(families.size()); i++)
        {
            if ((families[i].queueFlags & desired_flags) == desired_flags)
                return i;
        }
        return QUEUE_INDEX_MAX_VALUE;
    }

    uint32_t get_separate_queue_index(
        std::vector<VkQueueFamilyProperties> const &families, VkQueueFlags desired_flags, VkQueueFlags undesired_flags)
    {
        uint32_t index = QUEUE_INDEX_MAX_VALUE;
        for (uint32_t i = 0; i < static_cast<uint32_t>(families.size()); i++)
        {
            if ((families[i].queueFlags & desired_flags) == desired_flags && ((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
            {
                if ((families[i].queueFlags & undesired_flags) == 0)
                {
                    return i;
                }
                else
                {
                    index = i;
                }
            }
        }
        return index;
    }

    uint32_t get_dedicated_queue_index(
        std::vector<VkQueueFamilyProperties> const &families, VkQueueFlags desired_flags, VkQueueFlags undesired_flags)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(families.size()); i++)
        {
            if ((families[i].queueFlags & desired_flags) == desired_flags && (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 &&
                (families[i].queueFlags & undesired_flags) == 0)
                return i;
        }
        return QUEUE_INDEX_MAX_VALUE;
    }

    uint32_t get_present_queue_index(
        VkPhysicalDevice const phys_device, VkSurfaceKHR const surface, std::vector<VkQueueFamilyProperties> const &families)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(families.size()); i++)
        {
            VkBool32 presentSupport = false;
            if (surface != VK_NULL_HANDLE)
            {
                VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(phys_device, i, surface, &presentSupport);
                if (res != VK_SUCCESS)
                    return QUEUE_INDEX_MAX_VALUE; // TODO: determine if this should fail another way
            }
            if (presentSupport == VK_TRUE)
                return i;
        }
        return QUEUE_INDEX_MAX_VALUE;
    }

    bool supports_features(VkPhysicalDeviceFeatures supported,
                           VkPhysicalDeviceFeatures requested,
                           std::vector<GenericFeaturesPNextNode> const &extension_supported,
                           std::vector<GenericFeaturesPNextNode> const &extension_requested)
    {
        if (requested.robustBufferAccess && !supported.robustBufferAccess)
            return false;
        if (requested.fullDrawIndexUint32 && !supported.fullDrawIndexUint32)
            return false;
        if (requested.imageCubeArray && !supported.imageCubeArray)
            return false;
        if (requested.independentBlend && !supported.independentBlend)
            return false;
        if (requested.geometryShader && !supported.geometryShader)
            return false;
        if (requested.tessellationShader && !supported.tessellationShader)
            return false;
        if (requested.sampleRateShading && !supported.sampleRateShading)
            return false;
        if (requested.dualSrcBlend && !supported.dualSrcBlend)
            return false;
        if (requested.logicOp && !supported.logicOp)
            return false;
        if (requested.multiDrawIndirect && !supported.multiDrawIndirect)
            return false;
        if (requested.drawIndirectFirstInstance && !supported.drawIndirectFirstInstance)
            return false;
        if (requested.depthClamp && !supported.depthClamp)
            return false;
        if (requested.depthBiasClamp && !supported.depthBiasClamp)
            return false;
        if (requested.fillModeNonSolid && !supported.fillModeNonSolid)
            return false;
        if (requested.depthBounds && !supported.depthBounds)
            return false;
        if (requested.wideLines && !supported.wideLines)
            return false;
        if (requested.largePoints && !supported.largePoints)
            return false;
        if (requested.alphaToOne && !supported.alphaToOne)
            return false;
        if (requested.multiViewport && !supported.multiViewport)
            return false;
        if (requested.samplerAnisotropy && !supported.samplerAnisotropy)
            return false;
        if (requested.textureCompressionETC2 && !supported.textureCompressionETC2)
            return false;
        if (requested.textureCompressionASTC_LDR && !supported.textureCompressionASTC_LDR)
            return false;
        if (requested.textureCompressionBC && !supported.textureCompressionBC)
            return false;
        if (requested.occlusionQueryPrecise && !supported.occlusionQueryPrecise)
            return false;
        if (requested.pipelineStatisticsQuery && !supported.pipelineStatisticsQuery)
            return false;
        if (requested.vertexPipelineStoresAndAtomics && !supported.vertexPipelineStoresAndAtomics)
            return false;
        if (requested.fragmentStoresAndAtomics && !supported.fragmentStoresAndAtomics)
            return false;
        if (requested.shaderTessellationAndGeometryPointSize && !supported.shaderTessellationAndGeometryPointSize)
            return false;
        if (requested.shaderImageGatherExtended && !supported.shaderImageGatherExtended)
            return false;
        if (requested.shaderStorageImageExtendedFormats && !supported.shaderStorageImageExtendedFormats)
            return false;
        if (requested.shaderStorageImageMultisample && !supported.shaderStorageImageMultisample)
            return false;
        if (requested.shaderStorageImageReadWithoutFormat && !supported.shaderStorageImageReadWithoutFormat)
            return false;
        if (requested.shaderStorageImageWriteWithoutFormat && !supported.shaderStorageImageWriteWithoutFormat)
            return false;
        if (requested.shaderUniformBufferArrayDynamicIndexing && !supported.shaderUniformBufferArrayDynamicIndexing)
            return false;
        if (requested.shaderSampledImageArrayDynamicIndexing && !supported.shaderSampledImageArrayDynamicIndexing)
            return false;
        if (requested.shaderStorageBufferArrayDynamicIndexing && !supported.shaderStorageBufferArrayDynamicIndexing)
            return false;
        if (requested.shaderStorageImageArrayDynamicIndexing && !supported.shaderStorageImageArrayDynamicIndexing)
            return false;
        if (requested.shaderClipDistance && !supported.shaderClipDistance)
            return false;
        if (requested.shaderCullDistance && !supported.shaderCullDistance)
            return false;
        if (requested.shaderFloat64 && !supported.shaderFloat64)
            return false;
        if (requested.shaderInt64 && !supported.shaderInt64)
            return false;
        if (requested.shaderInt16 && !supported.shaderInt16)
            return false;
        if (requested.shaderResourceResidency && !supported.shaderResourceResidency)
            return false;
        if (requested.shaderResourceMinLod && !supported.shaderResourceMinLod)
            return false;
        if (requested.sparseBinding && !supported.sparseBinding)
            return false;
        if (requested.sparseResidencyBuffer && !supported.sparseResidencyBuffer)
            return false;
        if (requested.sparseResidencyImage2D && !supported.sparseResidencyImage2D)
            return false;
        if (requested.sparseResidencyImage3D && !supported.sparseResidencyImage3D)
            return false;
        if (requested.sparseResidency2Samples && !supported.sparseResidency2Samples)
            return false;
        if (requested.sparseResidency4Samples && !supported.sparseResidency4Samples)
            return false;
        if (requested.sparseResidency8Samples && !supported.sparseResidency8Samples)
            return false;
        if (requested.sparseResidency16Samples && !supported.sparseResidency16Samples)
            return false;
        if (requested.sparseResidencyAliased && !supported.sparseResidencyAliased)
            return false;
        if (requested.variableMultisampleRate && !supported.variableMultisampleRate)
            return false;
        if (requested.inheritedQueries && !supported.inheritedQueries)
            return false;

        for (size_t i = 0; i < extension_requested.size(); ++i)
        {
            auto res = GenericFeaturesPNextNode::match(extension_requested[i], extension_supported[i]);
            if (!res)
                return false;
        }

        return true;
    }

    PhysicalDevice PhysicalDeviceBuilder::populate_device_details(VkPhysicalDevice vk_phys_device,
                                                                  std::vector<GenericFeaturesPNextNode> const &src_extended_features_chain
#if !defined(VK_API_VERSION_1_1)
                                                                  ,
                                                                  VkInstance instance
#endif
    ) const
    {
        PhysicalDevice physical_device{};
        physical_device.physical_device = vk_phys_device;
        physical_device.surface = instance_info.surface;
        physical_device.defer_surface_initialization = criteria.defer_surface_initialization;
        physical_device.instance_version = instance_info.version;
        auto queue_families = get_vector_noerror<VkQueueFamilyProperties>(
            vkGetPhysicalDeviceQueueFamilyProperties, vk_phys_device);
        physical_device.queue_families = queue_families;

        vkGetPhysicalDeviceProperties(vk_phys_device, &physical_device.properties);
        vkGetPhysicalDeviceFeatures(vk_phys_device, &physical_device.features);
        vkGetPhysicalDeviceMemoryProperties(vk_phys_device, &physical_device.memory_properties);

        physical_device.name = physical_device.properties.deviceName;

        std::vector<VkExtensionProperties> available_extensions;
        auto available_extensions_ret = get_vector<VkExtensionProperties>(
            available_extensions, vkEnumerateDeviceExtensionProperties, vk_phys_device, nullptr);
        if (available_extensions_ret != VK_SUCCESS)
            return physical_device;
        for (const auto &ext : available_extensions)
        {
            physical_device.extensions.push_back(&ext.extensionName[0]);
        }

#if defined(VK_API_VERSION_1_1)
        physical_device.features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
#else
        physical_device.features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
#endif

        auto fill_chain = src_extended_features_chain;

        if (!fill_chain.empty() && (instance_info.version >= VK_API_VERSION_1_1 || instance_info.supports_properties2_ext))
        {

            GenericFeaturesPNextNode *prev = nullptr;
            for (auto &extension : fill_chain)
            {
                if (prev != nullptr)
                {
                    prev->pNext = &extension;
                }
                prev = &extension;
            }

#if defined(VK_API_VERSION_1_1)
            if (instance_info.version >= VK_API_VERSION_1_1 && physical_device.properties.apiVersion >= VK_API_VERSION_1_1)
            {
                VkPhysicalDeviceFeatures2 local_features{};
                local_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
                local_features.pNext = &fill_chain.front();
                vkGetPhysicalDeviceFeatures2(vk_phys_device, &local_features);
            }
            else
            {
                throw std::runtime_error("Please don't use a version of Vulkan less than 1.1");
            }
            // TODO: This is really annoying i need to getn function pointers but I don't want to pass a vkinstance, maybe just get the function pointers to begin with like vkb?
            //  else if (instance_info.supports_properties2_ext)
            //  {
            //      VkPhysicalDeviceFeatures2KHR local_features_khr{};
            //      local_features_khr.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
            //      local_features_khr.pNext = &fill_chain.front();
            //      auto fp_vkGetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)vkGetInstanceProcAddr(inst, "vkGetPhysicalDeviceFeatures2KHR");
            //      fp_vkGetPhysicalDeviceFeatures2KHR(vk_phys_device, &local_features_khr);
            //  }
#else
            VkPhysicalDeviceFeatures2KHR local_features_khr{};
            local_features_khr.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
            local_features_khr.pNext = &fill_chain.front();

            auto fp_vkGetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)vkGetInstanceProcAddr(inst, "vkGetPhysicalDeviceFeatures2KHR");

            if (instance_info.supports_properties2_ext)
            {
                fp_vkGetPhysicalDeviceFeatures2KHR(vk_phys_device, &local_features_khr);
            }
#endif
            physical_device.extended_features_chain = fill_chain;
        }

        return physical_device;
    }

    PhysicalDevice::Suitable PhysicalDeviceBuilder::is_device_suitable(PhysicalDevice const &pd) const
    {
        PhysicalDevice::Suitable suitable = PhysicalDevice::Suitable::yes;

        if (criteria.name.size() > 0 && criteria.name != pd.properties.deviceName)
            return PhysicalDevice::Suitable::no;

        if (criteria.required_version > pd.properties.apiVersion)
            return PhysicalDevice::Suitable::no;
        if (criteria.desired_version > pd.properties.apiVersion)
            suitable = PhysicalDevice::Suitable::partial;

        bool dedicated_compute = get_dedicated_queue_index(pd.queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT) !=
                                 QUEUE_INDEX_MAX_VALUE;
        bool dedicated_transfer = get_dedicated_queue_index(pd.queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT) !=
                                  QUEUE_INDEX_MAX_VALUE;
        bool separate_compute = get_separate_queue_index(pd.queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT) !=
                                QUEUE_INDEX_MAX_VALUE;
        bool separate_transfer = get_separate_queue_index(pd.queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT) !=
                                 QUEUE_INDEX_MAX_VALUE;

        bool present_queue = get_present_queue_index(pd.physical_device, instance_info.surface, pd.queue_families) !=
                             QUEUE_INDEX_MAX_VALUE;

        if (criteria.require_dedicated_compute_queue && !dedicated_compute)
            return PhysicalDevice::Suitable::no;
        if (criteria.require_dedicated_transfer_queue && !dedicated_transfer)
            return PhysicalDevice::Suitable::no;
        if (criteria.require_separate_compute_queue && !separate_compute)
            return PhysicalDevice::Suitable::no;
        if (criteria.require_separate_transfer_queue && !separate_transfer)
            return PhysicalDevice::Suitable::no;
        if (criteria.require_present && !present_queue && !criteria.defer_surface_initialization)
            return PhysicalDevice::Suitable::no;

        auto required_extensions_supported = check_device_extension_support(pd.extensions, criteria.required_extensions);
        if (required_extensions_supported.size() != criteria.required_extensions.size())
            return PhysicalDevice::Suitable::no;

        auto desired_extensions_supported = check_device_extension_support(pd.extensions, criteria.desired_extensions);
        if (desired_extensions_supported.size() != criteria.desired_extensions.size())
            suitable = PhysicalDevice::Suitable::partial;

        if (!criteria.defer_surface_initialization && criteria.require_present)
        {
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> present_modes;

            auto formats_ret = get_vector<VkSurfaceFormatKHR>(formats,
                                                              vkGetPhysicalDeviceSurfaceFormatsKHR,
                                                              pd.physical_device,
                                                              instance_info.surface);
            auto present_modes_ret = get_vector<VkPresentModeKHR>(present_modes,
                                                                  vkGetPhysicalDeviceSurfacePresentModesKHR,
                                                                  pd.physical_device,
                                                                  instance_info.surface);

            if (formats_ret != VK_SUCCESS || present_modes_ret != VK_SUCCESS || formats.empty() || present_modes.empty())
            {
                return PhysicalDevice::Suitable::no;
            }
        }

        if (!criteria.allow_any_type && pd.properties.deviceType != static_cast<VkPhysicalDeviceType>(criteria.preferred_type))
        {
            suitable = PhysicalDevice::Suitable::partial;
        }

        bool required_features_supported = supports_features(
            pd.features, criteria.required_features, pd.extended_features_chain, criteria.extended_features_chain);
        if (!required_features_supported)
            return PhysicalDevice::Suitable::no;

        for (uint32_t i = 0; i < pd.memory_properties.memoryHeapCount; i++)
        {
            if (pd.memory_properties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            {
                if (pd.memory_properties.memoryHeaps[i].size < criteria.required_mem_size)
                {
                    return PhysicalDevice::Suitable::no;
                }
                else if (pd.memory_properties.memoryHeaps[i].size < criteria.desired_mem_size)
                {
                    suitable = PhysicalDevice::Suitable::partial;
                }
            }
        }

        return suitable;
    }

    // delegate construction to the one with an explicit surface parameter
    PhysicalDeviceBuilder::PhysicalDeviceBuilder(Instance const &instance)
        : PhysicalDeviceBuilder(instance, VK_NULL_HANDLE) {}

    PhysicalDeviceBuilder::PhysicalDeviceBuilder(Instance const &instance, VkSurfaceKHR surface)
    {
        instance_info.instance = instance.instance;
        instance_info.version = instance.instance_version;
        instance_info.supports_properties2_ext = instance.supports_properties2_ext;
        instance_info.surface = surface;
        // criteria.require_present = !instance.headless;
        criteria.required_version = instance.api_version;
        criteria.desired_version = instance.api_version;
    }

    std::vector<PhysicalDevice> PhysicalDeviceBuilder::select_impl(DeviceSelectionMode selection) const
    {
#if !defined(NDEBUG)
        // Validation
        for (const auto &node : criteria.extended_features_chain)
        {
            assert(node.sType != static_cast<VkStructureType>(0) &&
                   "Features struct sType must be filled with the struct's "
                   "corresponding VkStructureType enum");
            assert(node.sType != VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 &&
                   "Do not pass VkPhysicalDeviceFeatures2 as a required extension feature structure. An "
                   "instance of this is managed internally for selection criteria and device creation.");
        }
#endif

        if (criteria.require_present && !criteria.defer_surface_initialization)
        {
            if (instance_info.surface == VK_NULL_HANDLE)
                throw std::runtime_error("no surface provided in select_impl");
            // return detail::Result<std::vector<PhysicalDevice>>{PhysicalDeviceError::no_surface_provided};
        }

        // Get the VkPhysicalDevice handles on the system
        std::vector<VkPhysicalDevice> vk_physical_devices;

        auto vk_physical_devices_ret = get_vector<VkPhysicalDevice>(
            vk_physical_devices, vkEnumeratePhysicalDevices, instance_info.instance);
        if (vk_physical_devices_ret != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to enumerate physical devices in select_impl");
            // return std::vector<PhysicalDevice>{PhysicalDeviceError::failed_enumerate_physical_devices,
            //                                                    vk_physical_devices_ret};
        }

        if (vk_physical_devices.size() == 0)
        {
            throw std::runtime_error("No physical devices found in select_impl");
            // return std::vector<PhysicalDevice>{PhysicalDeviceError::no_physical_devices_found};
        }

        auto fill_out_phys_dev_with_criteria = [&](PhysicalDevice &phys_dev)
        {
            phys_dev.features = criteria.required_features;
            phys_dev.extended_features_chain = criteria.extended_features_chain;
            bool portability_ext_available = false;
            for (const auto &ext : phys_dev.extensions)
                if (criteria.enable_portability_subset && ext == "VK_KHR_portability_subset")
                    portability_ext_available = true;

            phys_dev.extensions.clear();
            phys_dev.extensions.insert(
                phys_dev.extensions.end(), criteria.required_extensions.begin(), criteria.required_extensions.end());
            auto desired_extensions_supported = check_device_extension_support(phys_dev.extensions, criteria.desired_extensions);
            phys_dev.extensions.insert(
                phys_dev.extensions.end(), desired_extensions_supported.begin(), desired_extensions_supported.end());
            if (portability_ext_available)
            {
                phys_dev.extensions.push_back("VK_KHR_portability_subset");
            }
        };

        // if this option is set, always return only the first physical device found
        if (criteria.use_first_gpu_unconditionally && vk_physical_devices.size() > 0)
        {
            PhysicalDevice physical_device = populate_device_details(vk_physical_devices[0], criteria.extended_features_chain);
            fill_out_phys_dev_with_criteria(physical_device);
            return std::vector<PhysicalDevice>{physical_device};
        }

        // Populate their details and check their suitability
        std::vector<PhysicalDevice> physical_devices;
        for (auto &vk_physical_device : vk_physical_devices)
        {
            PhysicalDevice phys_dev = populate_device_details(vk_physical_device, criteria.extended_features_chain);
            phys_dev.suitable = is_device_suitable(phys_dev);
            if (phys_dev.suitable != PhysicalDevice::Suitable::no)
            {
                physical_devices.push_back(phys_dev);
            }
        }

        // sort the list into fully and partially suitable devices. use stable_partition to maintain relative order
        const auto partition_index = std::stable_partition(physical_devices.begin(), physical_devices.end(), [](auto const &pd)
                                                           { return pd.suitable == PhysicalDevice::Suitable::yes; });

        // Remove the partially suitable elements if they aren't desired
        if (selection == DeviceSelectionMode::only_fully_suitable)
        {
            physical_devices.erase(partition_index, physical_devices.end() - 1);
        }

        // Make the physical device ready to be used to create a Device from it
        for (auto &physical_device : physical_devices)
        {
            fill_out_phys_dev_with_criteria(physical_device);
        }

        return physical_devices;
    }

    PhysicalDevice PhysicalDeviceBuilder::select(DeviceSelectionMode selection) const
    {
        auto const selected_devices = select_impl(selection);

        // if (!selected_devices)
        //     throw std::runtime_error();
        // return PhysicalDevice{selected_devices.error()};
        if (selected_devices.size() == 0)
        {
            throw std::runtime_error("No suitable device found");
            // return detail::Result<PhysicalDevice>{PhysicalDeviceError::no_suitable_device};
        }

        return selected_devices.at(0);
    }

    // Return all devices which are considered suitable - intended for applications which want to let the user pick the physical device
    std::vector<PhysicalDevice> PhysicalDeviceBuilder::select_devices(DeviceSelectionMode selection) const
    {
        auto const selected_devices = select_impl(selection);
        // if (!selected_devices)
        //     return detail::Result<std::vector<PhysicalDevice>>{selected_devices.error()};
        if (selected_devices.size() == 0)
        {
            throw std::runtime_error("No suitable devices");
            // return detail::Result<std::vector<PhysicalDevice>>{PhysicalDeviceError::no_suitable_device};
        }
        return selected_devices;
    }

    std::vector<std::string> PhysicalDeviceBuilder::select_device_names(DeviceSelectionMode selection) const
    {
        auto const selected_devices = select_impl(selection);
        // if (!selected_devices)
        //     return detail::Result<std::vector<std::string>>{selected_devices.error()};
        if (selected_devices.size() == 0)
        {
            throw std::runtime_error("No suitable devices");
            // return detail::Result<std::vector<std::string>>{PhysicalDeviceError::no_suitable_device};
        }
        std::vector<std::string> names;
        for (const auto &pd : selected_devices)
        {
            names.push_back(pd.name);
        }
        return names;
    }

    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_surface(VkSurfaceKHR surface)
    {
        instance_info.surface = surface;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_name(std::string const &name)
    {
        criteria.name = name;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::prefer_gpu_device_type(PreferredDeviceType type)
    {
        criteria.preferred_type = type;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::allow_any_gpu_device_type(bool allow_any_type)
    {
        criteria.allow_any_type = allow_any_type;
        return *this;
    }

    PhysicalDeviceBuilder &PhysicalDeviceBuilder::require_present(bool require)
    {
        criteria.require_present = require;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::require_dedicated_transfer_queue()
    {
        criteria.require_dedicated_transfer_queue = true;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::require_dedicated_compute_queue()
    {
        criteria.require_dedicated_compute_queue = true;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::require_separate_transfer_queue()
    {
        criteria.require_separate_transfer_queue = true;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::require_separate_compute_queue()
    {
        criteria.require_separate_compute_queue = true;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::required_device_memory_size(VkDeviceSize size)
    {
        criteria.required_mem_size = size;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::desired_device_memory_size(VkDeviceSize size)
    {
        criteria.desired_mem_size = size;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::add_required_extension(const char *extension)
    {
        criteria.required_extensions.push_back(extension);
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::add_required_extensions(std::vector<const char *> extensions)
    {
        for (const auto &ext : extensions)
        {
            criteria.required_extensions.push_back(ext);
        }
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::add_desired_extension(const char *extension)
    {
        criteria.desired_extensions.push_back(extension);
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::add_desired_extensions(std::vector<const char *> extensions)
    {
        for (const auto &ext : extensions)
        {
            criteria.desired_extensions.push_back(ext);
        }
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_minimum_version(uint32_t major, uint32_t minor)
    {
        criteria.required_version = VK_MAKE_VERSION(major, minor, 0);
        return *this;
    }

    PhysicalDeviceBuilder &PhysicalDeviceBuilder::disable_portability_subset()
    {
        criteria.enable_portability_subset = false;
        return *this;
    }

    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_required_features(VkPhysicalDeviceFeatures const &features)
    {
        criteria.required_features = features;
        return *this;
    }

#if defined(VK_API_VERSION_1_2)
    // Just calls add_required_features
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_required_features_11(VkPhysicalDeviceVulkan11Features features_11)
    {
        features_11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
        add_required_extension_features(features_11);
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_required_features_12(VkPhysicalDeviceVulkan12Features features_12)
    {
        features_12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
        add_required_extension_features(features_12);
        return *this;
    }
#endif
#if defined(VK_API_VERSION_1_3)
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::set_required_features_13(VkPhysicalDeviceVulkan13Features features_13)
    {
        features_13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
        add_required_extension_features(features_13);
        return *this;
    }
#endif
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::defer_surface_initialization()
    {
        criteria.defer_surface_initialization = true;
        return *this;
    }
    PhysicalDeviceBuilder &PhysicalDeviceBuilder::select_first_device_unconditionally(bool unconditionally)
    {
        criteria.use_first_gpu_unconditionally = unconditionally;
        return *this;
    }

    // PhysicalDevice
    bool PhysicalDevice::has_dedicated_compute_queue() const
    {
        return get_dedicated_queue_index(queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT) != QUEUE_INDEX_MAX_VALUE;
    }
    bool PhysicalDevice::has_separate_compute_queue() const
    {
        return get_separate_queue_index(queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT) != QUEUE_INDEX_MAX_VALUE;
    }
    bool PhysicalDevice::has_dedicated_transfer_queue() const
    {
        return get_dedicated_queue_index(queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT) != QUEUE_INDEX_MAX_VALUE;
    }
    bool PhysicalDevice::has_separate_transfer_queue() const
    {
        return get_separate_queue_index(queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT) != QUEUE_INDEX_MAX_VALUE;
    }

    std::vector<VkQueueFamilyProperties> PhysicalDevice::get_queue_families() const { return queue_families; }
    std::vector<std::string> PhysicalDevice::get_extensions() const { return extensions; }
    PhysicalDevice::operator VkPhysicalDevice() const { return this->physical_device; }

}