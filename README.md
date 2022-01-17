# Volk

## Table of Contents
- [Building](#Building)
  - [Building for Unix](#UnixBuild)
  - [Building for MacOS](#MacOSBuild)
  - [Building for Windows](#WindowsBuild)
- [Tutorials](#Tutorials)
  - [Basics](#Basics)
  - [Point Lights](#PointLights)
- [Official Khronos Vulkan Samples](#Khronossamples)
- [Credits and Attributions](#CreditsAttributions)
    - [Libraries](#Libraries)
    - [People and Groups](#PeopleAndGroups)

## <a name="Building"></a> Building

### <a name="UnixBuild"></a> Unix Build Instructions

- Install the dependencies: cmake, glm, vulkan and glfw

- For example
  ```
    sudo apt install vulkan-tools
    sudo apt install libvulkan-dev
    sudo apt install vulkan-validationlayers-dev spirv-tools
    sudo apt install libglfw3-dev
    sudo apt install libglm-dev
    sudo apt install cmake
  ```
- To Build
    - Shell
        ```
        cd volk
        ./build.sh
        ```
    - Vscode build
        - Run the defualt build command from the build.json file
        - Run 


### <a name="MacOSBuild"></a> MacOS Build Instructions

#### Install Dependencies
- Note that installing xcode is not necessary here since the Apple framework libraries are automatically included.
- [Download and install MacOS Vulkan sdk](https://vulkan.lunarg.com/)
- [Download and install Homebrew](https://brew.sh/)

- Then in a terminal window

  ```
    brew install cmake
    brew install glfw
    brew install glm
  ```
 - Additionally for this renderer, a few extra steps are required.
  - Simply clone spdlog, imgui and Vulkan Memory Allocator into the home directory and Cmake should be able to find everything that is needed.

- To Build
      - Shell
        ```
        cd volk
        ./build.sh
        ```
    - Vscode build
        - Run the defualt build command from the build.json file
        - Run the executable

### <a name="WindowsBuild"></a> Windows build instructions

- [Download and install Windows Vulkan sdk](https://vulkan.lunarg.com/)
- [Download and install Windows cmake x64 installer](https://cmake.org/download/)
  - Make sure to select "Add cmake to the system Path for all users"
- [Download GLFW](https://www.glfw.org/download.html) (64-bit precompiled binary)
- [Download GLM](https://github.com/g-truc/glm/releases)
- Download and open the project and rename "envWindowsExample.cmake" to ".env.cmake"
- Update the filepath variables in .env.cmake to your installation locations

#### Building for Visual Studio 2019

- In windows powershell

```
 cd volk
 mkdir build
 cmake -S . -B .\build\
```

- In VSCode
    - Run default build command

- If cmake finished successfully, it will create a volk.sln file in the build directory that can be opened with visual studio. In visual studio right click the Shaders project -> build, to build the shaders. Right click the volk project -> set as startup project. Change from debug to release, and then build and start without debugging.

## <a name="Khronossamples"></a> Official Khronos Vulkan Samples

Khronos made an official Vulkan Samples repository available to the public ([press release](https://www.khronos.org/blog/vulkan-releases-unified-samples-repository?utm_source=Khronos%20Blog&utm_medium=Twitter&utm_campaign=Vulkan%20Repository)).

You can find this repository at https://github.com/KhronosGroup/Vulkan-Samples

## <a name="CreditsAttributions"></a> Credits

Mainly thanks to [BlurryPiano](https://github.com/blurrypiano/) (Brendan Galea) for making a really incredible and educational way to get into developing for vulkan, making a major part of this codebase, and being a really nice guy.

### <a name="Libraries"></a> Libraries
Thanks to the authors of these libraries :

- [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)
- [TinyObjLoader](https://github.com/tinyobjloader/tinyobjloader)
- [EnTT](https://github.com/skypjack/entt)

### <a name="PeopleAndGroups"></a> People and Groups

[LunarG](http://www.lunarg.com)

Thanks to [Sascha Willems](https://github.com/SaschaWillems/Vulkan)

Thanks to [ThinMatrix](https://www.youtube.com/user/ThinMatrix/featured)

## Attributions / Licenses

- Vulkan and the Vulkan logo are trademarks of the [Khronos Group Inc.](http://www.khronos.org)