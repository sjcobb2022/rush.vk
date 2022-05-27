#pragma once

//libs
#define GLFW_INCLUDE_VULKAN
#include "rush_pch.hpp"

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