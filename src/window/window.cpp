
#include "window.hpp"

namespace rush
{

    GLFWwindow *window_init(int width, int height, std::string window_title, GLFWmonitor *monitor, GLFWwindow *share)
    {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        GLFWwindow *window = glfwCreateWindow(
            width,
            height,
            window_title.c_str(),
            monitor,
            share);

        glfwSetFramebufferSizeCallback(window, window_framebufferResizeCallback);

        return window;
    }

    void window_createSurface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to craete window surface");
        }
    }

    void window_framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        glfwSetWindowSize(window, width, height);
    }

}