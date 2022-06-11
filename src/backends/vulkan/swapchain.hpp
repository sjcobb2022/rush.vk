#include "rush_pch.hpp"
#include "device.hpp"

namespace rush
{
    // ---- Swapchain ---- //
    struct Swapchain
    {
        VkDevice device = VK_NULL_HANDLE;
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;
        uint32_t image_count = 0;
        VkFormat image_format = VK_FORMAT_UNDEFINED;
        VkExtent2D extent = {0, 0};
        uint32_t requested_min_image_count = 0;                        // The value of minImageCount actually used when creating the swapchain; note that the presentation engine is always free to create more images than that.
        VkPresentModeKHR present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR; // The present mode actually used when creating the swapchain.
        VkAllocationCallbacks *allocation_callbacks = VK_NULL_HANDLE;

        // Returns a vector of VkImage handles to the swapchain.
        std::vector<VkImage> get_images();

        // Returns a vector of VkImageView's to the VkImage's of the swapchain.
        // VkImageViews must be destroyed.  The pNext chain must be a nullptr or a valid
        // structure.
        std::vector<VkImageView> get_image_views();
        std::vector<VkImageView> get_image_views(const void *pNext);
        void destroy_image_views(std::vector<VkImageView> const &image_views);

        // A conversion function which allows this Swapchain to be used
        // in places where VkSwapchainKHR would have been used.
        operator VkSwapchainKHR() const;

    private:
        struct
        {
            PFN_vkGetSwapchainImagesKHR fp_vkGetSwapchainImagesKHR = nullptr;
            PFN_vkCreateImageView fp_vkCreateImageView = nullptr;
            PFN_vkDestroyImageView fp_vkDestroyImageView = nullptr;
            PFN_vkDestroySwapchainKHR fp_vkDestroySwapchainKHR = nullptr;
        } internal_table;
        friend class SwapchainBuilder;
        friend void destroy_swapchain(Swapchain const &swapchain);
    };

    void destroy_swapchain(Swapchain const &swapchain);

    class SwapchainBuilder
    {
    public:
        // Construct a SwapchainBuilder with a `vkb::Device`
        explicit SwapchainBuilder(Device const &device);
        // Construct a SwapchainBuilder with a specific VkSurfaceKHR handle and `vkb::Device`
        explicit SwapchainBuilder(Device const &device, VkSurfaceKHR const surface);
        // Construct a SwapchainBuilder with Vulkan handles for the physical device, device, and surface
        // Optionally can provide the uint32_t indices for the graphics and present queue
        // Note: The constructor will query the graphics & present queue if the indices are not provided
        explicit SwapchainBuilder(VkPhysicalDevice const physical_device,
                                  VkDevice const device,
                                  VkSurfaceKHR const surface,
                                  uint32_t graphics_queue_index = QUEUE_INDEX_MAX_VALUE,
                                  uint32_t present_queue_index = QUEUE_INDEX_MAX_VALUE);

        Swapchain build() const;

        // Set the oldSwapchain member of VkSwapchainCreateInfoKHR.
        // For use in rebuilding a swapchain.
        SwapchainBuilder &set_old_swapchain(VkSwapchainKHR old_swapchain);
        SwapchainBuilder &set_old_swapchain(Swapchain const &swapchain);

        // Desired size of the swapchain. By default, the swapchain will use the size
        // of the window being drawn to.
        SwapchainBuilder &set_desired_extent(uint32_t width, uint32_t height);

        // When determining the surface format, make this the first to be used if supported.
        SwapchainBuilder &set_desired_format(VkSurfaceFormatKHR format);
        // Add this swapchain format to the end of the list of formats selected from.
        SwapchainBuilder &add_fallback_format(VkSurfaceFormatKHR format);
        // Use the default swapchain formats. This is done if no formats are provided.
        // Default surface format is {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
        SwapchainBuilder &use_default_format_selection();

        // When determining the present mode, make this the first to be used if supported.
        SwapchainBuilder &set_desired_present_mode(VkPresentModeKHR present_mode);
        // Add this present mode to the end of the list of present modes selected from.
        SwapchainBuilder &add_fallback_present_mode(VkPresentModeKHR present_mode);
        // Use the default presentation mode. This is done if no present modes are provided.
        // Default present modes: VK_PRESENT_MODE_MAILBOX_KHR with fallback VK_PRESENT_MODE_FIFO_KHR
        SwapchainBuilder &use_default_present_mode_selection();

