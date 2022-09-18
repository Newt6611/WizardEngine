#include "Example2D.h"


Example2D::Example2D() 
    : Layer("Example2D"), m_FlatColorPipeline("FlatColor"), 
    m_OrthographicCamera(-2.f, 2.f, -2.f, 2.f)
{
    InitConstBuffer();
    InitFlatRenderPipelineState();

    InitVertexBuffer();
    InitIndexBuffer();
}

Example2D::~Example2D()
{
    delete m_SRB;
}

void Example2D::OnEnter()
{

}

void Example2D::OnUpdate()
{
    const float clearColor[] = { 0.35f, 0.35f, 0.35f, 1.0f };
    
    ITextureView* pRTV = Wizard::RenderTool::GetCurrentBackBufferRTV();
    ITextureView* pDSV = Wizard::RenderTool::GetDepthBufferDSV();

    Wizard::RenderCommand::SetRenderTarget(1, &pRTV, pDSV);
    Wizard::RenderCommand::ClearRenderTarget(pRTV, clearColor);
    Wizard::RenderCommand::ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0);

    MapHelper<PerFrameConst> perFrameConst(Wizard::RenderTool::GetDeviceContext(), m_PerFrameConst->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
    perFrameConst->Projection = glm::transpose(m_OrthographicCamera.GetProjectionMatrix());
    perFrameConst->View = glm::transpose(m_OrthographicCamera.GetViewMatrix());

    MapHelper<PerObjConst> perObjConst(Wizard::RenderTool::GetDeviceContext(), m_PerObjConst->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
    perObjConst->World = glm::mat4(1.0);

    MapHelper<PSObjConst> psObjConst(Wizard::RenderTool::GetDeviceContext(), m_PSObjConst->GetBuffer(), MAP_WRITE, MAP_FLAG_DISCARD);
    psObjConst->Color = glm::vec4(1, 1, 0, 1);

    const uint64_t offset = 0;
    IBuffer* pBuffers[] = {m_VertexBuffer->GetBuffer()};
    Wizard::RenderCommand::SetVertexBuffers(0, 1, pBuffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    Wizard::RenderCommand::SetIndexBuffer(m_IndexBuffer->GetBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    Wizard::RenderCommand::SetPipelineState(m_FlatColorPipeline.GetPipelineState());
    Wizard::RenderCommand::CommitShaderResources(m_SRB);

    DrawIndexedAttribs drawAttrs;
    drawAttrs.IndexType = VT_UINT32;
    drawAttrs.NumIndices = 36;
    drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    Wizard::RenderCommand::DrawIndexed(drawAttrs);
}

void Example2D::OnEvent(Wizard::Event& e)
{
    
}

void Example2D::OnLeave()
{
    
}

void Example2D::InitFlatRenderPipelineState()
{
    m_FlatShader = Wizard::RenderTool::CreateShader("flat color", 
        "C:/Dev/Wizard/Sandbox/assets/shaders/flatvs.hlsl", 
        "C:/Dev/Wizard/Sandbox/assets/shaders/flatps.hlsl");

    m_FlatColorPipeline.createInfo.pVS = m_FlatShader->GetVertexShader();
    m_FlatColorPipeline.createInfo.pPS = m_FlatShader->GetPixelShader();

    LayoutElement layoutElement[] = {
        LayoutElement{0, 0, 3, VT_FLOAT32, False}
    };

    m_FlatColorPipeline.createInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElement;
    m_FlatColorPipeline.createInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElement);
    
    ShaderResourceVariableDesc vars[] = {
        { SHADER_TYPE_PIXEL, "g_Color", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE }
    };

    m_FlatColorPipeline.createInfo.PSODesc.ResourceLayout.Variables = vars;
    m_FlatColorPipeline.createInfo.PSODesc.ResourceLayout.NumVariables = _countof(vars);
    m_FlatColorPipeline.Generate();

    m_FlatColorPipeline.GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerFrame")->Set(m_PerFrameConst->GetBuffer());
    m_FlatColorPipeline.GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_VERTEX, "PerObj")->Set(m_PerObjConst->GetBuffer());
    m_FlatColorPipeline.GetPipelineState()->GetStaticVariableByName(SHADER_TYPE_PIXEL, "PSObjConst")->Set(m_PSObjConst->GetBuffer());

    m_FlatColorPipeline.GetPipelineState()->CreateShaderResourceBinding(&m_SRB, true);
}

void Example2D::InitVertexBuffer()
{
    struct Vertex
    {
        glm::vec3 position;
    };

    Vertex vertices[] = {
        glm::vec3(-1, -1, 0),
        glm::vec3(-1, 1, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(1, -1, 0),
    };

    m_VertexBuffer = Wizard::RenderTool::CreateVertexBuffer(vertices, sizeof(vertices), 4);    
}

void Example2D::InitIndexBuffer()
{
    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0
    };

    m_IndexBuffer = Wizard::RenderTool::CreateIndexBuffer(indices, 6);
}

void Example2D::InitConstBuffer()
{
    m_PerFrameConst = Wizard::RenderTool::CreateConstantBuffer<PerFrameConst>("PerFrameConst");
    m_PerObjConst = Wizard::RenderTool::CreateConstantBuffer<PerObjConst>("PerObjConst");
    m_PSObjConst = Wizard::RenderTool::CreateConstantBuffer<PSObjConst>("PSObj");
}