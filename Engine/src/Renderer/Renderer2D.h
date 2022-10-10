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

        static int GetQuads();
        static int GetVertices();
        static int GetIndices();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        static void StartBatch();
        static void NextBatch();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityID = 0);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = 0);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotate, int entityID = 0);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotate, int entityID = 0);

        // Texture
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, const glm::vec4& color = {1, 1, 1, 1}, int entityID = 0);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, const glm::vec4& color = {1, 1, 1, 1}, int entityID = 0);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotate, const glm::vec4& color = {1, 1, 1, 1}, int entityID = 0);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotate, const glm::vec4& color = {1, 1, 1, 1}, int entityID = 0);
        
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, std::shared_ptr<Texture2D> texture, int entityID);

    private:
        
        static void FlushQuad();

        static int GetTexIndex(std::shared_ptr<Texture2D> texture);
        static void InitQuadPipelineState();
    };
}