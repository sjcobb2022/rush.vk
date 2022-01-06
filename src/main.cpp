
#include "first_app.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  lve::FirstApp app{};

  spdlog::set_pattern("[%^---%l---%$] %v");
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}