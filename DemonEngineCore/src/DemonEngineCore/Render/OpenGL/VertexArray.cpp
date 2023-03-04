#include "VertexArray.hpp"

#include "DemonEngineCore/Logger.hpp"

#include <glad/glad.h>

namespace DemonEngine {

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
    {
        m_id = vertexArray.m_id;
        m_elementsCount = vertexArray.m_id;
        vertexArray.m_id = 0;
        vertexArray.m_elementsCount = 0;
        return *this;
    }

    VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
        : m_id(vertexArray.m_id)
        , m_elementsCount(vertexArray.m_elementsCount)
    {
        vertexArray.m_id = 0;
        vertexArray.m_elementsCount = 0;
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(const VertexBuffer& vertex_buffer)
    {
        bind();
        vertex_buffer.bind();

        //TODO - use buffer layout
        glEnableVertexAttribArray(m_elementsCount);
        glVertexAttribPointer(m_elementsCount, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        ++m_elementsCount;
    }
}