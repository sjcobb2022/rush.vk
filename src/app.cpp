#include "app.hpp"
#include "backends/vulkan/core.hpp"
#include "core/scene.hpp"

#include "ECS/components/relationship.hpp"
#include "ECS/components/transform.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "rush_pch.hpp"

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
        
        // scene.each(
        //     [&](entt::entity entity, c_Transform& t){
        //         spdlog::info("translation x: {}", t.translation.x);
        //     }
        // );

        Core test{window};

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

}