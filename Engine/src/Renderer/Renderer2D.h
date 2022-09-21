#pragma once
#include "glm/glm.hpp"
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Wizard {

    // ConstantBuffers
    //    VertexShader
    struct PerFrameConst
    {
        glm::mat4 ViewProjection;
    };

    // struct PerObjConst
    // {
    //     glm::mat4 World;
    // };

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
        static void EndScene();
        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotate = 0);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotate = 0);


        // Texture
        // static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& color = {1, 1, 1, 1}, float rotate = 0);
        // static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Texture* texture, const glm::vec4& color = {1, 1, 1, 1}, float rotate = 0);

    private:
        static void InitFlatPipelineState();
        static void InitTexturePipelineState();


    };
}