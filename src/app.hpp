#pragma once

#include <entt/entity/registry.hpp>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

// std
#include <memory>
#include <vector>

namespace volk
{
    class App
    {
        public:

            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            App();
            ~App();

            App(const App &) = delete;
            App &operator=(const App &) = delete;

            void run();

            void setup(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)>&& setupFunction);

            void loop(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)>&& loopFunction);

            void end(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)>&& endFunction);
        
        private:
            GLFWwindow* initWindow();

            void cleanUpWindow(GLFWwindow *window);
        
    };

} //namespace utr