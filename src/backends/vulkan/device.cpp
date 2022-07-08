#include "rush_pch.hpp"
#include "device.hpp"

namespace rush
{
    uint32_t Device::get_queue_index(QueueType type) const
    {
        uint32_t index = QUEUE_INDEX_MAX_VALUE;
        switch (type)
        {
        case QueueType::present:
            index = rush::get_present_queue_index(physical_device.physical_device, surface, queue_families);
            if (index == QUEUE_INDEX_MAX_VALUE)
                throw std::runtime_error("Present Queue Unavailabe");
            break;
        case QueueType::graphics:
            index = rush::get_first_queue_index(queue_families, VK_QUEUE_GRAPHICS_BIT);
            if (index == QUEUE_INDEX_MAX_VALUE)
                throw std::runtime_error("Graphics Queue Unavailable");
            break;
        case QueueType::compute:
            index = rush::get_separate_queue_index(queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT);
            if (index == QUEUE_INDEX_MAX_VALUE)
                throw std::runtime_error("Compute Queue Unavailabe");
            break;
        case QueueType::transfer:
            index = rush::get_separate_queue_index(queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT);
            if (index == QUEUE_INDEX_MAX_VALUE)
                throw std::runtime_error("Transfer Queue Unavailabe");
            break;
        default:
            throw std::runtime_error("Invalid Queue Family Index");
        }
        return index;
    }

    uint32_t Device::get_dedicated_queue_index(QueueType type) const
    {
        uint32_t index = QUEUE_INDEX_MAX_VALUE;
        switch (type)
        {
        case QueueType::compute:
            index = rush::get_dedicated_queue_index(queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT); // specify rush because otherwise name is invalid
            if (index == QUEUE_INDEX_MAX_VALUE)
                throw std::runtime_error("Compute Queue Unavailable");
            break;
        case QueueType::transfer:
            index = rush::get_dedicated_queue_index(queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT);
            if (index == QUEUE_INDEX_MAX_VALUE)
                throw std::runtime_error("Transfer Queue Unavailable");
            break;
        default:
            throw std::runtime_error("Invalid Queue Family Index");
        }
        return index;
    }

    VkQueue Device::get_queue(QueueType type) const
    {
        auto index = get_queue_index(type); // Will runtime error if anything is wrong
        VkQueue out_queue;
        vkGetDeviceQueue(device, index, 0, &out_queue);
        return out_queue;
    }

    VkQueue Device::get_dedicated_queue(QueueType type) const
    {
        auto index = get_dedicated_queue_index(type); // Will runtime error if anything is wrong
        VkQueue out_queue;
        vkGetDeviceQueue(device, index, 0, &out_queue);
        return out_queue;
    }

    Device::operator VkDevice() const { return this->device; }

    CustomQueueDescription::CustomQueueDescription(uint32_t index, uint32_t count, std::vector<float> priorities)
        : index(index), count(count), priorities(priorities)
    {
        assert(count == priorities.size());
    }

    void destroy_device(Device device)
    {
        vkDestroyDevice(device.device, device.allocation_callbacks);
    }

    DeviceBuilder::DeviceBuilder(PhysicalDevice phys_device) { physical_device = phys_device; }

