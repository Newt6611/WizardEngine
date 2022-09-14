#pragma once
#include "wzpch.h"
#include "Core.h"

#define PLATFORM_WIN32 1

#ifdef WZ_WINDOWS
    #ifdef WZ_D3D11
        #include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
    #endif
    #ifdef WZ_D3D12
        #include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
    #endif
    #ifdef WZ_VULKAN
        #include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
    #endif
#endif

#ifdef WZ_APPLE
    #ifdef WZ_METAL
        #include "Graphics/GraphicsEngineMetal/interface/EngineFactoryMtl.h"
    #endif
    #ifdef WZ_VULKAN
        #include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
    #endif
#endif

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include "Common/interface/RefCntAutoPtr.hpp"

using namespace Diligent;

namespace Wizard {
    class Window;

    enum class RendererAPI {
        None = (1 << 0),
        Vulkan = (1 << 1),
        D3D11 = (1 << 2),
        D3D12 = (1 << 3),
        OpenGL = (1 << 4)
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

        inline const RendererAPI GetAPI() {
            return m_Api;
        }

        inline const std::string& GetAPIName() {
            return m_ApiName;
        }

        void InitTest();
        void Render();

        void Present();

    private:
        RendererAPI m_Api = RendererAPI::None;
        std::string m_ApiName = "None";
        
        IRenderDevice* m_Device = nullptr;
        IDeviceContext* m_DeviceContext = nullptr;
        ISwapChain* m_SwapChain = nullptr;

        IPipelineState* m_PipelineState;
    };
}