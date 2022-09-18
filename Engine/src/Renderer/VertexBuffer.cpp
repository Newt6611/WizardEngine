#include "wzpch.h"
#include "VertexBuffer.h"

namespace Wizard {
    VertexBuffer::VertexBuffer(void* data, uint32_t size, uint32_t vertice_count, IRenderDevice* device)
        : m_VerticeCount(vertice_count)
    {
        BufferDesc buffDesc;
        buffDesc.Name      = "Vertex Buffer";
        buffDesc.Usage     = USAGE_IMMUTABLE;
        buffDesc.BindFlags = BIND_VERTEX_BUFFER;
        buffDesc.Size      = size;

        BufferData bufferData;
        bufferData.pData = data;
        bufferData.DataSize = size;
        device->CreateBuffer(buffDesc, &bufferData, &m_Buffer);
    }

    VertexBuffer::~VertexBuffer()
    {

    }
}