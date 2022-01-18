#include "app.hpp"

namespace volk
{
    
    App::App() {}

    App::~App() {}

    GLFWwindow *App::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        std::string windowName = "rush . vk";

        GLFWwindow *window = glfwCreateWindow(App::WIDTH, App::HEIGHT,
                                              windowName.c_str(),
                                              nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);

        return window;
    }

    void App::run()
    {

        GLFWwindow *window = initWindow();

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }

}