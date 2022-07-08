# pragma once

// std
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <filesystem>

// libs
//  glfw
#define GLFW_INCLUDE_VULKAN //include vulkan
#include <GLFW/glfw3.h> //core

//  EnTT
#include <entt/entt.hpp> //core

//  glm
#define GLM_FORCE_RADIANS //macros
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //macros
#include <glm/glm.hpp> //core
#include <glm/ext.hpp> //extensions (mat4)
#include <glm/gtc/constants.hpp> //consts (PI, Phi)
#include <glm/gtx/quaternion.hpp> //toMat4

//  spdlog
#include <spdlog/spdlog.h> //core

//  vulkan
#include <vulkan/vulkan.h> //core

//  imgui
#include "imgui.h" //core
#include "backends/imgui_impl_glfw.h" //glfw impl -- potential to add sdl2 or other
#include "backends/imgui_impl_vulkan.h" //vulkan impl

//  VMA
// #include "vk_mem_alloc.h" //core

//  tinygltf
#include <tiny_gltf.h> // get json.hpp from the repository

//  TinyObjLoader
#include <tiny_obj_loader.h> //core

#include <spirv_reflect.h>