#pragma once
#include "Graphics/GraphicsEngine/interface/EngineFactory.h"
#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/Shader.h"
#include "Common/interface/RefCntAutoPtr.hpp"


namespace Wizard {
    using namespace Diligent;    

    class Shader
    {
    public:
        Shader(const std::string& name, const char* file_path_v, const char* file_path_p, 
            IRenderDevice* device, IEngineFactory* engineFactory);
        virtual ~Shader();

        inline IShader* GetVertexShader() { return m_VertexShader; }
        inline IShader* GetPixelShader() { return m_PixelShader; }

    private:
        RefCntAutoPtr<IShader> m_VertexShader;
        RefCntAutoPtr<IShader> m_PixelShader;
    };
}