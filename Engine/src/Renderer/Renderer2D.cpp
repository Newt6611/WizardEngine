#include "wzpch.h"
#include "Renderer2D.h"
#include "RenderTool.h"
#include "RenderCommand.h"
#include "RenderPipelineState.h"
#include "Graphics/GraphicsTools/interface/MapHelper.hpp"
#include "Core/Log.h"

namespace Wizard {
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 UV;
        uint32_t TexIndex;
    };

    struct Render2DData
    {
        std::shared_ptr<ConstantBuffer<PerFrameConst>> PerFrameData;
        
        // Batch Rendering
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlot = 32;

        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<IndexBuffer> QuadIndexBuffer;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase;
        QuadVertex* QuadVertexBufferPtr;

        glm::vec4 QuadVertexPosition[4];

        std::shared_ptr<Texture2D> WhiteTexture;
        // For send to pixel shader
        ITextureView* TextureViewArray[MaxTextureSlot]; 
        // Holding texture Data to comapre textureID is in TextureViewArray or not <TextureID, Slot>
        std::array<int, MaxTextureSlot> TextureDatas; 
        uint32_t CurrentTextureSlot;
        //
        
        // Quad Render Pipeline //
        std::shared_ptr<RenderPipelineState> QuadPipelineState;
        std::shared_ptr<Shader> QuadShader;
        RefCntAutoPtr<IShaderResourceBinding> QuadSRB;
        //////////////////////////
    };

    static Render2DData s_RenderData;

    void Renderer2D::Init()
    {
        s_RenderData.QuadVertexBuffer = RenderTool::CreateVertexBuffer(sizeof(QuadVertex) * s_RenderData.MaxVertices);
        s_RenderData.QuadVertexBufferBase = new QuadVertex[s_RenderData.MaxVertices];

        uint32_t quadIndices[s_RenderData.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_RenderData.MaxIndices; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        s_RenderData.QuadIndexBuffer = RenderTool::CreateIndexBuffer(quadIndices, s_RenderData.MaxIndices);

        s_RenderData.QuadVertexPosition[0] = glm::vec4 { -0.5f, -0.5f, 0.0f, 1.0f };
        s_RenderData.QuadVertexPosition[1] = glm::vec4 { -0.5f,  0.5f, 0.0f, 1.0f };
        s_RenderData.QuadVertexPosition[2] = glm::vec4 {  0.5f,  0.5f, 0.0f, 1.0f };
        s_RenderData.QuadVertexPosition[3] = glm::vec4 {  0.5f, -0.5f, 0.0f, 1.0f };

        s_RenderData.CurrentTextureSlot = 0;
        s_RenderData.WhiteTexture = RenderTool::CreateTexture(1, 1, (const void*)0xffffffff);
        
        for (int i = 0; i < s_RenderData.MaxTextureSlot; ++i) {
            s_RenderData.TextureViewArray[i] = s_RenderData.WhiteTexture->GetTextureView();        
        }
        s_RenderData.TextureDatas[0] = s_RenderData.WhiteTexture->GetTextureID();

        s_RenderData.PerFrameData = RenderTool::CreateConstantBuffer<PerFrameConst>("PerFrameConstant");

        InitQuadPipelineState();
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_RenderData.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        StartBatch();

        MapHelper<PerFrameConst> perFrameConst(RenderTool::GetDeviceContext(), s_RenderData.PerFrameData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);

        perFrameConst->ViewProjection = glm::transpose(camera.GetProjectionMatrix() * camera.GetViewMatrix());
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::Flush()
    {
        if (s_RenderData.QuadIndexCount)
            FlushQuad();
    }

    void Renderer2D::StartBatch()
    {
        s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
        s_RenderData.QuadIndexCount = 0;
        s_RenderData.CurrentTextureSlot = 0;
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer2D::FlushQuad()
    {
        MapHelper<QuadVertex> Vertices(Renderer::Get()->GetDeviceContext(), s_RenderData.QuadVertexBuffer->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
        for (int i = 0; i < s_RenderData.QuadVertexBufferPtr - s_RenderData.QuadVertexBufferBase; ++i) {
            Vertices[i].Position = s_RenderData.QuadVertexBufferBase[i].Position;
            Vertices[i].Color = s_RenderData.QuadVertexBufferBase[i].Color;
            Vertices[i].UV = s_RenderData.QuadVertexBufferBase[i].UV;
            Vertices[i].TexIndex = s_RenderData.QuadVertexBufferBase[i].TexIndex;
        }

        const uint64_t offset = 0;
        IBuffer* pBuffers[] = {s_RenderData.QuadVertexBuffer->GetBuffer()};
        RenderCommand::SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        RenderCommand::SetIndexBuffer(s_RenderData.QuadIndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        s_RenderData.QuadSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Textures")->SetArray((IDeviceObject**)s_RenderData.TextureViewArray, 0, s_RenderData.MaxTextureSlot);

        RenderCommand::SetPipelineState(s_RenderData.QuadPipelineState->GetPipelineState());
        
        RenderCommand::CommitShaderResources(s_RenderData.QuadSRB);

        DrawIndexedAttribs drawAttrs;
        drawAttrs.IndexType = VT_UINT32;
        drawAttrs.NumIndices = s_RenderData.QuadIndexCount;
        drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        RenderCommand::DrawIndexed(drawAttrs);
    }

    // Color Quad // 
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color, nullptr);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, float rotate)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, color, rotate);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotate)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.f, 0.f, 1.f)) *
			    glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color, nullptr);
    }


    // Texture Quad //
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, texture, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color, texture);
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotate, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, texture, rotate, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotate, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.f, 0.f, 1.f)) *
			    glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color, texture);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, std::shared_ptr<Texture2D> texture)
    {
        int texIndex = GetTexIndex(texture);
        uint32_t vertexCount = 4;
        glm::vec2 texcoord[] = { { 0.f, 1.f }, { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f } };

        if (s_RenderData.QuadIndexCount >= s_RenderData.MaxIndices)
            NextBatch();

        for (int i = 0; i < vertexCount; ++i) {
            s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPosition[i];
            s_RenderData.QuadVertexBufferPtr->UV = texcoord[i];
            s_RenderData.QuadVertexBufferPtr->Color = color;
            s_RenderData.QuadVertexBufferPtr->TexIndex = texIndex;
            s_RenderData.QuadVertexBufferPtr++;
        }

        s_RenderData.QuadIndexCount += 6;
    }
    //

    void Renderer2D::InitQuadPipelineState()
    {
        s_RenderData.QuadPipelineState = std::make_shared<RenderPipelineState>("Flat Pipeline State");
        s_RenderData.QuadShader = RenderTool::CreateShader("flat color", "/shaders/quad_vs.hlsl","/shaders/quad_ps.hlsl");

        s_RenderData.QuadPipelineState->createInfo.pVS = s_RenderData.QuadShader->GetVertexShader();
        s_RenderData.QuadPipelineState->createInfo.pPS = s_RenderData.QuadShader->GetPixelShader();
        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        LayoutElement layoutElement[] = {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 4, VT_FLOAT32, False},
            LayoutElement{2, 0, 2, VT_FLOAT32, False},
            LayoutElement{3, 0, 1, VT_UINT32, False}
        };

        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
        
        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = true;
        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].SrcBlend = BLEND_FACTOR_SRC_ALPHA;
        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].DestBlend = BLEND_FACTOR_INV_SRC_ALPHA;

        // Texture
        ShaderResourceVariableDesc Vars[] = {
            // SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC means you can dynamic set SRB's data in one frame
            { SHADER_TYPE_PIXEL, "g_Textures", SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC }
        };
        s_RenderData.QuadPipelineState->createInfo.PSODesc.ResourceLayout.Variables = Vars;
        s_RenderData.QuadPipelineState->createInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

        // Sampler
        SamplerDesc SamLinearClampDesc {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, 
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
        };

        ImmutableSamplerDesc ImtblSamplers[] = {
            {SHADER_TYPE_PIXEL, "g_Textures", SamLinearClampDesc}
        };

        s_RenderData.QuadPipelineState->createInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
        s_RenderData.QuadPipelineState->createInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

        s_RenderData.QuadPipelineState->Generate();
        
        s_RenderData.QuadPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerFrame")->Set(s_RenderData.PerFrameData->GetBuffer());

        s_RenderData.QuadPipelineState->GetPipelineState()->CreateShaderResourceBinding(&s_RenderData.QuadSRB, true);
    }

    int Renderer2D::GetTexIndex(std::shared_ptr<Texture2D> texture)
    {
        if (!texture) return 0;
        int texIndex = 0;
        for (int i = 1; i < s_RenderData.CurrentTextureSlot + 1; ++i) {
            // if TextureDatas has texture, then we get index for TextureViewArray
            if (s_RenderData.TextureDatas[i] == texture->GetTextureID()) {
                texIndex = i;
                break;
            }
        }

        if (texIndex == 0) {
            if (s_RenderData.CurrentTextureSlot >= s_RenderData.MaxTextureSlot - 1)
                NextBatch();
            texIndex = ++s_RenderData.CurrentTextureSlot;
            s_RenderData.TextureViewArray[texIndex] = texture->GetTextureView();
            s_RenderData.TextureDatas[texIndex] = texture->GetTextureID();
        }

        return texIndex;
    }
}