#include "Shader.hpp"

#include "DemonEngineCore/Logger.hpp"

#include <glad/glad.h>

namespace DemonEngine
{
    bool createShader(const char* source, const GLenum shaderType, GLuint& shaderId)
    {
        shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);

        GLint success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

            CRITICAL("shader compilation error: {}", infoLog);
            return false;
        }
        return true;
    }


    Shader::Shader(const char* vertexShaderSRC, const char* fragmentShaderSRC)
    {
        GLuint vertexShaderID = 0;
        if (!createShader(vertexShaderSRC, GL_VERTEX_SHADER, vertexShaderID))
        {
            CRITICAL("VERTEX SHADER: compile time error!");
            glDeleteShader(vertexShaderID);
            return;
        }

        GLuint fragmentShaderID = 0;
        if (!createShader(fragmentShaderSRC, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            CRITICAL("FRAGMENT SHADER: compile time error!");
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            return;
        }

        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShaderID);
        glAttachShader(m_id, fragmentShaderID);
        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);
            CRITICAL("SHADER: link time error: {0}", infoLog);
            glDeleteProgram(m_id);
            m_id = 0;
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            return;
        }
        else
        {
            m_isCompiled = true;
        }

        glDetachShader(m_id, vertexShaderID);
        glDetachShader(m_id, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_id);
    }

    void   Shader::bind() const
    {
        glUseProgram(m_id);
    }

    void   Shader::unbind()
    {
        glUseProgram(0);
    }

    Shader& Shader::operator=(Shader&& shaderProg)
    {
        glDeleteProgram(m_id);
        m_id = shaderProg.m_id;
        m_isCompiled = shaderProg.m_isCompiled;

        shaderProg.m_id = 0;
        shaderProg.m_isCompiled = false;
        return *this;
    }

    Shader::Shader(Shader&& shaderProg)
    {
        m_id = shaderProg.m_id;
        m_isCompiled = shaderProg.m_isCompiled;

        shaderProg.m_id = 0;
        shaderProg.m_isCompiled = false;
    }
}