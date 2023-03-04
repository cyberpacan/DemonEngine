#pragma once
#include "VertexBuffer.hpp"

namespace DemonEngine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertexBuffer) noexcept;
        VertexArray(VertexArray&& vertexBuffer) noexcept;

        void addBuffer(const VertexBuffer& vertexBuffer);
        void bind() const;
        static void unbind();

    private:
        unsigned int m_id = 0;
        unsigned int m_elementsCount = 0;
    };

}