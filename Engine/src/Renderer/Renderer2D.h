#pragma once
#include "glm/glm.hpp"
#include "OrthographicCamera.h"

namespace Wizard {

    // ConstantBuffers
    //    VertexShader
    struct PerFrameConst
    {
        glm::mat4 View;
        glm::mat4 Projection;
    };

    struct PerObjConst
    {
        glm::mat4 World;
    };

    //    PixelShader
    struct FlatPSConst
    {
        glm::vec4 Color;
    };
    //

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

    private:
        static void InitFlatPipelineState();



    };
}