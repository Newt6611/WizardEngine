#pragma once

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsEngine/interface/Buffer.h"


namespace Wizard {
    using namespace Diligent;

    class VertexBuffer
    {
    public:
        VertexBuffer(void* data, uint32_t size, uint32_t count, IRenderDevice* device);

        ~VertexBuffer();

        inline uint32_t GetVerticeCount() { return m_VerticeCount; }
        inline IBuffer* GetBuffer() { return m_Buffer; }
    private:
        int m_VerticeCount;
        RefCntAutoPtr<IBuffer> m_Buffer;
    };
}