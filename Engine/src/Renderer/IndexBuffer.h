#pragma once

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsEngine/interface/Buffer.h"

namespace Wizard {
    using namespace Diligent;
    class IndexBuffer 
    {
    public:
        IndexBuffer(uint32_t* indices, int count, IRenderDevice* device);
        ~IndexBuffer();

        inline int GetCount() { return m_Count; }
        inline IBuffer* GetBuffer() { return m_Buffer; }

    private:
        int m_Count;
        RefCntAutoPtr<IBuffer> m_Buffer;
    };
}