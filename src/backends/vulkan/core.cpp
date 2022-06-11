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
        Instance *instance = builder.set_app_name("Test VK App")
                                 .request_validation_layers()
                                 .build();

        // glfwCreateWindowSurface
        VkSurfaceKHR surface;
        glfwCreateWindowSurface(instance->instance, window, nullptr, &surface);

        PhysicalDeviceBuilder phb{*instance};
        PhysicalDevice physical_device = phb.set_surface(surface)
                                             .set_minimum_version(1, 1)
                                             .require_dedicated_transfer_queue()
                                             .select();

        DeviceBuilder device_builder{physical_device};

        Device device = device_builder.build();

        device.get_queue(QueueType::graphics);

        SwapchainBuilder swapchain_builder{device, surface};

        int w, h;

        glfwGetWindowSize(window, &w, &h);

        Swapchain swapchain = swapchain_builder
                                  .set_desired_extent(w, h)
                                  .build();
    };

    Core::~Core(){
        // rush::device::cleanup();
    };

} // namespace rush