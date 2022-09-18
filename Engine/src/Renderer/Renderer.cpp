#include "wzpch.h"
#include "Renderer.h"
#include "Core/Log.h"
#include "Core/Window.h"
#include "GLFW/glfw3native.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"


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
        if (m_Api == RendererAPI::OpenGL)
            glfwMakeContextCurrent(window->GetWindow());
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
            auto* GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
#    endif
            m_EngineFactory = GetEngineFactoryD3D11();
            IEngineFactoryD3D11* pFactoryD3D11 = dynamic_cast<IEngineFactoryD3D11*>(&(*m_EngineFactory));
            
            EngineD3D11CreateInfo EngineCI;
            pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryD3D11->CreateSwapChainD3D11(m_Device, m_DeviceContext, SCDesc, FullScreenModeDesc{}, Window, &m_SwapChain);

            m_ApiName = "D3D11";
        }
        break;
#endif

#if WZ_D3D12
        case RendererAPI::D3D12:
        {
#    if ENGINE_DLL

            auto* GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
#    endif
            m_EngineFactory = GetEngineFactoryD3D12();
            IEngineFactoryD3D12* pFactoryD3D12 = dynamic_cast<IEngineFactoryD3D12*>(&(*m_EngineFactory));

            EngineD3D12CreateInfo EngineCI;
            pFactoryD3D12->CreateDeviceAndContextsD3D12(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryD3D12->CreateSwapChainD3D12(m_Device, m_DeviceContext, SCDesc, FullScreenModeDesc{}, Window, &m_SwapChain);

            m_ApiName = "D3D12";
        }
        break;
#endif


#if WZ_VULKAN
        case RendererAPI::Vulkan:
        {
#    if EXPLICITLY_LOAD_ENGINE_VK_DLL
            auto* GetEngineFactoryVk = LoadGraphicsEngineVk();
#    endif
            m_EngineFactory = GetEngineFactoryVk();
            IEngineFactoryVk* pFactoryVk = dynamic_cast<IEngineFactoryVk*>(&(*m_EngineFactory));

            EngineVkCreateInfo EngineCI;
            pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_Device, &m_DeviceContext);
            pFactoryVk->CreateSwapChainVk(m_Device, m_DeviceContext, SCDesc, Window, &m_SwapChain);

            m_ApiName = "Vulkan";
        }
        break;
#endif

