
#include "app.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>
#include <cassert>

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

struct velocity
{
    glm::vec3 vel;
};

struct position
{
    glm::vec3 pos;
};

int main()
{
    rush::App app{};

    // spdlog::formatter();
    spdlog::set_pattern("[%^-=%l=-%$] %v");

#if _DEBUG || !NDEBUG || DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif
    // create a scene.
    // something in the setup function.
    // in the setup function
    // take in the scene
    // register a component
    // when registering component, takes in a functional
    // functional used to define the behaviour of the component
    // e.g. registerComponent<T>(std::functional); --> implementation
    // registerComponentFunction<Renderable>([&](Scene sc, FrameInfo FI, ... maybe other stuff){
    //  vkCommandBuffer();
    // })
    // MAKE IT A TEMPLATED TYPE
    // template <typename... Args>
    // registerLoopInput<T>()
    // registerSetupInput<T>
    // would need a way to represent relationships (like a registry.view thing)
    // registerComponents<T, T, Args...>([&](Scene sc, FrameInfo FI, T Args...){doStuff();};

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}