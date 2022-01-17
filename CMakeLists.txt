include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME volk)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.0.1)

# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# 2. Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2019") # 2.1 Update lib-vc2019 to use same version as your visual studio
  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64") # 2.1 make sure matches glfw mingw subdirectory
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
	message(FATAL_ERROR "Could not find glfw library!")
else()
	message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()

# 3 Set EnTT path in .env.cmake
if(DEFINED ENTT_PATH)
  message(STATUS "Using EnTT path specified in .env -- uses single include")
  target_include_directories(${PROJECT_NAME} PUBLIC ${ENTT_PATH}/single_include)
  # set(EnTT::EnTT EnTT)
else()
  find_package(EnTT REQUIRED)
  message(STATUS "Found EnTT")
endif()

#4 find spdlog -- couldn't be beother to define the 

if(DEFINED SPDLOG_PATH)
  message(STATUS "Usinf spdlog path specified in cmake")
  # include_directories(${SPDLOG_PATH})
  target_include_directories(${PROJECT_NAME} ${SPDLOG_PATH}/include)
elseif(NOT TARGET spdlog)
  find_package(spdlog REQUIRED)
else()
  message(FATAL_ERROR "Could not find EnTT. Please define path in .env or install library -- note to self: add git submodule??")
endif()

#5. find bullet 
# if(DEFINED BULLET_PATH)
#   message(STATUS "Bullet path finding hasn't been setup yet")
# else()
#   find_package(Bullet REQUIRED)
#   message(STATUS "Found Bullet :: not implemented")
#   # set(BULLET_LIBS BulletDynamics BulletCollision LinearMath)
#   # message(STATUS "dir::: ${BULLET_PHYSICS_SOURCE_DIR}")
#   # set(BULLET_EXECUTABLE ${BULLET_DI}/build3/bullet.rc)
#   message(STATUS "bullet physics source dir")
# endif()

include_directories(libs/tinyonjloader)
include_directories(libs/imgui)

# If TINYOBJ_PATH not specified in .env.cmake, try fetching from git repo
if (NOT TINYOBJ_PATH)
  message(STATUS "TINYOBJ_PATH not specified in .env.cmake, using libs/tinyobjloader")
  set(TINYOBJ_PATH libs/tinyobjloader)
endif()

if(NOT IMGUI_PATH)
  message(STATUS "imgui path not specified using libs/imgui")
  set(IMGUI_PATH libs/imgui)
else()
  message(STATUS "using imgui path specified in .env")
endif()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)


add_executable(${PROJECT_NAME} 
${SOURCES}
${IMGUI_PATH}/backends/imgui_impl_glfw.cpp 
${IMGUI_PATH}/backends/imgui_impl_vulkan.cpp 
${IMGUI_PATH}/imgui.cpp 
${IMGUI_PATH}/imgui_draw.cpp 
${IMGUI_PATH}/imgui_demo.cpp 
${IMGUI_PATH}/imgui_tables.cpp 
${IMGUI_PATH}/imgui_widgets.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:"DEBUG">)

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")

  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${TINYOBJ_PATH}
    ${GLFW_INCLUDE_DIRS}
    ${IMGUI_PATH}
    ${GLM_PATH}
    )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )

  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1 EnTT::EnTT spdlog::spdlog)

elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")

    if(APPLE)

        # this throws lots of warnings
        # set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework CoreVideo")

        # instead find the libs beforehand so we can link them properly instead of using flags
        message(STATUS "Finding additional apple libs :: removes need for xcode")
        find_library(COCOA_LIBRARY Cocoa)
        find_library(IOKIT_LIBRARY IOKit)
        find_library(COREVIDEO_LIBRARY CoreVideo)

        set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})

    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${TINYOBJ_PATH}
      ${IMGUI_PATH}
      ${IMGUI_PATH}/backends
    )

    # Referencing APPLE_LIBS is possible here because it returns an empty string otherwise
    target_link_libraries(${PROJECT_NAME} ${APPLE_LIBS} glfw ${Vulkan_LIBRARIES} EnTT::EnTT spdlog::spdlog)

endif()


############## Build SHADERS #######################

# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# get all .vert and .frag files in shaders directory
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

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)