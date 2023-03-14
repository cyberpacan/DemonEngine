#include "Shader.hpp"

#include "DemonEngineCore/Logger.hpp"
#include <glm/gtc/type_ptr.hpp>
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


    Shader::Shader(const char* vertexShaderSrc, const char* fragmentShaderSrc)
    {
        GLuint vertexShaderID = 0;
        if (!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderID))
        {
            CRITICAL("VERTEX SHADER: compile time error!");
            glDeleteShader(vertexShaderID);
            return;
        }

        GLuint fragmentShaderID = 0;
        if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            CRITICAL("FRAGMENT SHADER: compile time error!");
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            return;
        }

        mID = glCreateProgram();
        glAttachShader(mID, vertexShaderID);
        glAttachShader(mID, fragmentShaderID);
        glLinkProgram(mID);

        GLint success;
        glGetProgramiv(mID, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(mID, 1024, nullptr, infoLog);
            CRITICAL("SHADER: link time error: {0}", infoLog);
            glDeleteProgram(mID);
            mID = 0;
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            return;
        }
        else
        {
            mIsCompiled = true;
        }

        glDetachShader(mID, vertexShaderID);
        glDetachShader(mID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    Shader::~Shader()
    {
        glDeleteProgram(mID);
    }

    void   Shader::bind() const
    {
        glUseProgram(mID);
    }

    void   Shader::unbind()
    {
        glUseProgram(0);
    }

    Shader& Shader::operator=(Shader&& shaderProg) 
    {
        glDeleteProgram(mID);
        mID = shaderProg.mID;
        mIsCompiled = shaderProg.mIsCompiled;

        shaderProg.mID = 0;
        shaderProg.mIsCompiled = false;
        return *this;
    }

    Shader::Shader(Shader&& shaderProg) 
    {
        mID = shaderProg.mID;
        mIsCompiled = shaderProg.mIsCompiled;

        shaderProg.mID = 0;
        shaderProg.mIsCompiled = false;
    }

    void Shader::setMatrix4(const char* name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(mID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
}
