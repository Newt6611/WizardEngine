#include "Renderer.h"
#include "Core/Log.h"
#include "VulkanRenderer.h"


namespace Wizard
{
    std::unique_ptr<Renderer> Renderer::Create(RendererAPI api)
    {
        switch (api)
        {
        case RendererAPI::None:
            WZ_ENGINE_ERROR("Init Render API Can't Be None");
            return nullptr;
            break;
        case RendererAPI::Vulkan:
            return std::make_unique<VulkanRenderer>();
            break;
        case RendererAPI::D3D12:
            WZ_ENGINE_ERROR("Not Support D3D12 Yet");
            return nullptr;
            break;
        }
    }
}