#include "app.hpp"
#include "core/rush_pipeline.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <spdlog/spdlog.h>

#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <GLFW/glfw3.h>

// #include "imgui.h"
// #include "backends/imgui_impl_glfw.h"
// #include "backends/imgui_impl_vulkan.h"

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace rush
{
    App::App() {}

    App::~App() {}

    GLFWwindow *App::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        std::string windowName = "rush.vk";

        GLFWwindow *window = glfwCreateWindow(App::WIDTH, App::HEIGHT,
                                              windowName.c_str(),
                                              nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);

        return window;
    }

    void App::run()
    {

        GLFWwindow *window = initWindow();

        Core test{};

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

}