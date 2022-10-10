#pragma once
#include "Renderer.h"
#include "Core/Window.h"


namespace Wizard {

    class RenderTool
    {
    public:
        static IRenderDevice* GetDevice()
        {
            return Renderer::Get()->GetDevice();
        }

        static IDeviceContext* GetDeviceContext()
        {
            return Renderer::Get()->GetDeviceContext();
        }

        static ISwapChain* GetSwapChain()
        {
            return Renderer::Get()->GetSwapChain();
        }

        static Window* GetWindow()
        {
            return Window::Get();
        }

        static std::shared_ptr<Shader> CreateShader(const std::string& name, const std::string& filepath_v, const std::string& filepath_p)
        {
            return Renderer::Get()->CreateShader(name, filepath_v, filepath_p);
        }

        static std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t size)
        {
            return Renderer::Get()->CreateVertexBuffer(size);
        }

        static std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, uint32_t size, uint32_t vertices_count)
        {
            return Renderer::Get()->CreateVertexBuffer(data, size, vertices_count);
        }

        static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, int count)
        {
            return Renderer::Get()->CreateIndexBuffer(indices, count);
        }

        static std::shared_ptr<Texture2D> CreateTexture(const char* filepath, bool isRGB)
        {
            return Renderer::Get()->CreateTexture(filepath, isRGB);
        }

        static std::shared_ptr<Texture2D> CreateTexture(uint32_t width, uint32_t height, const void* data)
        {
            return Renderer::Get()->CreateTexture(width, height, data);
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

        static ITextureView* GetMousePickingView()
        {
            return Renderer::Get()->m_MousePickingView;
        }

        static uint32_t GetMousePointingEntity()
        {
            return Renderer::Get()->m_MousePointingEntity;
        }

        static int GetDrawCall()
        {
            return Renderer::Get()->GetDrawCall();
        }

        static void AddOneDrawCall()
        {
            Renderer::Get()->AddOneDrawCall();
        }

        static void ResetDrawCall()
        {
            Renderer::Get()->ResetDrawCall();
        }
    };
}