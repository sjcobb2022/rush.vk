#pragma once

#include "rush_pch.hpp"
#include "physical_device.hpp"

namespace rush
{
    // ---- Queue ---- //
    enum class QueueType
    {
        present,
        graphics,
        compute,
        transfer
    };

    struct Device
    {
        VkDevice device = VK_NULL_HANDLE;
        PhysicalDevice physical_device;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        std::vector<VkQueueFamilyProperties> queue_families;
        VkAllocationCallbacks *allocation_callbacks = VK_NULL_HANDLE;

        uint32_t get_queue_index(QueueType type) const;
        // Only a compute or transfer queue type is valid. All other queue types do not support a 'dedicated' queue index
        uint32_t get_dedicated_queue_index(QueueType type) const;

        VkQueue get_queue(QueueType type) const;
        // Only a compute or transfer queue type is valid. All other queue types do not support a 'dedicated' queue
        VkQueue get_dedicated_queue(QueueType type) const;


        // Return a loaded dispatch table
        // DispatchTable make_table() const;

        // A conversion function which allows this Device to be used
        // in places where VkDevice would have been used.
        operator VkDevice() const;

    private:

        friend class DeviceBuilder;
        friend void destroy_device(Device device);
    };

    void destroy_device(Device device);

    struct CustomQueueDescription
    {
        explicit CustomQueueDescription(uint32_t index, uint32_t count, std::vector<float> priorities);
        uint32_t index = 0;
        uint32_t count = 0;
        std::vector<float> priorities;
    };

    class DeviceBuilder
    {
    public:
        // Any features and extensions that are requested/required in PhysicalDeviceBuilder are automatically enabled.
        explicit DeviceBuilder(PhysicalDevice physical_device);

        Device build() const;

        // For Advanced Users: specify the exact list of VkDeviceQueueCreateInfo's needed for the application.
        // If a custom queue setup is provided, getting the queues and queue indexes is up to the application.
        DeviceBuilder &custom_queue_setup(std::vector<CustomQueueDescription> queue_descriptions);

        // Add a structure to the pNext chain of VkDeviceCreateInfo.
        // The structure must be valid when DeviceBuilder::build() is called.
        template <typename T>
        DeviceBuilder &add_pNext(T *structure)
        {
            info.pNext_chain.push_back(reinterpret_cast<VkBaseOutStructure *>(structure));
            return *this;
        }

        // Provide custom allocation callbacks.
        DeviceBuilder &set_allocation_callbacks(VkAllocationCallbacks *callbacks);

    private:
        PhysicalDevice physical_device;
        struct DeviceInfo
        {
            VkDeviceCreateFlags flags = static_cast<VkDeviceCreateFlags>(0);
            std::vector<VkBaseOutStructure *> pNext_chain;
            std::vector<CustomQueueDescription> queue_descriptions;
            VkAllocationCallbacks *allocation_callbacks = VK_NULL_HANDLE;
        } info;
    };
}