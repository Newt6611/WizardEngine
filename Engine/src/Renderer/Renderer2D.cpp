#include "Renderer2D.h"
#include "RenderTool.h"
#include "RenderCommand.h"
#include "RenderPipelineState.h"
#include "Graphics/GraphicsTools/interface/MapHelper.hpp"
#include "Core/Log.h"

namespace Wizard {
    struct Q
    {
        glm::vec3 Position;
    };
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 UV;
    };

    struct TextureVertex
    {
        glm::vec3 Position;
        glm::vec2 UV;
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

        RefCntAutoPtr<ITexture> TextureArray;
        uint32_t CurrentTextureSlot = 0;
        //

        // Quad Color Render Pipeline //
        std::shared_ptr<RenderPipelineState> QuadPipelineState;
        std::shared_ptr<Shader> QuadShader;
        RefCntAutoPtr<IShaderResourceBinding> QuadSRB;
        ////////////////////////////////

        // Texture Render Pipeline //
        std::shared_ptr<VertexBuffer> TextureVertexBuffer;
        std::shared_ptr<RenderPipelineState> TexturePipelineState;
        std::shared_ptr<Shader> TextureShader;
        RefCntAutoPtr<IShaderResourceBinding> TextureSRB;
        /////////////////////////////
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


        s_RenderData.PerFrameData = RenderTool::CreateConstantBuffer<PerFrameConst>("PerFrameConstant");
        //s_RenderData.PerObjData = RenderTool::CreateConstantBuffer<PerObjConst>("PerObjConstant");

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
        }

        const uint64_t offset = 0;
        IBuffer* pBuffers[] = {s_RenderData.QuadVertexBuffer->GetBuffer()};
        RenderCommand::SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        RenderCommand::SetIndexBuffer(s_RenderData.QuadIndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        RenderCommand::SetPipelineState(s_RenderData.QuadPipelineState->GetPipelineState());
        RenderCommand::CommitShaderResources(s_RenderData.QuadSRB);

        DrawIndexedAttribs drawAttrs;
        drawAttrs.IndexType = VT_UINT32;
        drawAttrs.NumIndices = s_RenderData.QuadIndexCount;
        drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        RenderCommand::DrawIndexed(drawAttrs);
    }

    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color);
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
        DrawQuad(transform, color);
    }

    void Renderer2D:: DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture> texture, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, texture, color);
    }
    
    void Renderer2D:: DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture> texture, const glm::vec4& color)
    {

        //s_RenderData.TextureSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(texture->GetTextureView());
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        uint32_t vertexCount = 4;
        glm::vec2 texcoord[] = { { 0.f, 1.f }, { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f } };

        if (s_RenderData.QuadIndexCount >= s_RenderData.MaxIndices)
            NextBatch();

        for (int i = 0; i < vertexCount; ++i) {
            s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPosition[i];
            s_RenderData.QuadVertexBufferPtr->UV = texcoord[i];
            s_RenderData.QuadVertexBufferPtr->Color = color;
            s_RenderData.QuadVertexBufferPtr++;
        }

        s_RenderData.QuadIndexCount += 6;
    }

    void Renderer2D::LoadTextureArray(std::shared_ptr<Texture> texture)
    {
        // Create Texture Array
        if (!s_RenderData.TextureArray) {
            TextureDesc TexArrDesc      = texture->GetTextureDesc();
            TexArrDesc.ArraySize = s_RenderData.MaxTextureSlot;
            TexArrDesc.Type      = RESOURCE_DIM_TEX_2D_ARRAY;
            TexArrDesc.Usage     = USAGE_DEFAULT;
            TexArrDesc.BindFlags = BIND_SHADER_RESOURCE;
            Renderer::Get()->GetDevice()->CreateTexture(TexArrDesc, nullptr, &s_RenderData.TextureArray);
        }

        // copy texture into array
        for (uint32_t mip = 0; mip < texture->GetTextureDesc().MipLevels; ++mip) {
            CopyTextureAttribs CopyAttribs(texture->GetTexture(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
                                           s_RenderData.TextureArray, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            CopyAttribs.SrcMipLevel = mip;
            CopyAttribs.DstMipLevel = mip;
            CopyAttribs.DstSlice    = s_RenderData.CurrentTextureSlot; // texture index
            Renderer::Get()->GetDeviceContext()->CopyTexture(CopyAttribs);
        }

        ++s_RenderData.CurrentTextureSlot;
    }

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
            LayoutElement{2, 0, 2, VT_FLOAT32, False}
        };

        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
        s_RenderData.QuadPipelineState->createInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
        
        s_RenderData.QuadPipelineState->Generate();
        
        s_RenderData.QuadPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerFrame")->Set(s_RenderData.PerFrameData->GetBuffer());

        s_RenderData.QuadPipelineState->GetPipelineState()->CreateShaderResourceBinding(&s_RenderData.QuadSRB, true);
    }

    void Renderer2D::InitTexturePipelineState()
    {
        TextureVertex vertex[] = {
            { glm::vec3{-0.5f, -0.5f, 0.f }, glm::vec2 { 0, 1 } },
            { glm::vec3{-0.5f,  0.5f, 0.f }, glm::vec2 { 0, 0 } },
            { glm::vec3{ 0.5f,  0.5f, 0.f }, glm::vec2 { 1, 0 } },
            { glm::vec3{ 0.5f, -0.5f, 0.f }, glm::vec2 { 1, 1 } }
        };
        s_RenderData.TextureVertexBuffer = RenderTool::CreateVertexBuffer(vertex, sizeof(vertex), 4);

        s_RenderData.TexturePipelineState = std::make_shared<RenderPipelineState>("Texture Pipeline State");
        s_RenderData.TextureShader = RenderTool::CreateShader("texture", "/shaders/texturevs.hlsl","/shaders/textureps.hlsl");
        s_RenderData.TexturePipelineState->createInfo.pVS = s_RenderData.TextureShader->GetVertexShader();
        s_RenderData.TexturePipelineState->createInfo.pPS = s_RenderData.TextureShader->GetPixelShader();
        
        
        LayoutElement layoutElement[] = {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}
        };

        s_RenderData.TexturePipelineState->createInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
        s_RenderData.TexturePipelineState->createInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
        
        // Texture
        ShaderResourceVariableDesc Vars[] = {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
        };
        s_RenderData.TexturePipelineState->createInfo.PSODesc.ResourceLayout.Variables = Vars;
        s_RenderData.TexturePipelineState->createInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

        // Sampler
        SamplerDesc SamLinearClampDesc {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, 
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
        };

        ImmutableSamplerDesc ImtblSamplers[] = {
            {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}
        };

        s_RenderData.TexturePipelineState->createInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
        s_RenderData.TexturePipelineState->createInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

        s_RenderData.TexturePipelineState->Generate();
        
        //s_RenderData.TexturePipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerObj")->Set(s_RenderData.PerObjData->GetBuffer());
        s_RenderData.TexturePipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerFrame")->Set(s_RenderData.PerFrameData->GetBuffer());

        s_RenderData.TexturePipelineState->GetPipelineState()->CreateShaderResourceBinding(&s_RenderData.TextureSRB, true);
    }
}