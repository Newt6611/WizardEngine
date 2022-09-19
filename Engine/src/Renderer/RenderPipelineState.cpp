#include "wzpch.h"
#include "RenderPipelineState.h"
#include "Core/Log.h"
#include "Renderer.h"

namespace Wizard {
    RenderPipelineState::RenderPipelineState(const std::string& name)
    {
        ISwapChain* swapchain = Renderer::Get()->GetSwapChain();
    
        createInfo.PSODesc.Name = std::string(name + " pso").c_str(); 
        createInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        createInfo.GraphicsPipeline.NumRenderTargets = 1;
        createInfo.GraphicsPipeline.RTVFormats[0] = swapchain->GetDesc().ColorBufferFormat;
        createInfo.GraphicsPipeline.DSVFormat = swapchain->GetDesc().DepthBufferFormat;
        createInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        createInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        createInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;

        createInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    }

    void RenderPipelineState::Generate()
    {
        if (m_Generated) {
            WZ_ENGINE_WARN("{} already been generated", createInfo.PSODesc.Name);
            return;
        }
        m_Generated = true;
        IRenderDevice* device = Renderer::Get()->GetDevice();
        device->CreateGraphicsPipelineState(createInfo, &m_PipelineState);
    }

    RenderPipelineState::~RenderPipelineState()
    {
        
    }
}