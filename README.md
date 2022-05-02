# rush.vk

<br/>

[![windows](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-windows.yml/badge.svg)](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-windows.yml)
[![linux](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-linux.yml/badge.svg)](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-linux.yml)
[![mac](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-mac.yml/badge.svg)](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-mac.yml)



## Table of Contents
- [Build](#Building)
  - [Building for Unix](#UnixBuild)
  - [Building for MacOS](#MacOSBuild)
  - [Building for Windows](#WindowsBuild)
- [Credits and Attributions](#CreditsAttributions)
    - [Libraries](#Libraries)
    - [People and Groups](#PeopleAndGroups)

## <a name="Building"></a> Building

<br/>

> Remember to `git clone --recursive https://github.com/sjcobb2022/rush.vk.git`

<br/>

### <a name="UnixBuild"></a> Unix Build Instructions

- Install the dependencies: cmake, glm, vulkan and glfw, spdlog

- For example
  ```
  sudo apt install vulkan-tools
  sudo apt install libvulkan-dev
  sudo apt install vulkan-validationlayers-dev spirv-tools
  sudo apt install cmake
  ```

- To Build
    - Shell
        ```
        cmake -S . -B ./build
        cmake --build .
        ```


### <a name="MacOSBuild"></a> MacOS Build Instructions

#### Install Dependencies
- Note that installing xcode is not necessary here since the Apple framework libraries are automatically included.
- [Download and install MacOS Vulkan sdk](https://vulkan.lunarg.com/)

- Install cmake
  ```brew install cmake```
  ```or [Download](https://cmake.org/download/)```

- To Build
  - cmake
    ```
    cmake -S . -B ./build
    cmake --build .
    ```

### <a name="WindowsBuild"></a> Windows build instructions

- [Download and install the Vulkan SDK](https://vulkan.lunarg.com/)
- [Download and install CMake](https://cmake.org/download/)
  - Make sure to select "Add cmake to the system Path for all users"
- Download and open the project and rename ".env.windows.example.cmake" to ".env.windows.cmake"
- Update the filepath variables in .env.windows.cmake to your installation locations

#### Building for Visual Studio 2019

- In windows powershell
  ```
  cd rush.vk
  cmake -S . -B .\build\
  cmake --build .
  ```

> **Noted** All dependencies are locally included as submodules, no need to install the dependencies.

- If cmake finished successfully, it will create a rush.vk.sln file in the build directory that can be opened with visual studio. In visual studio right click the Shaders project -> build, to build the shaders. Right click the rush.vk project -> set as startup project. Change from debug to release, and then build and start without debugging.

You can find this repository at https://github.com/KhronosGroup/Vulkan-Samples

## <a name="CreditsAttributions"></a> Credits

Mainly thanks to [BlurryPiano](https://github.com/blurrypiano/) (Brendan Galea) for making a really incredible and educational pathway into vulkan dev, making a major part of this codebase, and being a really nice guy.

### <a name="Libraries"></a> Libraries
Thanks to the authors of these libraries :

- [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)
- [TinyObjLoader](https://github.com/tinyobjloader/tinyobjloader)
- [EnTT](https://github.com/skypjack/entt)
- [spdlog](https://github.com/gabime/spdlog)
- [GLFW](https://github.com/glfw/glfw)

### <a name="PeopleAndGroups"></a> People and Groups

[LunarG](http://www.lunarg.com)

Thanks to [Sascha Willems](https://github.com/SaschaWillems/Vulkan)

Thanks to [ThinMatrix](https://www.youtube.com/user/ThinMatrix/featured)

## Attributions / Licenses

- Vulkan and the Vulkan logo are trademarks of the [Khronos Group Inc.](http://www.khronos.org)
