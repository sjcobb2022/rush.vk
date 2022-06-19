#include "core.hpp"

// #include "device.hpp"
#include "rush_pch.hpp"
#include "instance.hpp"
#include "physical_device.hpp"
#include "device.hpp"
#include "swapchain.hpp"

// #include "vk_mem_alloc.h"

namespace rush
{

    Core::Core(GLFWwindow *window)
    {
        InstanceBuilder builder;
        Instance instance = builder.set_app_name("Test VK App")
                                .request_api_version(VK_VERSION_1_1)
                                .request_validation_layers()
                                .build();

        // spdlog::info("instance_api in core: {}", instance->api_version);

        // glfwCreateWindowSurface
        VkSurfaceKHR surface;
        glfwCreateWindowSurface(instance.instance, window, nullptr, &surface);

        PhysicalDeviceBuilder phb{instance, surface};
        PhysicalDevice physical_device = phb.set_surface(surface)
                                                          .set_minimum_version(0, 1, 1, 0)
                                                          .select();

        // for (auto &psd : physical_device)
        // {
        spdlog::debug("{}", physical_device.name);
        // }

        DeviceBuilder device_builder{physical_device};

        Device device = device_builder.build();

        // spdlog::

        SwapchainBuilder swapchain_builder{device};

        int w, h;

        glfwGetWindowSize(window, &w, &h);

        Swapchain swapchain = swapchain_builder
                                  .set_desired_extent(w, h)
                                  .set_desired_present_mode(VK_PRESENT_MODE_FIFO_RELAXED_KHR)
                                  .build();

        // swapchain.get_image_views();
    };

    Core::~Core(){
        // rush::device::cleanup();
    };

} // namespace rush