if(WIN32)
  include(./.env.windows.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
elseif(UNIX)
  include(./.env.unix.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
else()
  include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
endif()

message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.16.0)
set(NAME rush_vk)
project(${NAME} VERSION 0.1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")


message(NOTICE "Find Vulkan Loader and Shaderc")
find_package(Vulkan REQUIRED)

# find_package(Shaderc REQUIRED)

message(STATUS "spirv tools SOURCE DIR ${spirv-tools_SOURCE_DIR}")

message(NOTICE "Adding Vulkan Memory Allocator")
add_subdirectory(libs/vma)

message(NOTICE "Adding GLFW")

if(WIN32)
	add_compile_definitions(_GLFW_WIN32=1)
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

# message(NOTICE "Adding shaderc")
# set(SHADERC_SKIP_INSTALL OFF CACHE BOOL "" FORCE)
# set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
# set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)
# set(SHADERC_SKIP_COPYRIGHT_CHECK ON CACHE BOOL "" FORCE)
# set(SHADERC_ENABLE_SHARED_CRT ON CACHE BOOL "" FORCE)
# add_subdirectory(libs/shaderc)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)

# target_link_libraries(${PROJECT_NAME} PUBLIC shaderc)

if(NOT TARGET shaderc OR NOT ENV{USE_SHADERC})
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
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC VulkanMemoryAllocator)
target_link_libraries(${PROJECT_NAME} PUBLIC EnTT::EnTT)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:"DEBUG">)