#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"
#include <entt/entity/registry.hpp>

// lib
#include <vulkan/vulkan.h>

namespace lve {
struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LveCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  std::shared_ptr<entt::registry> gameObjectsRegistry;
};
}  // namespace lve
