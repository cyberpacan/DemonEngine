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


    int Application::start(unsigned int windowWidth, unsigned int windowHeight, const char* title)
    {
        mPWindow = std::make_unique<Window>(title, windowWidth, windowHeight);
        mPWindow->setEventCallback(
            [](Event& event)
            {
                INFO("[EVENT] Changed size to {0}x{1}", event.width, event.height);
            }
        );
        

        while (true)
        {
            mPWindow->onUpdate();
            onUpdate();
        }
        return 0;
        
    }
}