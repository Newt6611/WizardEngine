#pragma once
#include "wzpch.h"

namespace Wizard {
    enum class RendererAPI 
    {
        None = 0,
        Vulkan,
        D3D12
    };
    
    class Renderer
    {
    public:
        static std::unique_ptr<Renderer> Create(RendererAPI api);
        Renderer() { }
        virtual ~Renderer() { }

        virtual RendererAPI GetAPI() {
            return RendererAPI::None;
        };

        virtual std::string GetAPIName() {
            return "None";
        };
    };
}