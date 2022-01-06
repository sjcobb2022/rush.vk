
#include "first_app.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  spdlog::set_pattern("[%^---%l---%$] %v");
  
  lve::FirstApp app{};
  
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}