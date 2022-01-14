#include "volk_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"
#include "volk_camera.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <spdlog/spdlog.h>

#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_vulkan.h"
#include "imgui/imgui.h"

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

#include "utils.hpp"

namespace volk {

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

void App::init_imgui() {
  VkDescriptorPoolSize pool_sizes[] = {
      {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
      {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 1000;
  pool_info.poolSizeCount = std::size(pool_sizes);
  pool_info.pPoolSizes = pool_sizes;

  // VkDescriptorPool imguiPool;

  vkCreateDescriptorPool(lveDevice.device(), &pool_info, nullptr, &imguiPool);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // ImGuiIO& io = ImGui::GetIO();
  // (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows

  ImGui_ImplGlfw_InitForVulkan(lveWindow.getGLFWwindow(), false);

  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = lveDevice.getInstance();
  init_info.PhysicalDevice = lveDevice.getPhysicalDevice();
  init_info.Device = lveDevice.device();
  init_info.Queue = lveDevice.graphicsQueue();
  init_info.DescriptorPool = imguiPool;
  init_info.MinImageCount = 3;
  init_info.ImageCount = 3;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  ImGui_ImplVulkan_Init(&init_info, lveRenderer.getSwapChainRenderPass());

  {
    // Use any command queue
    VkCommandPool command_pool = lveDevice.getCommandPool();
    VkCommandBuffer command_buffer = lveDevice.beginSingleTimeCommands();

    spdlog::info("initialised command pool and command buffer");

    // vkResetCommandPool(lveDevice.device(), command_pool, 0);

    spdlog::info("reset command pool");
    // check_vk_result(err);
    // VkCommandBufferBeginInfo begin_info = {};
    // begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    // vkBeginCommandBuffer(command_buffer, &begin_info);

    spdlog::info("finished BeginCommandBuffer");
    // check_vk_result(err);

    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

    spdlog::info("created fotn textures with command buffer");

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &command_buffer;
    vkEndCommandBuffer(command_buffer);
    // lveRenderer.endFrame();

    spdlog::info("ended frame");
    // check_vk_result(err);
    vkQueueSubmit(lveDevice.graphicsQueue(), 1, &end_info, VK_NULL_HANDLE);
    // check_vk_result(err);
    vkDeviceWaitIdle(lveDevice.device());
    // check_vk_result(err);
    ImGui_ImplVulkan_DestroyFontUploadObjects();

    spdlog::info("end:");
  }
}

// void App::cursorCallback(GLFWwindow *window, double x, double y){

//   // spdlog::info("{}x : {}y", x, y);

//   glfwSetCursorPos(window, 0, 0);
// }

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

  double mouseX, mouseY;

  // IMGUI_CHECKVERSION();
  // ImGui::CreateContext();
  // ImGuiIO& io = ImGui::GetIO();
  // (void)io;
  // ImGui::StyleColorsDark();

  init_imgui();

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    glfwGetCursorPos(lveWindow.getGLFWwindow(), &mouseX, &mouseY);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the docking environment
    // ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
    //                                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
    //                                ImGuiWindowFlags_NoMove |
    //                                ImGuiWindowFlags_NoBringToFrontOnFocus |
    //                                ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    // ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
    // ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    // ImGui::End();

    bool show_demo_window = true;
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    // cameraController.moveMouse(lveWindow.getGLFWwindow(), frameTime, viewerObject, mouseX,
    // mouseY);

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
    TransformComponent cameraTransform = viewerObject.getComponent<TransformComponent>();
    camera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);


    // Rendering
    ImGui::Render();

    if ((commandBuffer = lveRenderer.beginFrame())) {

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

      // ImGui::EndFrame();

      ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

      // ImGuiIO& io = ImGui::GetIO();
      // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      //   GLFWwindow* backup_current_context = glfwGetCurrentContext();
      //   ImGui::UpdatePlatformWindows();
      //   ImGui::RenderPlatformWindowsDefault();
      //   glfwMakeContextCurrent(backup_current_context);
      // }

      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();

      // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), lveDevice.beginSingleTimeCommands());

      // ImGuiIO& io = ImGui::GetIO();

      // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      //   GLFWwindow* backup_current_context = glfwGetCurrentContext();
      //   ImGui::UpdatePlatformWindows();
      //   ImGui::RenderPlatformWindowsDefault();
      //   glfwMakeContextCurrent(backup_current_context);
      // }

    }
    // }
  }

  // cleanup
  vkDeviceWaitIdle(lveDevice.device());
  vkDestroyDescriptorPool(lveDevice.device(), imguiPool, nullptr);
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // ImGui_ImplVulkanH_DestroyWindow(lveDevice.getInstance(), lveDevice.device(), ImGui_, g_Allocator); //TODOs
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

}  // namespace volk
