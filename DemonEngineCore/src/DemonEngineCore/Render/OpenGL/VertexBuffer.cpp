#include "VertexBuffer.hpp"

#include "DemonEngineCore/Logger.hpp"

#include <glad/glad.h>

namespace DemonEngine {

    constexpr unsigned int ShaderDataType2componentCount(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
            return 1;

        case ShaderDataType::Float2:
        case ShaderDataType::Int2:
            return 2;

        case ShaderDataType::Float3:
        case ShaderDataType::Int3:
            return 3;

        case ShaderDataType::Float4:
        case ShaderDataType::Int4:
            return 4;
        }

        ERROR("ShaderDataType2componentsCount: unknown ShaderDataType");
        return 0;
    }


    constexpr size_t ShaderDataTypeSize(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return sizeof(GLfloat) * ShaderDataType2componentCount(type);

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return sizeof(GLint) * ShaderDataType2componentCount(type);
        }

        ERROR("ShaderDataTypeSize: unknown ShaderDataType");
        return 0;
    }


    constexpr unsigned int ShaderDataType2componentType(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        }

        ERROR("ShaderDataType2componentType: unknown ShaderDataType");
        return GL_FLOAT;
    }

    constexpr GLenum Usage2GLenum(const VertexBuffer::EUsage usage)
    {
        switch (usage)
        {
        case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
        case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
        case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    BufferElement::BufferElement(const ShaderDataType _type)
        : type(_type)
        , componentType(ShaderDataType2componentType(_type))
        , componentsCount(ShaderDataType2componentCount(_type))
        , size(ShaderDataTypeSize(_type))
        , offset(0)
    {
    }


    VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage)
        : mBufferLayout(std::move(buffer_layout))
    {
        glGenBuffers(1, &mID);
        glBindBuffer(GL_ARRAY_BUFFER, mID);
        glBufferData(GL_ARRAY_BUFFER, size, data, Usage2GLenum(usage));
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &mID);
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
    {
        mID = vertexBuffer.mID;
        vertexBuffer.mID = 0;
        return *this;
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
        : mID(vertexBuffer.mID)
        , mBufferLayout(std::move(vertexBuffer.mBufferLayout))
    {
        vertexBuffer.mID = 0;
        
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, mID);
    }

    void VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}