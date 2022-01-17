#include "app.hpp"

namespace utr
{

    App::App() {}

    App::~App() {}

    GLFWwindow *App::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        std::string windowName = "UnTitled Renderer";

        GLFWwindow *window = glfwCreateWindow(800, 600,
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