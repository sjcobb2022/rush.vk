#include "core.hpp"

// #include "device.hpp"
#include "instance.hpp"

// // std headers
// #include <cstring>
// #include <iostream>
// #include <set>
// #include <unordered_set>

// #include <vulkan/vulkan.h>
// #include <GLFW/glfw3.h>
// #include <spdlog/spdlog.h>



// #include "vk_mem_alloc.h"

namespace rush
{

    Core::Core(GLFWwindow *window)
    {
        // device = rush::device::createDevice(window);
        InstanceBuilder builder;
        auto instance = builder.set_app_name("Test VK App")
                        .request_validation_layers()
                        .build();
    };

    Core::~Core()
    {
        // rush::device::cleanup();
    };

} // namespace rush