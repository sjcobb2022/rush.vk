#include "core.hpp"

// #include "device.hpp"
#include "rush_pch.hpp"
#include "instance.hpp"

// #include "vk_mem_alloc.h"

namespace rush
{

    Core::Core(GLFWwindow *window)
    {
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