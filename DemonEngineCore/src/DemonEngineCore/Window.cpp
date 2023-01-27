#include "DemonEngineCore/Window.hpp"
#include "DemonEngineCore/Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DemonEngine {

    static bool s_GLFW_initialized = false;

    Window::Window(std::string title, const unsigned int width, const unsigned int height):
        m_data({ std::move(title), width, height })
    {
        int resultCode = init();
    }

    Window::~Window()
    {
        shutdown();
    }

    int Window::init()
    {
        INFO("Creating window '{0}' with size {1}-{2}", m_data.title, m_data.width, m_data.height);

        if (!s_GLFW_initialized)
        {
            if (!glfwInit())
            {
                CRITICAL("Cant initialize GLFW");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            CRITICAL("Cant create window {0} with size {1}-{2}", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            CRITICAL("Failed to initialize GLAD");
            return -3;
        }


        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;

                Event event;
                event.width = width;
                event.height = height;
                data.eventCallback(event);
            }
        );


        return 0;
    }

    void Window::shutdown()
    {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::onUpdate()
    {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

}