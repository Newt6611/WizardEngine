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

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        static void StartBatch();
        static void NextBatch();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotate);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotate);
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

        // Texture
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture> texture, const glm::vec4& color = {1, 1, 1, 1});
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture> texture, const glm::vec4& color = {1, 1, 1, 1});

    private:
        static void FlushQuad();

        static void LoadTextureArray(std::shared_ptr<Texture> texture);

        static void InitQuadPipelineState();
        static void InitTexturePipelineState();


    };
}