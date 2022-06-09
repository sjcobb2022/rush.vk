#pragma once

// libs
#include "rush_pch.hpp"

namespace rush
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

        // void registerSetupIn

        void setup(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)> &&setupFunction);

        void loop(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)> &&loopFunction);

        void end(std::function<void(entt::registry sceneRegistry, int placeHolderforCam)> &&endFunction);

    };

} // namespace utr