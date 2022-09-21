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
        //std::shared_ptr<ConstantBuffer<PerObjConst>> PerObjData;


        // Batch Rendering
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;

        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<IndexBuffer> QuadIndexBuffer;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase;
        QuadVertex* QuadVertexBufferPtr;
        //

        // Flat Color Render Pipeline //
        std::shared_ptr<VertexBuffer> FlatVertexBuffer;
        std::shared_ptr<RenderPipelineState> FlatPipelineState;
        std::shared_ptr<Shader> FlatQuadShader;
        std::shared_ptr<ConstantBuffer<FlatPSConst>> FlatPSData;
        RefCntAutoPtr<IShaderResourceBinding> FlatSRB;
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

        uint32_t* quadIndices = new uint32_t[s_RenderData.MaxIndices];
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
        delete[] quadIndices;

        s_RenderData.PerFrameData = RenderTool::CreateConstantBuffer<PerFrameConst>("PerFrameConstant");
        //s_RenderData.PerObjData = RenderTool::CreateConstantBuffer<PerObjConst>("PerObjConstant");

        InitFlatPipelineState();

        //InitTexturePipelineState();
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_RenderData.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
        s_RenderData.QuadIndexCount = 0;

        MapHelper<PerFrameConst> perFrameConst(RenderTool::GetDeviceContext(), s_RenderData.PerFrameData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);

        perFrameConst->ViewProjection = glm::transpose(camera.GetProjectionMatrix() * camera.GetViewMatrix());
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::Flush()
    {

        const uint64_t offset = 0;
        IBuffer* pBuffers[] = {s_RenderData.QuadVertexBuffer->GetBuffer()};
        RenderCommand::SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        RenderCommand::SetIndexBuffer(s_RenderData.QuadIndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase);
        RenderCommand::UpdateBuffer(s_RenderData.QuadVertexBuffer->GetBuffer(), 0, dataSize, s_RenderData.QuadVertexBufferBase);

        RenderCommand::SetPipelineState(s_RenderData.FlatPipelineState->GetPipelineState());
        RenderCommand::CommitShaderResources(s_RenderData.FlatSRB);

        DrawIndexedAttribs drawAttrs;
        drawAttrs.IndexType = VT_UINT32;
        drawAttrs.NumIndices = s_RenderData.QuadIndexCount;
        drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        RenderCommand::DrawIndexed(drawAttrs);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, float rotate)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0), size, color, rotate);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotate)
    {
        s_RenderData.QuadVertexBufferPtr->Position = position;
        
        s_RenderData.QuadVertexBufferPtr->Color = color;
        s_RenderData.QuadVertexBufferPtr->UV = glm::vec2(0, 1);
        s_RenderData.QuadVertexBufferPtr++;

        s_RenderData.QuadVertexBufferPtr->Position = {position.x, position.y + size.y, position.z};
        s_RenderData.QuadVertexBufferPtr->Color = color;
        s_RenderData.QuadVertexBufferPtr->UV = glm::vec2(0, 0);
        s_RenderData.QuadVertexBufferPtr++;

        s_RenderData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z};
        s_RenderData.QuadVertexBufferPtr->Color = color;
        s_RenderData.QuadVertexBufferPtr->UV = glm::vec2(1, 0);
        s_RenderData.QuadVertexBufferPtr++;

        s_RenderData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z};
        s_RenderData.QuadVertexBufferPtr->Color = color;
        s_RenderData.QuadVertexBufferPtr->UV = glm::vec2(1, 1);
        s_RenderData.QuadVertexBufferPtr++;

        s_RenderData.QuadIndexCount += 6;
    }

    // void Renderer2D:: DrawQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& color, float rotate)
    // {
    //     DrawQuad(glm::vec3(position.x, position.y, 0), size, texture, color, rotate);
    // }
    
    // void Renderer2D:: DrawQuad(const glm::vec3& position, const glm::vec2& size, Texture* texture, const glm::vec4& color, float rotate)
    // {
    //     MapHelper<PerObjConst> perObjConst(RenderTool::GetDeviceContext(), s_RenderData.PerObjData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
    //     glm::mat4 world = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotate), {0, 0, 1}) *
    //         glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1));

    //     perObjConst->World = glm::transpose(world);

    //     MapHelper<FlatPSConst> flatPSConst(RenderTool::GetDeviceContext(), s_RenderData.FlatPSData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
    //     flatPSConst->Color = color;

    //     s_RenderData.TextureSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(texture->GetTextureView());

    //     // Draw
    //     const uint64_t offset = 0;
    //     IBuffer* pBuffers[] = {s_RenderData.TextureVertexBuffer->GetBuffer()};
    //     RenderCommand::SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    //     RenderCommand::SetIndexBuffer(s_RenderData.FlatQuadIndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    //     RenderCommand::SetPipelineState(s_RenderData.TexturePipelineState->GetPipelineState());
    //     RenderCommand::CommitShaderResources(s_RenderData.TextureSRB);

    //     DrawIndexedAttribs drawAttrs;
    //     drawAttrs.IndexType = VT_UINT32;
    //     drawAttrs.NumIndices = s_RenderData.FlatQuadIndexBuffer->GetCount();
    //     drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    //     RenderCommand::DrawIndexed(drawAttrs);
    // }


    void Renderer2D::InitFlatPipelineState()
    {
        s_RenderData.FlatPipelineState = std::make_shared<RenderPipelineState>("Flat Pipeline State");
        s_RenderData.FlatQuadShader = RenderTool::CreateShader("flat color", "/shaders/flatvs.hlsl","/shaders/flatps.hlsl");
        s_RenderData.FlatPipelineState->createInfo.pVS = s_RenderData.FlatQuadShader->GetVertexShader();
        s_RenderData.FlatPipelineState->createInfo.pPS = s_RenderData.FlatQuadShader->GetPixelShader();
        s_RenderData.FlatPipelineState->createInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        LayoutElement layoutElement[] = {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 4, VT_FLOAT32, False},
            LayoutElement{2, 0, 2, VT_FLOAT32, False}
        };

        s_RenderData.FlatPipelineState->createInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
        s_RenderData.FlatPipelineState->createInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
        
        s_RenderData.FlatPipelineState->Generate();
        
        //s_RenderData.FlatPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerObj")->Set(s_RenderData.PerObjData->GetBuffer());
        s_RenderData.FlatPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerFrame")->Set(s_RenderData.PerFrameData->GetBuffer());

        //s_RenderData.FlatPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_PIXEL, "PSObjConst")->Set(s_RenderData.FlatPSData->GetBuffer());

        s_RenderData.FlatPipelineState->GetPipelineState()->CreateShaderResourceBinding(&s_RenderData.FlatSRB, true);
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

        s_RenderData.TexturePipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_PIXEL, "PSObjConst")->Set(s_RenderData.FlatPSData->GetBuffer());

        s_RenderData.TexturePipelineState->GetPipelineState()->CreateShaderResourceBinding(&s_RenderData.TextureSRB, true);
    }
}