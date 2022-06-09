#include "app.hpp"
#include "backends/vulkan/core.hpp"
#include "core/scene.hpp"

#include "ECS/components/relationship.hpp"
#include "ECS/components/transform.hpp"

#include "window/window.hpp"

// #include "util/util.hpp"

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
        //add to deletion queue
        // delq_push([&](){
        //     glfwDestroyWindow(window);
        //     glfwTerminate();
        // });

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

        delq_flush();
    }

}