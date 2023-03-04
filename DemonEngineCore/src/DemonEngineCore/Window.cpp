#include "DemonEngineCore/Window.hpp"
#include "DemonEngineCore/Logger.hpp"
#include "DemonEngineCore/Render/OpenGL/Shader.hpp"
#include "DemonEngineCore/Render/OpenGL/VertexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


namespace DemonEngine {

    static bool s_GLFW_initialized = false;

    

    GLfloat points[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    const char* vertexShader =
        R"(#version 330 core
        layout(location = 0) in vec4 vertexPos;
        layout(location = 1) in vec4 vertexColor;
        out vec4 color;
        void main() {
           color = vertexColor;
           gl_Position = vec4(vertexPos);
        })";

    const char* fragmentShader =
        R"(#version 330 core
        in vec4 color;
        out vec4 fragColor;
        void main() {
           fragColor = vec4(color);
        })";

    std::unique_ptr<Shader> p_shader;
    std::unique_ptr<VertexBuffer> p_points_vbo;
    std::unique_ptr<VertexBuffer> p_colors_vbo;
    GLuint VAO;


    Window::Window(std::string title, const unsigned int width, const unsigned int height):
        m_data({ std::move(title), width, height })
    {
        int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
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

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                glViewport(0, 0, width, height);
            }
        );


        p_shader = std::make_unique<Shader>(vertexShader, fragmentShader);
        if (!p_shader->isCompiled())
        {
            return false;
        }

        p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points));
        p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors));

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(0);
        p_points_vbo->bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        p_points_vbo->bind();
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
    }

    void Window::shutdown()
    {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::onUpdate()
    {
        glClearColor(m_bgc[0], m_bgc[1], m_bgc[2], m_bgc[3]);
        glClear(GL_COLOR_BUFFER_BIT);


        p_shader->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);




        ImGuiIO& IO = ImGui::GetIO();
        IO.DisplaySize.x = static_cast<float>(get_width());
        IO.DisplaySize.y = static_cast<float>(get_height());


        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("BackGroundColor WINDOW");
        ImGui::ColorEdit4("BackGroundColor", m_bgc);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

}