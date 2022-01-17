
#include "app.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// #include <fstream>

int main()
{
    utr::App app;

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