#include "app.hpp"
#include "core/core.hpp"
#include "ECS/scene.hpp"

#include "ECS/components/relationship.hpp"
#include "ECS/components/transform.hpp"

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
    struct ignore_t
    {
    };

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

        Scene scene{};

        // auto ent = scene.createEntity();

        // scene.emplace<c_Transform, c_Relationship>(ent, c_Transform{glm::vec3{10.0f, 2.0f, 2.0f}}, c_Relationship{});

        auto en0 = scene.create();
        auto en1 = scene.create();


        scene.emplace<c_Transform>(en0, c_Transform{});
        scene.emplace<c_Transform>(en1, c_Transform{});
        
        scene.createRuntimeViewIterator(
            [&](entt::entity entity, c_Transform &transform){
                // spdlog::info("test");
            }
        );

        // scene.createRuntimeViewIterator([](){});

        // assert(scene.get<c_Transform>(ent).translation.x == 10.0f);

        
        scene.flushViewIterators();

        Core test{window};

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

}