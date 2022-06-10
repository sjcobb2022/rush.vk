#pragma once

#include "rush_pch.hpp"
#include "instance.hpp"
namespace rush
{

    class PhysicalDeviceBuilder;
    class DeviceBuilder;

    struct PhysicalDevice
    {
        std::string name;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        // Note that this reflects selected features carried over from required features, not all features the physical device supports.
        VkPhysicalDeviceFeatures features{};
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceMemoryProperties memory_properties{};

        // Has a queue family that supports compute operations but not graphics nor transfer.
        bool has_dedicated_compute_queue() const;
        // Has a queue family that supports transfer operations but not graphics nor compute.
        bool has_dedicated_transfer_queue() const;

        // Has a queue family that supports transfer operations but not graphics.
        bool has_separate_compute_queue() const;
        // Has a queue family that supports transfer operations but not graphics.
        bool has_separate_transfer_queue() const;

        // Advanced: Get the VkQueueFamilyProperties of the device if special queue setup is needed
        std::vector<VkQueueFamilyProperties> get_queue_families() const;

        // Query the list of extensions which should be enabled
        std::vector<std::string> get_extensions() const;

        operator VkPhysicalDevice() const;

    private:
        uint32_t instance_version = VK_API_VERSION_1_0;
        std::vector<std::string> extensions;
        std::vector<VkQueueFamilyProperties> queue_families;
// std::vector<detail::GenericFeaturesPNextNode> extended_features_chain;
#if defined(VK_API_VERSION_1_1)
        VkPhysicalDeviceFeatures2 features2{};
#else
        VkPhysicalDeviceFeatures2KHR features2{};
#endif

        bool defer_surface_initialization = false;
        enum class Suitable
        {
            yes,
            partial,
            no
        };
        Suitable suitable = Suitable::yes;
        friend class PhysicalDeviceBuilder;
        friend class DeviceBuilder;
    };

    enum class PreferredDeviceType
    {
        other = 0,
        integrated = 1,
        discrete = 2,
        virtual_gpu = 3,
        cpu = 4
    };

    enum class DeviceSelectionMode
    {
        // return all suitable and partially suitable devices
        partially_and_fully_suitable,
        // return only physical devices which are fully suitable
        only_fully_suitable
    };

    class PhysicalDeviceBuilder
    {
    public:
        explicit PhysicalDeviceBuilder(Instance const &inst);
        explicit PhysicalDeviceBuilder(Instance const &inst, VkSurfaceKHR surface);

        PhysicalDevice select(DeviceSelectionMode selection = DeviceSelectionMode::partially_and_fully_suitable) const;

        PhysicalDevice select_devices(DeviceSelectionMode selection = DeviceSelectionMode::partially_and_fully_suitable) const;

        PhysicalDevice select_device_names(DeviceSelectionMode selection = DeviceSelectionMode::partially_and_fully_suitable) const;

        PhysicalDeviceBuilder &set_surface(VkSurfaceKHR surface);

        PhysicalDeviceBuilder &set_name(std::string const &name);

        PhysicalDeviceBuilder &prefer_gpu_device_type(PreferredDeviceType type = PreferredDeviceType::discrete);

        PhysicalDeviceBuilder &require_present(bool require = true);

        PhysicalDeviceBuilder &require_dedicated_compute_queue();
        PhysicalDeviceBuilder &require_dedicated_transfer_queue();
        PhysicalDeviceBuilder &require_separate_compute_queue();
        PhysicalDeviceBuilder &require_separate_transfer_queue();

        PhysicalDeviceBuilder &required_device_memory_size(VkDeviceSize size);
        PhysicalDeviceBuilder &desired_device_memory_size(VkDeviceSize size);
        PhysicalDeviceBuilder &add_required_extension(const char *extension);

        PhysicalDeviceBuilder &add_required_extensions(std::vector<const char *> extensions);

        PhysicalDeviceBuilder &add_desired_extension(const char *extension);

        PhysicalDeviceBuilder &add_desired_extensions(std::vector<const char *> extensions);

        PhysicalDeviceBuilder &set_minimum_version(uint32_t major, uint32_t minor);

        PhysicalDeviceBuilder &disable_portability_subset();

    private:
        struct InstanceInfo
        {
            VkInstance instance = VK_NULL_HANDLE;
            VkSurfaceKHR surface = VK_NULL_HANDLE;
            uint32_t version = VK_API_VERSION_1_0;
            bool headless = false;
            bool supports_properties2_ext = false;
        } instance_info;

        struct SelectionCriteria
        {
            std::string name;
            PreferredDeviceType preferred_type = PreferredDeviceType::discrete;
            bool allow_any_type = true;
            bool require_present = true;
            bool require_dedicated_transfer_queue = false;
            bool require_dedicated_compute_queue = false;
            bool require_separate_transfer_queue = false;
            bool require_separate_compute_queue = false;
            VkDeviceSize required_mem_size = 0;
            VkDeviceSize desired_mem_size = 0;

            std::vector<std::string> required_extensions;
            std::vector<std::string> desired_extensions;

            uint32_t required_version = VK_API_VERSION_1_0;
            uint32_t desired_version = VK_API_VERSION_1_0;

            VkPhysicalDeviceFeatures required_features{};
#if defined(VK_API_VERSION_1_1)
            VkPhysicalDeviceFeatures2 required_features2{};
            // std::vector<detail::GenericFeaturesPNextNode> extended_features_chain;
#endif
            bool defer_surface_initialization = false;
            bool use_first_gpu_unconditionally = false;
            bool enable_portability_subset = true;
        } criteria;

        // PhysicalDevice populate_device_details(VkPhysicalDevice phys_device,
        // std::vector<detail::GenericFeaturesPNextNode> const& src_extended_features_chain) const;

        PhysicalDevice::Suitable is_device_suitable(PhysicalDevice const &phys_device) const;
    };

    // ---- Queue ---- //
    enum class QueueType
    {
        present,
        graphics,
        compute,
        transfer
    };

    
}