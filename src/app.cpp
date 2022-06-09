#include "app.hpp"
#include "backends/vulkan/core.hpp"
#include "core/scene.hpp"

#include "ECS/components/relationship.hpp"
#include "ECS/components/transform.hpp"

#include "window/window.hpp"

#include "rush_pch.hpp"

// libs

namespace rush
{
    struct ignore_t
    {
    };

    App::App() {}

    App::~App() {}

    void App::run()
    {

        GLFWwindow *window = window_init(App::WIDTH, App::HEIGHT, (std::string)"rush_vk", nullptr, nullptr);

        m_DeletionQueue.push_function([&](){
            glfwDestroyWindow(window);
            glfwTerminate();
        });

        Scene scene{};

        Core test{window};

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

    }

}