#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <DemonEngineCore/Application.hpp>

using namespace std;

namespace DemonEngine
{
	Application::Application()
	{

	}
	Application::~Application()
	{

	}

	int Application::Start(unsigned int winWidth, unsigned int winHeight, const char* winTitle)
	{
        
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return -1;
        }

        glClearColor(1, 1, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            onUpdate();
        }

        glfwTerminate();
        return 0;
	}

	
}