        // Set the bitmask of the image usage for acquired swapchain images.
        SwapchainBuilder &set_image_usage_flags(VkImageUsageFlags usage_flags);
        // Add a image usage to the bitmask for acquired swapchain images.
        SwapchainBuilder &add_image_usage_flags(VkImageUsageFlags usage_flags);
        // Use the default image usage bitmask values. This is the default if no image usages
        // are provided. The default is VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
        SwapchainBuilder &use_default_image_usage_flags();

        // Set the bitmask of the format feature flag for acquired swapchain images.
        SwapchainBuilder &set_format_feature_flags(VkFormatFeatureFlags feature_flags);
        // Add a format feature to the bitmask for acquired swapchain images.
        SwapchainBuilder &add_format_feature_flags(VkFormatFeatureFlags feature_flags);
        // Use the default format feature bitmask values. This is the default if no format features
        // are provided. The default is VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT
        SwapchainBuilder &use_default_format_feature_flags();

        // Set the number of views in for multiview/stereo surface
        SwapchainBuilder &set_image_array_layer_count(uint32_t array_layer_count);

        // Set whether the Vulkan implementation is allowed to discard rendering operations that
        // affect regions of the surface that are not visible. Default is true.
        // Note: Applications should use the default of true if they do not expect to read back the content
        // of presentable images before presenting them or after reacquiring them, and if their fragment
        // shaders do not have any side effects that require them to run for all pixels in the presentable image.
        SwapchainBuilder &set_clipped(bool clipped = true);

        // Set the VkSwapchainCreateFlagBitsKHR.
        SwapchainBuilder &set_create_flags(VkSwapchainCreateFlagBitsKHR create_flags);
        // Set the transform to be applied, like a 90 degree rotation. Default is no transform.
        SwapchainBuilder &set_pre_transform_flags(VkSurfaceTransformFlagBitsKHR pre_transform_flags);
        // Set the alpha channel to be used with other windows in on the system. Default is VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR.
        SwapchainBuilder &set_composite_alpha_flags(VkCompositeAlphaFlagBitsKHR composite_alpha_flags);

        // Add a structure to the pNext chain of VkSwapchainCreateInfoKHR.
        // The structure must be valid when SwapchainBuilder::build() is called.
        template <typename T>
        SwapchainBuilder &add_pNext(T *structure)
        {
            info.pNext_chain.push_back(reinterpret_cast<VkBaseOutStructure *>(structure));
            return *this;
        }

        // Provide custom allocation callbacks.
        SwapchainBuilder &set_allocation_callbacks(VkAllocationCallbacks *callbacks);

    private:
        void add_desired_formats(std::vector<VkSurfaceFormatKHR> &formats) const;
        void add_desired_present_modes(std::vector<VkPresentModeKHR> &modes) const;

        struct SwapchainInfo
        {
            VkPhysicalDevice physical_device = VK_NULL_HANDLE;
            VkDevice device = VK_NULL_HANDLE;
            std::vector<VkBaseOutStructure *> pNext_chain;
            VkSwapchainCreateFlagBitsKHR create_flags = static_cast<VkSwapchainCreateFlagBitsKHR>(0);
            VkSurfaceKHR surface = VK_NULL_HANDLE;
            std::vector<VkSurfaceFormatKHR> desired_formats;
            uint32_t desired_width = 256;
            uint32_t desired_height = 256;
            uint32_t array_layer_count = 1;
            VkImageUsageFlags image_usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            VkFormatFeatureFlags format_feature_flags = VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
            uint32_t graphics_queue_index = 0;
            uint32_t present_queue_index = 0;
            VkSurfaceTransformFlagBitsKHR pre_transform = static_cast<VkSurfaceTransformFlagBitsKHR>(0);
            VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            std::vector<VkPresentModeKHR> desired_present_modes;
            bool clipped = true;
            VkSwapchainKHR old_swapchain = VK_NULL_HANDLE;
            VkAllocationCallbacks *allocation_callbacks = VK_NULL_HANDLE;
        } info;
    };
}