    Device DeviceBuilder::build() const
    {

        std::vector<CustomQueueDescription> queue_descriptions;
        queue_descriptions.insert(queue_descriptions.end(), info.queue_descriptions.begin(), info.queue_descriptions.end());

        if (queue_descriptions.size() == 0)
        {
            for (uint32_t i = 0; i < physical_device.queue_families.size(); i++)
            {
                queue_descriptions.push_back(CustomQueueDescription{i, 1, std::vector<float>{1.0f}});
            }
        }

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        for (auto &desc : queue_descriptions)
        {
            VkDeviceQueueCreateInfo queue_create_info = {};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = desc.index;
            queue_create_info.queueCount = desc.count;
            queue_create_info.pQueuePriorities = desc.priorities.data();
            queueCreateInfos.push_back(queue_create_info);
        }

        std::vector<const char *> extensions;
        for (const auto &ext : physical_device.extensions)
        {
            extensions.push_back(ext.c_str());
        }
        if (physical_device.surface != VK_NULL_HANDLE || physical_device.defer_surface_initialization)
            extensions.push_back({VK_KHR_SWAPCHAIN_EXTENSION_NAME});

        bool has_phys_dev_features_2 = false;
        bool user_defined_phys_dev_features_2 = false;
        std::vector<VkBaseOutStructure *> final_pnext_chain;
        VkDeviceCreateInfo device_create_info = {};

#if defined(VK_API_VERSION_1_1)
        for (auto &pnext : info.pNext_chain)
        {
            if (pnext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2)
            {
                user_defined_phys_dev_features_2 = true;
                break;
            }
        }

        auto physical_device_extension_features_copy = physical_device.extended_features_chain;
        VkPhysicalDeviceFeatures2 local_features2{};
        local_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

        if (!user_defined_phys_dev_features_2)
        {
            if (physical_device.instance_version >= VK_API_VERSION_1_1)
            {
                local_features2.features = physical_device.features;
                final_pnext_chain.push_back(reinterpret_cast<VkBaseOutStructure *>(&local_features2));
                has_phys_dev_features_2 = true;
                for (auto &features_node : physical_device_extension_features_copy)
                {
                    final_pnext_chain.push_back(reinterpret_cast<VkBaseOutStructure *>(&features_node));
                }
            }
        }
        else
        {
            spdlog::warn("User provided VkPhysicalDeviceFeatures2 instance found in pNext chain. All "
                         "requirements added via 'add_required_extension_features' will be ignored.");
        }

        if (!user_defined_phys_dev_features_2 && !has_phys_dev_features_2)
        {
            device_create_info.pEnabledFeatures = &physical_device.features;
        }
#endif

        for (auto &pnext : info.pNext_chain)
        {
            final_pnext_chain.push_back(pnext);
        }

        setup_pNext_chain(device_create_info, final_pnext_chain);

#if !defined(NDEBUG)
        for (auto &node : final_pnext_chain)
        {
            assert(node->sType != VK_STRUCTURE_TYPE_APPLICATION_INFO);
        }
#endif
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.flags = info.flags;
        device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        device_create_info.pQueueCreateInfos = queueCreateInfos.data();
        device_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        device_create_info.ppEnabledExtensionNames = extensions.data();

        Device device;

        VkResult res = vkCreateDevice(
            physical_device.physical_device, &device_create_info, info.allocation_callbacks, &device.device);
        if (res != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create device");
        }

        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr; //using this cause the other method didn't seem to work
        vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo alloc_info = {};
        alloc_info.vulkanApiVersion = VK_API_VERSION_1_1;
        alloc_info.instance = physical_device.instance; // has conversion operator
        alloc_info.physicalDevice = physical_device;    // has conversion operator
        alloc_info.device = device;                     // has conversion operator
        alloc_info.pVulkanFunctions = &vulkanFunctions; //pass vulkan functions

        vmaCreateAllocator(&alloc_info, &device.allocator); //allocator is already instantiated in struct, properly allocate it here.

        //pass on info shit

        device.physical_device = physical_device;
        device.surface = physical_device.surface;
        device.queue_families = physical_device.queue_families;
        device.allocation_callbacks = info.allocation_callbacks; 
        return device;
    }

    DeviceBuilder &DeviceBuilder::custom_queue_setup(std::vector<CustomQueueDescription> queue_descriptions)
    {
        info.queue_descriptions = queue_descriptions;
        return *this;
    }

    DeviceBuilder &DeviceBuilder::set_allocation_callbacks(VkAllocationCallbacks *callbacks)
    {
        info.allocation_callbacks = callbacks;
        return *this;
    }

}