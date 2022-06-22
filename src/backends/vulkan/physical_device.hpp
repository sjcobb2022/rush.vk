#pragma once

#include "rush_pch.hpp"
#include "instance.hpp"
#include "vk_util.hpp"

namespace rush
{

    uint32_t get_first_queue_index(std::vector<VkQueueFamilyProperties> const &families, VkQueueFlags desired_flags);

    uint32_t get_present_queue_index(VkPhysicalDevice const phys_device, VkSurfaceKHR const surface, std::vector<VkQueueFamilyProperties> const &families);

    uint32_t get_separate_queue_index(std::vector<VkQueueFamilyProperties> const &families, VkQueueFlags desired_flags, VkQueueFlags undesired_flags);

    uint32_t get_dedicated_queue_index(std::vector<VkQueueFamilyProperties> const &families, VkQueueFlags desired_flags, VkQueueFlags undesired_flags);

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
        uint32_t instance_version = VK_API_VERSION_1_1;
        std::vector<std::string> extensions;
        std::vector<VkQueueFamilyProperties> queue_families;
        std::vector<GenericFeaturesPNextNode> extended_features_chain;
        VkInstance instance; //its just a handle so probs ok to store
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

        // Return all devices which are considered suitable - intended for applications which want to let the user pick the physical device
        std::vector<PhysicalDevice> select_devices(
            DeviceSelectionMode selection = DeviceSelectionMode::partially_and_fully_suitable) const;

        // Return the names of all devices which are considered suitable - intended for applications which want to let the user pick the physical device
        std::vector<std::string> select_device_names(
            DeviceSelectionMode selection = DeviceSelectionMode::partially_and_fully_suitable) const;

        // Set the surface in which the physical device should render to.
        // Be sure to set it if swapchain functionality is to be used.
        PhysicalDeviceBuilder &set_surface(VkSurfaceKHR surface);

        // Set the name of the device to select.
        PhysicalDeviceBuilder &set_name(std::string const &name);
        // Set the desired physical device type to select. Defaults to PreferredDeviceType::discrete.
        PhysicalDeviceBuilder &prefer_gpu_device_type(PreferredDeviceType type = PreferredDeviceType::discrete);
        // Allow selection of a gpu device type that isn't the preferred physical device type. Defaults to true.
        PhysicalDeviceBuilder &allow_any_gpu_device_type(bool allow_any_type = true);

        // Require that a physical device supports presentation. Defaults to true.
        PhysicalDeviceBuilder &require_present(bool require = true);

        // Require a queue family that supports compute operations but not graphics nor transfer.
        PhysicalDeviceBuilder &require_dedicated_compute_queue();
        // Require a queue family that supports transfer operations but not graphics nor compute.
        PhysicalDeviceBuilder &require_dedicated_transfer_queue();

        // Require a queue family that supports compute operations but not graphics.
        PhysicalDeviceBuilder &require_separate_compute_queue();
        // Require a queue family that supports transfer operations but not graphics.
        PhysicalDeviceBuilder &require_separate_transfer_queue();

        // Require a memory heap from VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT with `size` memory available.
        PhysicalDeviceBuilder &required_device_memory_size(VkDeviceSize size);
        // Prefer a memory heap from VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT with `size` memory available.
        PhysicalDeviceBuilder &desired_device_memory_size(VkDeviceSize size);

        // Require a physical device which supports a specific extension.
        PhysicalDeviceBuilder &add_required_extension(const char *extension);
        // Require a physical device which supports a set of extensions.
        PhysicalDeviceBuilder &add_required_extensions(std::vector<const char *> extensions);

        // Prefer a physical device which supports a specific extension.
        PhysicalDeviceBuilder &add_desired_extension(const char *extension);
        // Prefer a physical device which supports a set of extensions.
        PhysicalDeviceBuilder &add_desired_extensions(std::vector<const char *> extensions);

        // By default PhysicalDeviceSelector enables the portability subset if available
        // This function disables that behavior
        PhysicalDeviceBuilder &disable_portability_subset();

        // Require a physical device which supports a specific set of general/extension features.
#if defined(VK_API_VERSION_1_1)
        template <typename T>
        PhysicalDeviceBuilder &add_required_extension_features(T const &features)
        {
            criteria.extended_features_chain.push_back(features);
            return *this;
        }
#endif

        PhysicalDeviceBuilder &set_minimum_version(uint32_t variant = 0, uint32_t major=1, uint32_t minor=1, uint32_t patch = 0);

        // Require a physical device which supports the features in VkPhysicalDeviceFeatures.
        PhysicalDeviceBuilder &set_required_features(VkPhysicalDeviceFeatures const &features);
#if defined(VK_API_VERSION_1_2)
        // Require a physical device which supports the features in VkPhysicalDeviceVulkan11Features.
        // Must have vulkan version 1.2 - This is due to the VkPhysicalDeviceVulkan11Features struct being added in 1.2, not 1.1
        PhysicalDeviceBuilder &set_required_features_11(VkPhysicalDeviceVulkan11Features features_11);
        // Require a physical device which supports the features in VkPhysicalDeviceVulkan12Features.
        // Must have vulkan version 1.2
        PhysicalDeviceBuilder &set_required_features_12(VkPhysicalDeviceVulkan12Features features_12);
#endif
#if defined(VK_API_VERSION_1_3)
        // Require a physical device which supports the features in VkPhysicalDeviceVulkan13Features.
        // Must have vulkan version 1.3
        PhysicalDeviceBuilder &set_required_features_13(VkPhysicalDeviceVulkan13Features features_13);
#endif

        // Used when surface creation happens after physical device selection.
        // Warning: This disables checking if the physical device supports a given surface.
        PhysicalDeviceBuilder &defer_surface_initialization();

        // Ignore all criteria and choose the first physical device that is available.
        // Only use when: The first gpu in the list may be set by global user preferences and an application may wish to respect it.
        PhysicalDeviceBuilder &select_first_device_unconditionally(bool unconditionally = true);

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
            std::vector<GenericFeaturesPNextNode> extended_features_chain;
#endif
            bool defer_surface_initialization = false;
            bool use_first_gpu_unconditionally = false;
            bool enable_portability_subset = true;
        } criteria;

        PhysicalDevice populate_device_details(VkPhysicalDevice phys_device,
                                               std::vector<GenericFeaturesPNextNode> const &src_extended_features_chain
#if !defined(VK_API_VERSION_1_1) // stupid hack to avoid having to use getInstanceProc
                                               ,
                                               VkInstance instance
#endif
        ) const;

        PhysicalDevice::Suitable is_device_suitable(PhysicalDevice const &phys_device) const;

        std::vector<PhysicalDevice> select_impl(DeviceSelectionMode selection) const;
    };
}