if(WIN32)
  include(./.env.windows.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
elseif(UNIX)
  include(./.env.unix.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
else()
  include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
endif()

message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME rush_vk)

if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in ${LOCAL_ENV}")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.0.1)

message(STATUS "")
message(STATUS "Locating non-compiled libraries")
message(STATUS "")

# If TINYOBJ_PATH not specified in .env.cmake, get from local copy
if(NOT GLM_PATH)
  message(STATUS "GLM_PATH not specified in ${LOCAL_ENV}. Using libs/glm")
  set(GLM_PATH libs/glm)
endif()

# If TINYOBJ_PATH not specified in .env.cmake, get from local copy
if (NOT TINYOBJ_PATH)
  message(STATUS "TINYOBJ_PATH not specified in ${LOCAL_ENV}. Using libs/tinyobjloader")
  set(TINYOBJ_PATH libs/tinyobjloader)
endif()

# If IMGUI path not specified in .env.cmake, get from local copy
if(NOT IMGUI_PATH)
  message(STATUS "imgui path not specified using libs/imgui")
  set(IMGUI_PATH libs/imgui)
else()
  message(STATUS "Using ImGui path specified in ${LOCAL_ENV}")
endif()

# If VMA path not specified in .env.cmake, get from local copy
if(NOT VMA_PATH)
  message(STATUS "VMA_PATH not specified in ${LOCAL_ENV}. Using libs/vma")
  set(VMA_INCLUDE libs/vma/include)
else()
  message(STATUS "Using VMA path specified in ${LOCAL_ENV}")
  set(VMA_INCLUDE ${VMA_PATH}/include)
endif()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

message(STATUS "Adding executables")

add_executable(
  ${PROJECT_NAME} 
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

message(STATUS "")
message(STATUS "Linking dynamic libraries")
message(STATUS "")


# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
  
  target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
  target_link_directories(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} vulkan-1)
  
elseif(NOT TARGET vulkan-1)
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan -- works on UNIX machines not windows
  message(STATUS "Found Vulkan package: $ENV{VULKAN_SDK}")
else()
  message(FATAL_ERROR "Could not find Vulkan. Please define VULKAN_SDK in ${LOCAL_ENV} or install dependency")
endif()

# 2. Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "GLFW path specified in ${LOCAL_ENV}")
  if (MSVC)
    add_subdirectory("${GLFW_PATH}")
    target_link_libraries(${PROJECT_NAME} glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL      "Ignore docs :: GLFW lib only" )
    set(GLFW_INSTALL OFF CACHE BOOL         "Ignore install :: GLFW lib only" )
    set(GLFW_BUILD_TESTS OFF CACHE BOOL     "Ignore test :: GLFW lib only" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "Ignore examples :: glfw lib only" FORCE)

    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_PATH}/include)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
    
    set(GLFW_LIB glfw)
    target_link_directories(${PROJECT_NAME} PRIVATE ${GLFW_LIB})

  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW TO BUILD GLFW")
    # set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64") # 2.1 make sure matches glfw mingw subdirectory

    add_subdirectory("${GLFW_PATH}")
    target_link_libraries(${PROJECT_NAME} glfw)
    
    set(GLFW_BUILD_DOCS OFF CACHE BOOL      "Ignore docs :: GLFW lib only" )
    set(GLFW_INSTALL OFF CACHE BOOL         "Ignore install :: GLFW lib only" )
    set(GLFW_BUILD_TESTS OFF CACHE BOOL     "Ignore test :: GLFW lib only" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "Ignore examples :: glfw lib only" FORCE)

    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_PATH}/include)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

  endif()
elseif(NOT TARGET glfw)
  find_package(glfw3 3.3 REQUIRED)
  message(STATUS "Found GLFW package at: ${GLFW_SOURCE_DIR}")
else()
  message(FATAL_ERROR "Could not find glfw. Please define path in ${LOCAL_ENV} or install dependency")
endif()

# 3 Set EnTT path in .env.cmake
if(DEFINED ENTT_PATH)
  message(STATUS "EnTT path specified in ${LOCAL_ENV}")
  add_subdirectory("${ENTT_PATH}")
  target_link_libraries(${PROJECT_NAME} EnTT::EnTT)
  target_include_directories(${PROJECT_NAME} PRIVATE ${ENTT_PATH}/single_include)
  target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_INCLUDE_NONE)
  message(STATUS "Using EnTT at: ${EnTT_SOURCE_DIR}")
elseif(NOT TARGET EnTT::EnTT)
  find_package(EnTT REQUIRED) #Throws error at fail
  message(STATUS "Found EnTT package at: ${EnTT_SOURCE_DIR}")
else()
  message(FATAL_ERROR "Could not find spdlog. Please define path in ${LOCAL_ENV} or install dependency")
endif()

#4 find spdlog
if(DEFINED SPDLOG_PATH)
  message(STATUS "spdlog path specified in ${LOCAL_ENV}")
  add_subdirectory("${SPDLOG_PATH}")
  target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
  target_include_directories(${PROJECT_NAME} PRIVATE ${SPDLOG_PATH}/include)
  target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_INCLUDE_NONE)
  message(STATUS "Using spdlog at: ${SPDLOG_PATH}")
elseif(NOT TARGET spdlog)
  find_package(spdlog REQUIRED)
  message(STATUS "Found spdlog package at: ${spdlog_DIR}")
else()
  message(FATAL_ERROR "Could not find spdlog. Please define path in ${LOCAL_ENV} or install dependency")
endif()


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
    ${TINYOBJ_PATH}
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
    ${VMA_INCLUDE}
    ${GLM_PATH}
    )

elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")

    if(APPLE)

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
      ${VMA_PATH}
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