#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace DemonEngine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertexBuffer) noexcept;
        VertexArray(VertexArray&& vertexBuffer) noexcept;

        void addVertexBuffer(const VertexBuffer& vertexBuffer);
        void setIndexBuffer(const IndexBuffer& indexBuffer);
        void bind() const;
        static void unbind();
        size_t getIndicesCount() const { return mIndicesCount; }

    private:
        unsigned int mID = 0;
        unsigned int mElementsCount = 0;
        size_t mIndicesCount = 0;
    };

}