#include "wzpch.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Core/Log.h"
#include "Core/Window.h"

#include "Renderer2D.h"

#if WZ_APPLE
    extern void* GetNSWindowView(GLFWwindow* wnd);
#endif

namespace Wizard
{
    void Renderer::Init(RendererAPI api, Window* window)
    {
        m_Api = api;

#if PLATFORM_WIN32
        Win32NativeWindow Window{ glfwGetWin32Window(window->GetWindow()) };
#endif
#if PLATFORM_LINUX
        LinuxNativeWindow Window;
        Window.WindowId = glfwGetX11Window(window->GetWindow());
        Window.pDisplay = glfwGetX11Display();
#endif
#if PLATFORM_MACOS
        MacOSNativeWindow Window{GetNSWindowView(window->GetWindow())};
#endif

        SwapChainDesc SCDesc;
        switch (m_Api)
        {
#if WZ_D3D11
        case RendererAPI::D3D11:
        {
#   if ENGINE_DLL
            auto* GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
#   endif
            m_EngineFactory = GetEngineFactoryD3D11();
            IEngineFactoryD3D11* pFactoryD3D11 = dynamic_cast<IEngineFactoryD3D11*>(&(*m_EngineFactory));
            
            EngineD3D11CreateInfo EngineCI;
            EngineCI.Features.BindlessResources = DEVICE_FEATURE_STATE_OPTIONAL;
            pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryD3D11->CreateSwapChainD3D11(m_Device, m_DeviceContext, SCDesc, FullScreenModeDesc{}, Window, &m_SwapChain);

            m_ApiName = "D3D11";
        }
        break;
#endif

#if WZ_D3D12
        case RendererAPI::D3D12:
        {
#   if ENGINE_DLL

            auto* GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
#   endif
            m_EngineFactory = GetEngineFactoryD3D12();
            IEngineFactoryD3D12* pFactoryD3D12 = dynamic_cast<IEngineFactoryD3D12*>(&(*m_EngineFactory));

            EngineD3D12CreateInfo EngineCI;
            EngineCI.Features.BindlessResources = DEVICE_FEATURE_STATE_OPTIONAL;
            pFactoryD3D12->CreateDeviceAndContextsD3D12(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryD3D12->CreateSwapChainD3D12(m_Device, m_DeviceContext, SCDesc, FullScreenModeDesc{}, Window, &m_SwapChain);

            m_ApiName = "D3D12";
        }
        break;
#endif

#if WZ_GL
        case RendererAPI::OpenGL:
        {
#   if EXPLICITLY_LOAD_ENGINE_GL_DLL
            auto GetEngineFactoryOpenGL = LoadGraphicsEngineOpenGL();
#   endif
            m_EngineFactory = GetEngineFactoryOpenGL();
            IEngineFactoryOpenGL* pFactoryOpenGL = dynamic_cast<IEngineFactoryOpenGL*>(&(*m_EngineFactory));
            EngineGLCreateInfo EngineCI;
            EngineCI.Features.BindlessResources = DEVICE_FEATURE_STATE_OPTIONAL;
            EngineCI.Window = Window;
            pFactoryOpenGL->CreateDeviceAndSwapChainGL(EngineCI, &m_Device, &m_DeviceContext, SCDesc, &m_SwapChain);
        }
        break;
#endif // GL_SUPPORTED


#if WZ_VULKAN
        case RendererAPI::Vulkan:
        {
#   if EXPLICITLY_LOAD_ENGINE_VK_DLL
            auto* GetEngineFactoryVk = LoadGraphicsEngineVk();
#   endif
            m_EngineFactory = GetEngineFactoryVk();
            IEngineFactoryVk* pFactoryVk = dynamic_cast<IEngineFactoryVk*>(&(*m_EngineFactory));

            EngineVkCreateInfo EngineCI;
            EngineCI.Features.BindlessResources = DEVICE_FEATURE_STATE_OPTIONAL;
            pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryVk->CreateSwapChainVk(m_Device, m_DeviceContext, SCDesc, Window, &m_SwapChain);

            m_ApiName = "Vulkan";
        }
        break;
#endif

#if WZ_METAL
        case RendererAPI::Metal:
        {
            m_EngineFactory = GetEngineFactoryMtl();
            IEngineFactoryMtl* pFactoryMtl = GetEngineFactoryMtl();

            EngineMtlCreateInfo EngineCI;
            EngineCI.Features.BindlessResources = DEVICE_FEATURE_STATE_OPTIONAL;
            pFactoryMtl->CreateDeviceAndContextsMtl(EngineCI, &m_pDevice, &m_pImmediateContext);
            pFactoryMtl->CreateSwapChainMtl(m_pDevice, m_pImmediateContext, SCDesc, Window, &m_pSwapChain);
        }
        break;
#endif

        default:
            WZ_ENGINE_ERROR("Unknown/unsupported device type");
            break;
        }

        if (m_Device == nullptr || m_DeviceContext == nullptr || m_SwapChain == nullptr)
            WZ_ENGINE_ERROR("Device or deviceContext or swapchain null");


        // Init Renderer2D
        Renderer2D::Init();

    }

    void Renderer::SetClearColor(float color[4])
    {
        m_ClearColor[0] = color[0]; 
        m_ClearColor[1] = color[1]; 
        m_ClearColor[2] = color[2]; 
        m_ClearColor[3] = color[3]; 
    }

    void Renderer::ClearColor(ITextureView* view)
    {
        m_DeviceContext->ClearRenderTarget(view, m_ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
    }

    void Renderer::Present()
    {
        m_SwapChain->Present();
    }

    void Renderer::OnResize(int width, int height)
    {
        WZ_ENGINE_TRACE("Resize {} {}", width, height);
        m_SwapChain->Resize((Uint32)width, (Uint32)height);
    }
}
