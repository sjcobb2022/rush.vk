
#include "app.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>


int main()
{
    rush::App app{};

    // spdlog::formatter();
    spdlog::set_pattern("[%^-=%l=-%$] %v");

    #ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
    #endif

    //create a scene.
    //something in the setup function.
    //in the setup function
    //take in the scene
    //register a component
    //when registering component, takes in a functional
    //functional used to define the behaviour of the component
    //e.g. registerComponent<T>(std::functional); --> implementation
    //registerComponent<Renderable>([&](Scene sc, FrameInfo FI, ... maybe other stuff){
    // vkCommandBuffer() 
    //})
    //would need a way to represent relationships (like a registry.view thing)
    //registerComponents<T, T, Args...>([&](Scene sc, FrameInfo FI, T Args...){doStuff();})

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