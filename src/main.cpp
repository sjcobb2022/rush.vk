
#include "app.hpp"

// pch
// #include "rush_pch.hpp"

int main()
{
    // spdlog::formatter();
    spdlog::set_pattern("[%^-=%l=-%$] %v");

#if _DEBUG || !NDEBUG || DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    try
    {
        rush::App app{};

        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}