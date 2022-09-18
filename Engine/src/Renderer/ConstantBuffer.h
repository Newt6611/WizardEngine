#pragma once
#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsEngine/interface/Buffer.h"

namespace Wizard {
    using namespace Diligent;

    template<typename T>
    class ConstantBuffer
    {
    public:
        ConstantBuffer(const std::string& name, IRenderDevice* device)
        {
            BufferDesc bufferDesc;
            bufferDesc.Name = std::string(name + " uniform buffer").c_str();
            bufferDesc.BindFlags = BIND_UNIFORM_BUFFER;
            bufferDesc.Usage = USAGE_DYNAMIC;
            bufferDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
            bufferDesc.Size = sizeof(T);
            device->CreateBuffer(bufferDesc, nullptr, &m_ConstantBuffer);
        }

        ~ConstantBuffer() {}

        IBuffer* GetConstantBuffer() { return m_ConstantBuffer; }

    private:
        RefCntAutoPtr<IBuffer> m_ConstantBuffer;
    };
}