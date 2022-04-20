
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

#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

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