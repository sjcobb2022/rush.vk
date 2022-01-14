#pragma once

#include <entt/entity/registry.hpp>

#include "lve_descriptors.hpp"
#include "lve_device.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"
#include "volk_game_object.hpp"

// libs
#include <GLFW/glfw3.h>

// std
#include <memory>
#include <vector>

namespace volk {

class App {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  App();
  ~App();

  App(const App &) = delete;
  App &operator=(const App &) = delete;

  void run();

  void init_imgui();

  void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

 private:
  void loadGameObjects();

  LveWindow lveWindow{WIDTH, HEIGHT, "Volk.renderer"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  // note: order of declarations matters
  std::unique_ptr<LveDescriptorPool> globalPool{};
  std::shared_ptr<entt::registry> m_Registry{std::make_unique<entt::registry>()};

  VkCommandBuffer commandBuffer;

  VkDescriptorPool imguiPool;
};
}  // namespace volk