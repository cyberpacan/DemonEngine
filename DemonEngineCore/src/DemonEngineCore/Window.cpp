#include "DemonEngineCore/Window.hpp"
#include "DemonEngineCore/Logger.hpp"
#include "DemonEngineCore/Render/OpenGL/Shader.hpp"
#include "DemonEngineCore/Render/OpenGL/VertexBuffer.hpp"
#include "DemonEngineCore/Render/OpenGL/VertexArray.hpp"
#include "DemonEngineCore/Render/OpenGL/IndexBuffer.hpp"

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


namespace DemonEngine {

    static bool sInitializedGLFW = false;

    
    GLfloat positionColor[] = {
           -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
           -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    const char* vertexShader =
        R"(#version 330 core
        layout(location = 0) in vec4 vertexPos;
        layout(location = 1) in vec4 vertexColor;
        uniform mat4 modelMat;
        out vec4 color;
        void main() {
           color = vertexColor;
           gl_Position = modelMat * vec4(vertexPos);
        })";

    const char* fragmentShader =
        R"(#version 330 core
        in vec4 color;
        out vec4 fragColor;
        void main() {
           fragColor = vec4(color);
        })";

    std::unique_ptr<Shader> pShader;
    

    std::unique_ptr<VertexBuffer> pPositionColorVBO;
    std::unique_ptr<IndexBuffer> pIndexBuffer;
    std::unique_ptr<VertexArray> pVAO;

    float scale[3] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f, 0.f, 0.f };


    Window::Window(std::string title, const unsigned int width, const unsigned int height):
        mData({ std::move(title), width, height })
    {
        int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(mPWindow, true);
    }

    Window::~Window()
    {
        shutdown();
    }

    int Window::init()
    {
        INFO("Creating window '{0}' with size {1}-{2}", mData.title, mData.width, mData.height);

        if (!sInitializedGLFW)
        {
            if (!glfwInit())
            {
                CRITICAL("Cant initialize GLFW");
                return -1;
            }
            sInitializedGLFW = true;
        }

        mPWindow = glfwCreateWindow(mData.width, mData.height, mData.title.c_str(), nullptr, nullptr);
        if (!mPWindow)
        {
            CRITICAL("Cant create window {0} with size {1}-{2}", mData.title, mData.width, mData.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(mPWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            CRITICAL("Failed to initialize GLAD");
            return -3;
        }


        glfwSetWindowUserPointer(mPWindow, &mData);

        glfwSetWindowSizeCallback(mPWindow,
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

        glfwSetFramebufferSizeCallback(mPWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                glViewport(0, 0, width, height);
            }
        );


        pShader = std::make_unique<Shader>(vertexShader, fragmentShader);
        if (!pShader->isCompiled())
        {
            return false;
        }

        BufferLayout buffer_layout_1vec3
        {
            ShaderDataType::Float3
        };

        

        BufferLayout buffer_layout_2vec3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };


        pVAO = std::make_unique<VertexArray>();
        pPositionColorVBO = std::make_unique<VertexBuffer>(positionColor, sizeof(positionColor), buffer_layout_2vec3);
        pIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        pVAO->addVertexBuffer(*pPositionColorVBO);
        pVAO->setIndexBuffer(*pIndexBuffer);

        return 0;
    }

    void Window::shutdown()
    {
        glfwDestroyWindow(mPWindow);
        glfwTerminate();
    }

    void Window::onUpdate()
    {
        glClearColor(mBGC[0], mBGC[1], mBGC[2], mBGC[3]);
        glClear(GL_COLOR_BUFFER_BIT);







        ImGuiIO& IO = ImGui::GetIO();
        IO.DisplaySize.x = static_cast<float>(getWidth());
        IO.DisplaySize.y = static_cast<float>(getHeight());


        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("BackGroundColor WINDOW");
        ImGui::ColorEdit4("BackGroundColor", mBGC);

        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("translate", translate, -1.f, 1.f);

        pShader->bind();

        glm::mat4 scaleMat(scale[0], 0, 0, 0,
                            0, scale[1], 0, 0,
                            0, 0, scale[2], 0,
                            0, 0, 0, 1);

        float rotate_in_radians = glm::radians(rotate);
        glm::mat4 rotateMat(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
            -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translateMat(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1);

        glm::mat4 modelMat = translateMat * rotateMat * scaleMat;
        pShader->setMatrix4("modelMat", modelMat);


        pVAO->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(pVAO->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
        
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       

        glfwSwapBuffers(mPWindow);
        glfwPollEvents();
    }

}