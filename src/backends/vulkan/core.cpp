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
        std::vector<PhysicalDevice> physical_device = phb.set_surface(surface)
                                                          .set_minimum_version(1, 1)
                                                          .require_dedicated_transfer_queue()
                                                          .select_devices();

        for (auto &psd : physical_device)
        {
            spdlog::info("{}", psd.name);
        }

        DeviceBuilder device_builder{physical_device.at(0)};

        Device device = device_builder.build();

        // spdlog::info("queue index: {}", device.get_queue_index(QueueType::graphics));

        // device.get_queue(QueueType::graphics);
        // device.get_queue(QueueType::present);

        SwapchainBuilder swapchain_builder{device};

        int w, h;

        glfwGetWindowSize(window, &w, &h);

        Swapchain swapchain = swapchain_builder.build();

    };

    Core::~Core(){
        // rush::device::cleanup();
    };

} // namespace rush