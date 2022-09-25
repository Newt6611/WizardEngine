#include "wzpch.h"
#include "Shader.h"

namespace Wizard {
    Shader::Shader(const std::string& name, const char* file_path_v, const char* file_path_p, IRenderDevice* device, IEngineFactory* engineFactory)
    {
        ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> shaderSourceFactory;
        engineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &shaderSourceFactory);
        ShaderCI.pShaderSourceStreamFactory = shaderSourceFactory;

        // Vertex Shader
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = std::string(name + " vs").c_str();
        ShaderCI.FilePath = file_path_v;
        device->CreateShader(ShaderCI, &m_VertexShader);

        // Pixel Shader
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = std::string(name + " ps").c_str();
        ShaderCI.FilePath = file_path_p;
        device->CreateShader(ShaderCI, &m_PixelShader);
    }

    Shader::~Shader()
    {
    }
}