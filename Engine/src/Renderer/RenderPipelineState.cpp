#include "wzpch.h"
#include "RenderPipelineState.h"

namespace Wizard {
    RenderPipelineState::RenderPipelineState()
    {
        swapchain = Renderer::Get()->GetSwapChain();
    }

    RenderPipelineState::~RenderPipelineState()
    {
        
    }

    void RenderPipelineState::Init(const char* state_name)
    {
        ISwapChain* swapchain = Renderer::Get()->GetSwapChain();

        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = state_name;
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = swapchain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat = swapchain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
    }
}