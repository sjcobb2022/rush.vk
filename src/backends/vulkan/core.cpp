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

        // spdlog::info("instance_api in core: {}", instance->api_version);

        // glfwCreateWindowSurface
        VkSurfaceKHR surface;
        glfwCreateWindowSurface(instance->instance, window, nullptr, &surface);

        PhysicalDeviceBuilder phb{*instance, surface};
        std::vector<PhysicalDevice> physical_device = phb.set_surface(surface)
                                                          .set_minimum_version(0, 1, 1, 0)
                                                          //.select_first_device_unconditionally()
                                                        //   .require_separate_transfer_queue()
                                                          //.require_dedicated_transfer_queue()
                                                          .select_devices();

        for (auto &psd : physical_device)
        {
            spdlog::debug("{}", psd.name);
        }

        DeviceBuilder device_builder{physical_device.at(0)};

        Device device = device_builder.build();

        // spdlog::

        SwapchainBuilder swapchain_builder{device, surface};

        // int w, h;

        // glfwGetWindowSize(window, &w, &h);

        // Swapchain swapchain = swapchain_builder.build();
    };

    Core::~Core(){
        // rush::device::cleanup();
    };

} // namespace rush