#include "DemonEngineCore/Application.hpp"
#include "DemonEngineCore/Window.hpp"
#include "DemonEngineCore/Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include <iostream>

namespace DemonEngine
{

    Application::Application()
    {
        INFO("Starting Application");
    }


    Application::~Application()
    {

        INFO("Closing Application");
    }


    int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
    {
        m_pWindow = std::make_unique<Window>(title, window_width, window_height);
        m_pWindow->setEventCallback(
            [](Event& event)
            {
                INFO("[EVENT] Changed size to {0}x{1}", event.width, event.height);
            }
        );
        

        while (true)
        {
            m_pWindow->onUpdate();
            onUpdate();
        }
        return 0;
        
    }
}