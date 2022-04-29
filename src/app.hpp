#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <spdlog/spdlog.h>

#include <entt/entity/registry.hpp>

// libs
#include <GLFW/glfw3.h>

// std
#include <memory>
#include <vector>
#include <functional>

namespace rush
{
    class App
    {

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        // App(const App &) = delete;
        // App &operator=(const App &) = delete;

        void run();

        // void registerSetupIn

        // void setup(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)> &&setupFunction);

        // void loop(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)> &&loopFunction);

        // void end(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)> &&endFunction);

    private:
        // GLFWwindow *initWindow();

        // void cleanUpWindow(GLFWwindow *window);

        // std::vector<>;
    };

} // namespace utr