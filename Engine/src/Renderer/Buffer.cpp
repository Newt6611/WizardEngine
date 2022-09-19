#include "wzpch.h"
#include "Buffer.h"

namespace Wizard {

    // Vertex Buffer
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



    // Index Buffer
    IndexBuffer::IndexBuffer(uint32_t* indices, int count, IRenderDevice* device) 
        : m_Count(count)
    {
        BufferDesc buffDesc;
        buffDesc.Name      = "Index Buffer";
        buffDesc.Usage     = USAGE_IMMUTABLE;
        buffDesc.BindFlags = BIND_INDEX_BUFFER;
        buffDesc.Size      = sizeof(uint32_t) * count;

        BufferData bufferData;
        bufferData.pData    = indices;
        bufferData.DataSize = sizeof(uint32_t) * count;
        device->CreateBuffer(buffDesc, &bufferData, &m_Buffer);
    }

    IndexBuffer::~IndexBuffer()
    {

    }
}