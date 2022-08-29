# setup env (really only needed for windows)
include(./.env..cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)

message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.16.0)
set(NAME rush_vk)

project(${NAME})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

message(NOTICE "Find Vulkan Loader and Shaderc")
find_package(Vulkan REQUIRED)

find_package(Shaderc REQUIRED)

message(NOTICE "Adding Vulkan Memory Allocator")
# set(VMA_STATIC_VULKAN_FUNCTIONS ON CACHE BOOL "" FORCE)
# set(VMA_DYNAMIC_VULKAN_FUNCTIONS OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/vma)

if(WIN32)
  add_compile_definitions(_GLFW_WIN32=ON)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)

message(NOTICE "Adding spdlog")
add_subdirectory(libs/spdlog)

message(NOTICE "Adding GLM")
add_subdirectory(libs/glm)

message(NOTICE "Adding imgui")
add_subdirectory(libs/imgui)

message(NOTICE "Adding entt")
add_subdirectory(libs/entt)

message(NOTICE "Adding tinyobjloader")
add_subdirectory(libs/tinyobjloader)

message(NOTICE "Adding SPIRV-Reflect")
set(SPIRV_REFLECT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "" FORCE)
set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL "" FORCE)
add_subdirectory(libs/SPIRV-Reflect)

message(NOTICE "Adding tinygltf")
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_HEADER_ONLY OFF CACHE BOOL "" FORCE)
set(TINYGLTF_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(libs/tinygltf)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# if(NOT TARGET shaderc OR NOT ENV{USE_SHADERC})
message(NOTICE "Building shaders without ShaderC")

find_program(GLSL_VALIDATOR glslangValidator HINTS
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}
  /usr/bin
  /usr/local/bin
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/
  $ENV{VULKAN_SDK}/Bin32/)

file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/assets/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/assets/shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/assets/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

# endif()
add_custom_target(
  Shaders
  DEPENDS ${SPIRV_BINARY_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE glm)

target_link_libraries(${PROJECT_NAME} PRIVATE VulkanMemoryAllocator)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
target_link_libraries(${PROJECT_NAME} PRIVATE shaderc)

target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader)
target_link_libraries(${PROJECT_NAME} PRIVATE tinygltf)

target_link_libraries(${PROJECT_NAME} PRIVATE spirv-reflect-static)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/rush_pch.hpp)

if("${ENGINE_DIR}")
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DENGINE_DIR="${ENGINE_DIR}")
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DENGINE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
endif()
