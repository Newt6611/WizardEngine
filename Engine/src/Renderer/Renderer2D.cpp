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
        glm::vec2 TexCoord;
    };

    struct Render2DData
    {
        std::shared_ptr<ConstantBuffer<PerFrameConst>> PerFrameData;
        std::shared_ptr<ConstantBuffer<PerObjConst>> PerObjData;

        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<IndexBuffer> QuadIndexBuffer;

        // Flat Color Render Pipeline
        std::shared_ptr<RenderPipelineState> FlatPipelineState;
        std::shared_ptr<Shader> FlatQuadShader;
        std::shared_ptr<ConstantBuffer<FlatPSConst>> FlatPSData;
        RefCntAutoPtr<IShaderResourceBinding> FlatSRB;
        //
    };

    static Render2DData* s_RenderData;

    void Renderer2D::Init()
    {
        s_RenderData = new Render2DData;
        s_RenderData->PerFrameData = RenderTool::CreateConstantBuffer<PerFrameConst>("PerFrameConstant");
        s_RenderData->PerObjData = RenderTool::CreateConstantBuffer<PerObjConst>("PerObjConstant");

        QuadVertex quadVertex[] = {
            { glm::vec3{-0.5f, -0.5f, 0.f}, glm::vec2{ 0.f, 1.f } },
            { glm::vec3{-0.5f,  0.5f, 0.f}, glm::vec2{ 0.f, 0.f } },
            { glm::vec3{ 0.5f,  0.5f, 0.f}, glm::vec2{ 1.f, 0.f } },
            { glm::vec3{ 0.5f, -0.5f, 0.f}, glm::vec2{ 1.f, 1.f } }
        };
        s_RenderData->QuadVertexBuffer = RenderTool::CreateVertexBuffer(quadVertex, sizeof(quadVertex), 4);

        uint32_t quadIndices[] = {
            0, 1, 2, 2, 3, 0
        };
        s_RenderData->QuadIndexBuffer = RenderTool::CreateIndexBuffer(quadIndices, 6);

        s_RenderData->FlatPSData = RenderTool::CreateConstantBuffer<FlatPSConst>("FlatPSConstant");
        InitFlatPipelineState();
    }

    void Renderer2D::Shutdown()
    {
        delete s_RenderData;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        MapHelper<PerFrameConst> perFrameConst(RenderTool::GetDeviceContext(), s_RenderData->PerFrameData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
        perFrameConst->View = glm::transpose(camera.GetViewMatrix());
        perFrameConst->Projection = glm::transpose(camera.GetProjectionMatrix());
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        MapHelper<PerObjConst> perObjConst(RenderTool::GetDeviceContext(), s_RenderData->PerObjData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
        glm::mat4 world(1.0f);
        world = glm::translate(world, glm::vec3(position.x, position.y, 0)) * 
            glm::scale(world, glm::vec3(size.x, size.y, 0));

        perObjConst->World = glm::transpose(world);

        MapHelper<FlatPSConst> flatPSConst(RenderTool::GetDeviceContext(), s_RenderData->FlatPSData->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
        flatPSConst->Color = color;

        // Draw
        const uint64_t offset = 0;
        IBuffer* pBuffers[] = {s_RenderData->QuadVertexBuffer->GetBuffer()};
        RenderCommand::SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        RenderCommand::SetIndexBuffer(s_RenderData->QuadIndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        RenderCommand::SetPipelineState(s_RenderData->FlatPipelineState->GetPipelineState());
        RenderCommand::CommitShaderResources(s_RenderData->FlatSRB);

        DrawIndexedAttribs drawAttrs;
        drawAttrs.IndexType = VT_UINT32;
        drawAttrs.NumIndices = s_RenderData->QuadIndexBuffer->GetCount();
        drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        RenderCommand::DrawIndexed(drawAttrs);
    }

    void Renderer2D::InitFlatPipelineState()
    {
        s_RenderData->FlatPipelineState = std::make_shared<RenderPipelineState>("Flat Pipeline State");
        s_RenderData->FlatQuadShader = RenderTool::CreateShader("flat color", "/shaders/flatvs.hlsl","/shaders/flatps.hlsl");
        s_RenderData->FlatPipelineState->createInfo.pVS = s_RenderData->FlatQuadShader->GetVertexShader();
        s_RenderData->FlatPipelineState->createInfo.pPS = s_RenderData->FlatQuadShader->GetPixelShader();
        
        s_RenderData->FlatPipelineState->createInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        
        LayoutElement layoutElement[] = {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}
        };

        s_RenderData->FlatPipelineState->createInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
        s_RenderData->FlatPipelineState->createInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
        
        ShaderResourceVariableDesc vars[] = {
            { SHADER_TYPE_PIXEL, "g_Color", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE }
        };

        s_RenderData->FlatPipelineState->createInfo.PSODesc.ResourceLayout.Variables = vars;
        s_RenderData->FlatPipelineState->createInfo.PSODesc.ResourceLayout.NumVariables = _countof(vars);

        s_RenderData->FlatPipelineState->Generate();
        
        s_RenderData->FlatPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerObj")->Set(s_RenderData->PerObjData->GetBuffer());
        s_RenderData->FlatPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerFrame")->Set(s_RenderData->PerFrameData->GetBuffer());

        s_RenderData->FlatPipelineState->GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_PIXEL, "PSObjConst")->Set(s_RenderData->FlatPSData->GetBuffer());

        s_RenderData->FlatPipelineState->GetPipelineState()->CreateShaderResourceBinding(&s_RenderData->FlatSRB, true);
    }
}