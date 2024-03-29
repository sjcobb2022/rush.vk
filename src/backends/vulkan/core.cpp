#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include "core.hpp"

#include "rush_pch.hpp"
#include "instance.hpp"
#include "physical_device.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "shader.hpp"
#include "pipeline.hpp"

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

        PhysicalDeviceBuilder phb{instance};
        PhysicalDevice physical_device = phb.set_surface(surface)
                                             .set_minimum_version(0, 1, 1, 0)
                                             .select(); //can select multiple

        DeviceBuilder device_builder{physical_device};

        Device device = device_builder.build();

        SwapchainBuilder swapchain_builder{device};

        int w, h;

        glfwGetWindowSize(window, &w, &h);

        Swapchain swapchain = swapchain_builder
                                  .set_desired_extent(w, h)
                                  .set_desired_present_mode(VK_PRESENT_MODE_FIFO_RELAXED_KHR)
                                  .build();

        auto images = swapchain.get_images();
        auto views = swapchain.get_image_views();
        // auto d_images = swapchain.get_depth_images();
        // auto d_views = swapchain.get_depth_views();

        // TODO:
        //  ☑️ DEPTH IMAGES --> depth format, swapchain extent, then create image with allocated memory (use VMA) and create image views from there
        //  RENDERPASS --> swapchain image format, color attachments (color and depth attachment),
        //  FRAMEBUFFERS --> image views, swapchain extent, device, a renderpass, image count
        //  SYNC OBJECTS (semaphores, fences) --> dependant on: device, max frames, imagecount
        //  DESCRIPTORS (can use basic impl from vkguide)
        //  BUFFERS -- this is gonna take a while
        //  SHADERC -- implement runtime

        // vkDeviceWaitIdle(device);
        // cleanupSwapChain();
        // createSwapChain();
        // createImageViews();
        // createRenderPass();
        // createGraphicsPipeline();
        // createFramebuffers();

        // Need to get: triangle verts from brendans old one

        // TODO: Pipeline deletion, image deletion, image view deletion

        ShaderBuilder shaderBuilder{device};

        auto vertS = shaderBuilder.set_path("assets/shaders/simple_shader.vert").set_stage(VK_SHADER_STAGE_VERTEX_BIT).build();
        auto fragS = shaderBuilder.set_path("assets/shaders/simple_shader.frag.spv").set_stage(VK_SHADER_STAGE_FRAGMENT_BIT).build();

        PipelineBuilder pipelineBuilder{device};

        pipelineBuilder.add_stage(vertS).add_stage(fragS);
        
    };

    Core::~Core(){
        // rush::device::cleanup();
    };

} // namespace rush