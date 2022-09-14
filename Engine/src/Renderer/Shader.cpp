#include "Shader.h"
#include "Renderer.h"
#include "Log.h"
#include "VulkanShader.h"

namespace Wizard {
    std::unique_ptr<Shader> Shader::Create()
    {
        /*switch (Renderer::Get()->GetAPI())
        {
        case RendererAPI::None:
            WZ_ENGINE_ERROR("Create Sgader RenderAPI Can't Be None");
            return nullptr;
            break;
        case RendererAPI::Vulkan:
            return std::make_unique<VulkanShader>();
            break;
        case RendererAPI::D3D12:
            WZ_ENGINE_ERROR("Not Support D3D12(Shader) Yet");
            return nullptr;
            break;
        }*/

        WZ_ENGINE_ERROR("Return Null Renderer");
        return nullptr;
    }
     
    std::vector<char> Shader::ReadByteCode(const std::string& fileName)
    {
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            WZ_ENGINE_ERROR("Failed To Read File {}", fileName);
            return std::vector<char>();
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }
}