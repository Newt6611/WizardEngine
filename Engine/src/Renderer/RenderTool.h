#pragma once
#include "Renderer.h"


namespace Wizard {

    class RenderTool
    {
    public:
        static IDeviceContext* GetDeviceContext()
        {
            return Renderer::Get()->GetDeviceContext();
        }  

        static std::shared_ptr<Shader> CreateShader(const std::string& name, const std::string& filepath_v, const std::string& filepath_p)
        {
            return Renderer::Get()->CreateShader(name, filepath_v, filepath_p);
        }

        static std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, uint32_t size, uint32_t vertices_count)
        {
            return Renderer::Get()->CreateVertexBuffer(data, size, vertices_count);
        }

        static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, int count)
        {
            return Renderer::Get()->CreateIndexBuffer(indices, count);
        }

        template<typename T>
        static std::shared_ptr<ConstantBuffer<T>> CreateConstantBuffer(const std::string& name)
        {
            return Renderer::Get()->CreateConstantBuffer<T>(name);
        }

        static ITextureView* GetCurrentBackBufferRTV()
        {
            return Renderer::Get()->GetSwapChain()->GetCurrentBackBufferRTV();
        }

        static ITextureView* GetDepthBufferDSV()
        {
            return Renderer::Get()->GetSwapChain()->GetDepthBufferDSV();
        }
    };
}