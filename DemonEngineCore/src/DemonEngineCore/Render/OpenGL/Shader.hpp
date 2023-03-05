#pragma once

namespace DemonEngine {

    class Shader
    {
    public:
        Shader(const char* vertexShaderSrc, const char* fragmentShaderSrc);
        Shader(Shader&&);
        Shader& operator=(Shader&&);
        ~Shader();

        Shader() = delete;
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void bind() const;
        static void unbind();
        bool isCompiled() const { return mIsCompiled; }

    private:
        bool mIsCompiled = false;
        unsigned int mID = 0;
    };

}