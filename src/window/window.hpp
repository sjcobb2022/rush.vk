#pragma once

//libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//std
#include <string>

namespace utr
{

    /**
     * @brief wrapper around glfw window
     */

    class UtrWindow
    {
    public:
        UtrWindow(int w, int h, std::string window_title);
        ~UtrWindow();


    private:
        

    
    };

} // utr