#include "Renderer.h"
#include "Core/Log.h"
#include "Core/Window.h"
#include "GLFW/glfw3native.h"


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
        Window.WindowId = glfwGetX11Window(m_Window);
        Window.pDisplay = glfwGetX11Display();
        if (DevType == RENDER_DEVICE_TYPE_GL)
            glfwMakeContextCurrent(m_Window);
#endif
#if PLATFORM_MACOS
        MacOSNativeWindow Window;
        if (DevType == RENDER_DEVICE_TYPE_GL)
            glfwMakeContextCurrent(m_Window);
        else
            Window.pNSView = GetNSWindowView(m_Window);
#endif

        SwapChainDesc SCDesc;
        switch (m_Api)
        {
#if WZ_D3D11
        case RendererAPI::D3D11:
        {
#    if ENGINE_DLL
            // Load the dll and import GetEngineFactoryD3D11() function
            auto* GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
#    endif
            auto* pFactoryD3D11 = GetEngineFactoryD3D11();

            EngineD3D11CreateInfo EngineCI;
            pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryD3D11->CreateSwapChainD3D11(m_Device, m_DeviceContext, SCDesc, FullScreenModeDesc{}, Window, &m_SwapChain);

            m_ApiName = "D3D11";
        }
        break;
#endif // D3D11_SUPPORTED


#if WZ_D3D12
        case RendererAPI::D3D12:
        {
#    if ENGINE_DLL
            // Load the dll and import GetEngineFactoryD3D12() function
            auto* GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
#    endif
            auto* pFactoryD3D12 = GetEngineFactoryD3D12();

            EngineD3D12CreateInfo EngineCI;
            pFactoryD3D12->CreateDeviceAndContextsD3D12(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryD3D12->CreateSwapChainD3D12(m_Device, m_DeviceContext, SCDesc, FullScreenModeDesc{}, Window, &m_SwapChain);

            m_ApiName = "D3D12";
        }
        break;
#endif // D3D12_SUPPORTED


//#if WZ_GL
//        case RendererAPI::OpenGL:
//        {
//#    if EXPLICITLY_LOAD_ENGINE_GL_DLL
//            // Load the dll and import GetEngineFactoryOpenGL() function
//            auto GetEngineFactoryOpenGL = LoadGraphicsEngineOpenGL();
//#    endif
//            auto* pFactoryOpenGL = GetEngineFactoryOpenGL();
//
//            EngineGLCreateInfo EngineCI;
//            EngineCI.Window = Window;
//            pFactoryOpenGL->CreateDeviceAndSwapChainGL(EngineCI, &m_pDevice, &m_pImmediateContext, SCDesc, &m_pSwapChain);
//        }
//        break;
//#endif // GL_SUPPORTED


#if WZ_VULKAN
        case RendererAPI::Vulkan:
        {
#    if EXPLICITLY_LOAD_ENGINE_VK_DLL
            // Load the dll and import GetEngineFactoryVk() function
            auto* GetEngineFactoryVk = LoadGraphicsEngineVk();
#    endif
            auto* pFactoryVk = GetEngineFactoryVk();

            EngineVkCreateInfo EngineCI;
            pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryVk->CreateSwapChainVk(m_Device, m_DeviceContext, SCDesc, Window, &m_SwapChain);

            m_ApiName = "Vulkan";
        }
        break;
#endif // VULKAN_SUPPORTED

#if METAL_SUPPORTED
        case RENDER_DEVICE_TYPE_METAL:
        {
            auto* pFactoryMtl = GetEngineFactoryMtl();

            EngineMtlCreateInfo EngineCI;
            pFactoryMtl->CreateDeviceAndContextsMtl(EngineCI, &m_pDevice, &m_pImmediateContext);
            pFactoryMtl->CreateSwapChainMtl(m_pDevice, m_pImmediateContext, SCDesc, Window, &m_pSwapChain);
        }
        break;
#endif // METAL_SUPPORTED

        default:
            WZ_ENGINE_ERROR("Unknown/unsupported device type");
            break;
        }

        if (m_Device == nullptr || m_DeviceContext == nullptr || m_SwapChain == nullptr)
            WZ_ENGINE_ERROR("Device or deviceContext or swapchain null");

    }


    static const char* VSSource = R"(
    struct PSInput 
    { 
        float4 Pos   : SV_POSITION; 
        float3 Color : COLOR; 
    };
    void main(in  uint    VertId : SV_VertexID,
              out PSInput PSIn) 
    {
        float4 Pos[3];
        Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
        Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
        Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);
        float3 Col[3];
        Col[0] = float3(1.0, 0.0, 0.0); // red
        Col[1] = float3(0.0, 1.0, 0.0); // green
        Col[2] = float3(0.0, 0.0, 1.0); // blue
        PSIn.Pos   = Pos[VertId];
        PSIn.Color = Col[VertId];
    }
    )";

        // Pixel shader simply outputs interpolated vertex color
        static const char* PSSource = R"(
    struct PSInput 
    { 
        float4 Pos   : SV_POSITION; 
        float3 Color : COLOR; 
    };
    struct PSOutput
    { 
        float4 Color : SV_TARGET; 
    };
    void main(in  PSInput  PSIn,
              out PSOutput PSOut)
    {
        PSOut.Color = float4(PSIn.Color.rgb, 1.0);
    }
    )";

    void Renderer::InitTest()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "Simple Triangle";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_SwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat = m_SwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

        ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.UseCombinedTextureSamplers = true;
        RefCntAutoPtr<IShader> pVS;
        {
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint = "main";
            ShaderCI.Desc.Name = "Triangle vertex shader";
            ShaderCI.Source = VSSource;
            m_Device->CreateShader(ShaderCI, &pVS);
        }

        RefCntAutoPtr<IShader> pPS;
        {
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint = "main";
            ShaderCI.Desc.Name = "Triangle pixel shader";
            ShaderCI.Source = PSSource;
            m_Device->CreateShader(ShaderCI, &pPS);
        }

        PSOCreateInfo.pVS = pVS;
        PSOCreateInfo.pPS = pPS;
        m_Device->CreateGraphicsPipelineState(PSOCreateInfo, &m_PipelineState);
    }

    void Renderer::Render()
    {
        // Clear the back buffer
        const float ClearColor[] = { 0.350f, 0.350f, 0.350f, 1.0f };
        // Let the engine perform required state transitions
        ITextureView* pRTV = m_SwapChain->GetCurrentBackBufferRTV();
        ITextureView* pDSV = m_SwapChain->GetDepthBufferDSV();
        m_DeviceContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_DeviceContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_DeviceContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state in the immediate context
        m_DeviceContext->SetPipelineState(m_PipelineState);

        // Typically we should now call CommitShaderResources(), however shaders in this example don't
        // use any resources.

        DrawAttribs drawAttrs;
        drawAttrs.NumVertices = 3; // We will render 3 vertices
        m_DeviceContext->Draw(drawAttrs);
    }

    void Renderer::Present()
    {
        m_SwapChain->Present();
    }
}