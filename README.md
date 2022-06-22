# rush.vk

<br/>

[![windows](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-windows.yml/badge.svg)](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-windows.yml)
[![linux](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-linux.yml/badge.svg)](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-linux.yml)
[![mac](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-mac.yml/badge.svg)](https://github.com/sjcobb2022/rush.vk/actions/workflows/build-mac.yml)



## Table of Contents
- [Build](#Building)
  - [Building for Unix](#Linux)
  - [Building for MacOS](#MacOS)
  - [Building for Windows](#Windows)
- [Credits and Attributions](#CreditsAttributions)
    - [Libraries](#Libraries)
    - [People and Groups](#PeopleAndGroups)

## <a name="Building"></a> Building

<br/>

> Remember to `git clone --recursive https://github.com/sjcobb2022/rush.vk.git`

### <a name="Linux"></a> Unix Build Instructions

- Install the dependencies: cmake, vulkan, glfw

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


### <a name="MacOS"></a> MacOS Build Instructions

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

### <a name="Windows"></a> Windows build instructions

- [Download and install the Vulkan SDK](https://vulkan.lunarg.com/)
- [Download and install CMake](https://cmake.org/download/)
  - Make sure to select "Add cmake to the system Path for all users"
- If Vulkan is not in the default location, add .env.cmake and point cmake in the right direction to find the vulkan lib
#### Building for Visual Studio 2019

- In windows powershell
  ```
  cd rush.vk
  cmake -S . -B .\build\
  cmake --build .\buidld\
  ```

> **Noted** All dependencies are locally included as submodules, no need to install the dependencies.

- If cmake finished successfully, it will create a rush.vk.sln file in the build directory that can be opened with visual studio. In visual studio right click the Shaders project -> build, to build the shaders. Right click the rush.vk project -> set as startup project. Change from debug to release, and then build and start without debugging.

You can find this repository at https://github.com/KhronosGroup/Vulkan-Samples

## <a name="CreditsAttributions"></a> Credits

Mainly thanks to [BlurryPiano](https://github.com/blurrypiano/) (Brendan Galea) for making a really incredible and educational pathway into vulkan dev, making a major part of this codebase, and being a really nice guy.

### <a name="Libraries"></a> Libraries
Thanks to the authors of these libraries :

- [Dear ImGui](https://github.com/ocornut/imgui)
- [EnTT](https://github.com/skypjack/entt)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [KTX](tps://github.com/KhronosGroup/KTX-Software)
- [spdlog](https://github.com/gabime/spdlog)
- [TinyGltf](https://github.com/syoyo/tinygltf)
- [TinyObjLoader](https://github.com/tinyobjloader/tinyobjloader)
- [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)


### <a name="PeopleAndGroups"></a> People and Groups

Thanks to [LunarG](http://www.lunarg.com)

Thanks to [Sascha Willems](https://github.com/SaschaWillems/Vulkan)

Thanks to [ThinMatrix](https://www.youtube.com/user/ThinMatrix)

## Attributions / Licenses

- Vulkan and the Vulkan logo are trademarks of the [Khronos Group Inc.](http://www.khronos.org)