#if METAL_SUPPORTED
        case RendererAPI::Metal:
        {
            m_EngineFactory = GetEngineFactoryMtl();
            IEngineFactoryMtl* pFactoryMtl = GetEngineFactoryMtl();

            EngineMtlCreateInfo EngineCI;
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

    }

    void Renderer::InitTest()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "CUBE PSO";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_SwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat = m_SwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
        
        std::shared_ptr<Shader> cubeShader = CreateShader("cube", "C:/Dev/Wizard/Engine/assets/shaders/cube.vs", "C:/Dev/Wizard/Engine/assets/shaders/cube.ps");

        PSOCreateInfo.pVS = cubeShader->GetVertexShader();
        PSOCreateInfo.pPS = cubeShader->GetPixelShader();

        m_ConstantBuffer = CreateConstantBuffer<glm::mat4>("cube");
        
        LayoutElement layoutElement[] = {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}
        };

        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
        
        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        ShaderResourceVariableDesc vars[] = {
            { SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE }
        };

        PSOCreateInfo.PSODesc.ResourceLayout.Variables = vars;
        PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(vars);

        SamplerDesc sameLinearClampDesc {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
        };

        ImmutableSamplerDesc imtbSamplers[] = {
            { SHADER_TYPE_PIXEL, "g_Texture", sameLinearClampDesc }
        };

        PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = imtbSamplers;
        PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(imtbSamplers); 

        m_Device->CreateGraphicsPipelineState(PSOCreateInfo, &m_PipelineState);

        m_PipelineState->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_ConstantBuffer->GetConstantBuffer());
        m_PipelineState->CreateShaderResourceBinding(&m_SRB, true);

        CreateVertexBuffer();
        CreateIndexBuffer();
        LoadTexture();
    }

    void Renderer::CreateVertexBuffer()
    {
        struct Vertex
        {
            glm::vec3 pos;
            glm::vec2 uv;
        };
        
        Vertex CubeVerts[] =
        {
            {glm::vec3(-1,-1,-1), glm::vec2(0,1)},
            {glm::vec3(-1,+1,-1), glm::vec2(0,0)},
            {glm::vec3(+1,+1,-1), glm::vec2(1,0)},
            {glm::vec3(+1,-1,-1), glm::vec2(1,1)},

            {glm::vec3(-1,-1,-1), glm::vec2(0,1)},
            {glm::vec3(-1,-1,+1), glm::vec2(0,0)},
            {glm::vec3(+1,-1,+1), glm::vec2(1,0)},
            {glm::vec3(+1,-1,-1), glm::vec2(1,1)},

            {glm::vec3(+1,-1,-1), glm::vec2(0,1)},
            {glm::vec3(+1,-1,+1), glm::vec2(1,1)},
            {glm::vec3(+1,+1,+1), glm::vec2(1,0)},
            {glm::vec3(+1,+1,-1), glm::vec2(0,0)},

            {glm::vec3(+1,+1,-1), glm::vec2(0,1)},
            {glm::vec3(+1,+1,+1), glm::vec2(0,0)},
            {glm::vec3(-1,+1,+1), glm::vec2(1,0)},
            {glm::vec3(-1,+1,-1), glm::vec2(1,1)},

            {glm::vec3(-1,+1,-1), glm::vec2(1,0)},
            {glm::vec3(-1,+1,+1), glm::vec2(0,0)},
            {glm::vec3(-1,-1,+1), glm::vec2(0,1)},
            {glm::vec3(-1,-1,-1), glm::vec2(1,1)},

            {glm::vec3(-1,-1,+1), glm::vec2(1,1)},
            {glm::vec3(+1,-1,+1), glm::vec2(0,1)},
            {glm::vec3(+1,+1,+1), glm::vec2(0,0)},
            {glm::vec3(-1,+1,+1), glm::vec2(1,0)}
        };
        m_VertexBuffer = CreateVertexBuffer(CubeVerts, sizeof(CubeVerts), sizeof(CubeVerts) / sizeof(Vertex));
    }

    void Renderer::CreateIndexBuffer()
    {
        uint32_t Indices[] =
        {
            2,0,1,    2,3,0,
            4,6,5,    4,7,6,
            8,10,9,   8,11,10,
            12,14,13, 12,15,14,
            16,18,17, 16,19,18,
            20,21,22, 20,22,23
        };
        // clang-format on

        m_IndexBuffer = CreateIndexBuffer(Indices, 36);
    }

    void Renderer::LoadTexture()
    {
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB = true;
        RefCntAutoPtr<ITexture> Tex;
        CreateTextureFromFile("C:/Dev/Wizard/Engine/assets/textures/DGLogo.png", loadInfo, m_Device, &Tex);
        // Get shader resource view from the texture
        m_TextureSRV = Tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

        // Set texture SRV in the SRB
        m_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(m_TextureSRV);
    }

    void Renderer::Render()
    {
        // Clear the back buffer
        const float ClearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        // Let the engine perform required state transitions
        ITextureView* pRTV = m_SwapChain->GetCurrentBackBufferRTV();
        ITextureView* pDSV = m_SwapChain->GetDepthBufferDSV();
        m_DeviceContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_DeviceContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_DeviceContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        MapHelper<glm::mat4> CBConsant(m_DeviceContext, m_ConstantBuffer->GetConstantBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConsant = glm::transpose(m_WVP);
        
        const uint64_t offset = 0;
        IBuffer* pBuffers[] = {m_VertexBuffer->GetBuffer()};
        m_DeviceContext->SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_DeviceContext->SetIndexBuffer(m_IndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        // Set the pipeline state in the immediate context
        m_DeviceContext->SetPipelineState(m_PipelineState);
        m_DeviceContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs  drawAttrs;
        drawAttrs.IndexType = VT_UINT32;
        drawAttrs.NumIndices = 36;
        drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_DeviceContext->DrawIndexed(drawAttrs);
    }

    void Renderer::Update()
    {
        glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0f, 5.0f));
        //auto View = glm::lookAtLH(glm::vec3{0.f,0.f, -5.f}, glm::vec3{0.f,0.f,1.f}, glm::vec3{0.f, 1.f, 0.f});
        
        auto Proj = glm::perspectiveFovLH(glm::radians(45.0f), 800.f, 600.f, 0.1f, 100.f);
        
        m_WVP = Proj * View;
    }

    void Renderer::Present()
    {
        m_SwapChain->Present();
    }
}