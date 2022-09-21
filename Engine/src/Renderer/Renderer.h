#pragma once

#include "Core.h"

#if defined(WZ_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32
    #ifdef WZ_D3D11
        #include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
    #endif
    #ifdef WZ_D3D12
        #include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
    #endif
    #ifdef WZ_VULKAN
        #include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
    #endif
    #ifdef WZ_GL
        #include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
    #endif

#elif defined(WZ_APPLE)
    #define PLATFORM_MACOS 1
    #ifdef WZ_METAL
        #include "Graphics/GraphicsEngineMetal/interface/EngineFactoryMtl.h"
    #endif
    #ifdef WZ_VULKAN
        #include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
    #endif
#elif defined(WZ_LINUX)
    #define PLATFORM_LINUX 1
    #define GLFW_EXPOSE_NATIVE_X11
    #ifdef WZ_VULKAN
        #include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
    #endif
#endif

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include "Graphics/GraphicsEngine/interface/EngineFactory.h"

#include "Graphics/GraphicsEngine/interface/Texture.h"
#include "Graphics/GraphicsEngine/interface/TextureView.h"

#include "Graphics/GraphicsTools/interface/MapHelper.hpp"

#include "Common/interface/RefCntAutoPtr.hpp"
#include "TextureLoader/interface/TextureLoader.h"
#include "TextureLoader/interface/TextureUtilities.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderPipelineState.h"
#include "Shader.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"

using namespace Diligent;

namespace Wizard {
    
    class Window;

    enum class RendererAPI {
        None   = (1 << 0),
        Vulkan = (1 << 1),
        D3D11  = (1 << 2),
        D3D12  = (1 << 3),
        OpenGL = (1 << 4),
        Metal  = (1 << 5)
    };

    class Renderer
    {
    public:
        static Renderer* Get() {
            static Renderer INSTANCE;
            return &INSTANCE;
        }

        Renderer() = default;
        Renderer(const Renderer&) = delete;
        virtual ~Renderer() { }

        void Init(RendererAPI api, Window* window);
        void Shutdown();

        inline const RendererAPI GetAPI() { return m_Api; }

        inline const std::string& GetAPIName() { return m_ApiName; }

        inline void ResetDrawCall() { m_DrawCall = 0; }
        inline void AddOneDrawCall() { ++m_DrawCall; }
        inline int GetDrawCall() { return m_DrawCall; } 


        inline IRenderDevice* GetDevice() { return m_Device; }
        inline IDeviceContext* GetDeviceContext() { return m_DeviceContext; }
        inline ISwapChain* GetSwapChain() { return m_SwapChain; }
        inline IEngineFactory* GetEngineFactory() { return m_EngineFactory; }

        std::shared_ptr<Shader> CreateShader(const std::string& name, const std::string& filepath_v, const std::string& filepath_p)
        {
            return std::make_shared<Shader>(name, filepath_v.c_str(), filepath_p.c_str(), m_Device, m_EngineFactory);
        }

        std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t size)
        {
            return std::make_shared<VertexBuffer>(size, m_Device);
        }

        std::shared_ptr<VertexBuffer> CreateVertexBuffer(void* data, uint32_t size, uint32_t vertice_count)
        {
            return std::make_shared<VertexBuffer>(data, size, vertice_count, m_Device);
        }

        std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, int count)
        {
            return std::make_shared<IndexBuffer>(indices, count, m_Device);
        }

        std::shared_ptr<Texture> CreateTexture(const char* filepath, bool isRGB)
        {
            return std::make_shared<Texture>(filepath, isRGB, m_Device);
        }

        template <typename T>
        std::shared_ptr<ConstantBuffer<T>> CreateConstantBuffer(const std::string& name)
        {
            return std::make_shared<ConstantBuffer<T>>(name, m_Device);
        }

        

        void Present();

        void OnResize(int width, int height);

    private:
        RendererAPI m_Api = RendererAPI::None;
        std::string m_ApiName = "None";
        
        RefCntAutoPtr<IEngineFactory> m_EngineFactory;
        RefCntAutoPtr<IRenderDevice> m_Device;
        RefCntAutoPtr<IDeviceContext> m_DeviceContext;
        RefCntAutoPtr<ISwapChain> m_SwapChain;

        int m_DrawCall = 0;
    };
}