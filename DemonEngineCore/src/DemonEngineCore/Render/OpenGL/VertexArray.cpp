#include "VertexArray.hpp"

#include "DemonEngineCore/Logger.hpp"

#include <glad/glad.h>

namespace DemonEngine {

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &mID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &mID);
    }

    VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
    {
        mID = vertexArray.mID;
        mElementsCount = vertexArray.mID;
        vertexArray.mID = 0;
        vertexArray.mElementsCount = 0;
        return *this;
    }

    VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
        : mID(vertexArray.mID)
        , mElementsCount(vertexArray.mElementsCount)
    {
        vertexArray.mID = 0;
        vertexArray.mElementsCount = 0;
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(mID);
    }

    void VertexArray::unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer)
    {
        bind();
        vertexBuffer.bind();

        for (const BufferElement& currentElement : vertexBuffer.getLayout().getElements())
        {
            glEnableVertexAttribArray(mElementsCount);
            glVertexAttribPointer(
                mElementsCount,
                static_cast<GLint>(currentElement.componentsCount),
                currentElement.componentType,
                GL_FALSE,
                static_cast<GLsizei>(vertexBuffer.getLayout().getStride()),
                reinterpret_cast<const void*>(currentElement.offset)
            );
            ++mElementsCount;
        }
    }

    void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer)
    {
        bind();
        indexBuffer.bind();
        mIndicesCount = indexBuffer.getCount();
    }
}