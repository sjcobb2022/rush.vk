#pragma once

// libs
#include "rush_pch.hpp"

namespace rush
{
    /**
     * @brief Initialise window and place it into the deletion queue.
     *
     * @param width Width of the window
     * @param height Height of the window
     * @param window_title Title of the window
     * @param monitor GLFWMonitor to focus on, default: nullptr
     * @param share GLFWindow* to focus on, default: nullptr
     * @return GLFWwindow*
     */
    GLFWwindow *window_init(int w, int h, std::string window_title, GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);

    void window_createSurface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface);

    void window_framebufferResizeCallback(GLFWwindow *window, int width, int height);
} // rush