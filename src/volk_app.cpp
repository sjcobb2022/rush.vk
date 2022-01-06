#include "volk_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"
#include "volk_camera.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <spdlog/spdlog.h>

#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace lve {

struct GlobalUbo {
  glm::mat4 projection{1.f};
  glm::mat4 view{1.f};
  glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};  // w is intensity
  glm::vec3 lightPosition{-1.f};
  alignas(16) glm::vec4 lightColor{1.f};  // w is light intensity
};

App::App() {
  globalPool =
      LveDescriptorPool::Builder(lveDevice)
          .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();
  loadGameObjects();
}

App::~App() {}

void App::run() {
  std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

  for (int i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<LveBuffer>(
        lveDevice,
        sizeof(GlobalUbo),
        1,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffers[i]->map();
  }

  auto globalSetLayout =
      LveDescriptorSetLayout::Builder(lveDevice)
          .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
          .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

  for (int i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    LveDescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{
      lveDevice,
      lveRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  PointLightSystem pointLightSystem{
      lveDevice,
      lveRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  LveCamera camera{};

  auto viewerObject = LveGameObject::createGameObject(m_Registry);

  viewerObject.addComponent<TransformComponent>(
      TransformComponent{{0.0f, 0.0f, -2.5f} /*translation -2.5*/});

  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();
  // spdlog::info("this should print before the malloc");

  //
  double curMouseX, curMouseY, newMouseX, newMouseY, dxMouseX, dxMouseY;
  glfwGetCursorPos(lveWindow.getGLFWwindow(), &curMouseX, &curMouseY);

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    glfwGetCursorPos(lveWindow.getGLFWwindow(), &newMouseX, &newMouseY);

    // dxMouseX = newMouseX - curMouseX;
    // dxMouseY = newMouseY - curMouseY;
    // curMouseX = newMouseX;
    // curMouseY = newMouseY;

    // spdlog::info("{}   :   {}", dxMouseX, dxMouseY);

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
    TransformComponent cameraTransform = viewerObject.getComponent<TransformComponent>();
    camera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);

    // spdlog::info("{}   :   {}", xpos, ypos);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      int frameIndex = lveRenderer.getFrameIndex();

      FrameInfo frameInfo{
          frameIndex,
          frameTime,
          commandBuffer,
          camera,
          globalDescriptorSets[frameIndex],
          m_Registry};

      // update
      GlobalUbo ubo{};
      ubo.projection = camera.getProjection();
      ubo.view = camera.getView();
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void App::loadGameObjects() {
  std::shared_ptr<LveModel> lveModel =
      LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");

  auto flatVase = LveGameObject::createGameObject(m_Registry);

  flatVase.addComponent<std::shared_ptr<LveModel>>(lveModel);
  flatVase.addComponent<TransformComponent>(TransformComponent{
      {-.5f, .5f, 0.f},  // translation
      {3.f, 1.5f, 3.f}   // scale
  });

  // create flat vase model

  lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
  auto smoothVase = LveGameObject::createGameObject(m_Registry);

  smoothVase.addComponent<std::shared_ptr<LveModel>>(lveModel);
  smoothVase.addComponent<TransformComponent>(TransformComponent{
      {.5f, .5f, 0.f},  // translation
      {3.f, 1.5f, 3.f}  // scale
  });

  // create smooth vase model

  lveModel = LveModel::createModelFromFile(lveDevice, "models/quad.obj");
  auto floor = LveGameObject::createGameObject(m_Registry);

  floor.addComponent<std::shared_ptr<LveModel>>(lveModel);
  floor.addComponent<TransformComponent>(
      TransformComponent{{0.f, .5f, 0.f}, /*translation */ {3.f, 1.f, 3.f} /*scale*/});

  // create floor model
}

}  // namespace lve
