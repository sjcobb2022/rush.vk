#pragma once

//libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//std
#include <string>

namespace rush
{

    /**
     * @brief wrapper around glfw window
     */

    class vWindow
    {
    public:
        vWindow(int w, int h, std::string window_title);
        ~vWindow();


    private:
        

    
    };

} // utr