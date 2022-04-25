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
project(${NAME} VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")


message(STATUS " :: Find Vulkan loader and Shaderc")
find_package(Vulkan REQUIRED)
find_package(Shaderc REQUIRED)


message(STATUS " :: Adding Vulkan Memory Allocator")
add_subdirectory(libs/vma)

message(STATUS " :: Adding GLFW")

if(WIN32)
	add_compile_definitions(_GLFW_WIN32=1)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
set(GLFW_USE_WAYLAND ON CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)

message(STATUS " :: Adding spdlog")
add_subdirectory(libs/spdlog)

message(STATUS " :: Adding glm")
add_subdirectory(libs/glm)

message(STATUS " :: Adding imgui")
add_subdirectory(libs/imgui)

message(STATUS " :: Adding entt")
add_subdirectory(libs/entt)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)

target_link_libraries(${PROJECT_NAME} PUBLIC shaderc)

if(NOT TARGET shaderc OR NOT ENV{USE_SHADERC})
    message(STATUS " :: Building shaders without ShaderC")

    file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    )

    foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endforeach(GLSL)

    add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
    )
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC VulkanMemoryAllocator)
target_link_libraries(${PROJECT_NAME} PUBLIC EnTT::EnTT)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:"DEBUG">)