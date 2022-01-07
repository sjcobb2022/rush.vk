#pragma once

#include "volk_camera.hpp"
#include "volk_game_object.hpp"
#include <entt/entity/registry.hpp>

// lib
#include <vulkan/vulkan.h>

namespace volk {
struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LveCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  std::shared_ptr<entt::registry> gameObjectsRegistry;
};
}  // namespace lve
