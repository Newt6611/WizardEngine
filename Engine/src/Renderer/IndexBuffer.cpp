#include "wzpch.h"
#include "IndexBuffer.h"

namespace Wizard {

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