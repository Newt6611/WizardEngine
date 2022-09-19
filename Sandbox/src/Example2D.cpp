#include "Example2D.h"


Example2D::Example2D() 
    : Layer("Example2D"), m_CameraController(1280.f / 720.f)
{

}

Example2D::~Example2D()
{

}

void Example2D::OnEnter()
{

}

void Example2D::OnUpdate(float ts)
{
    m_CameraController.OnUpdate(ts);

    const float clearColor[] = { 0.35f, 0.35f, 0.35f, 1.0f };
    
    ITextureView* pRTV = Wizard::RenderTool::GetCurrentBackBufferRTV();
    ITextureView* pDSV = Wizard::RenderTool::GetDepthBufferDSV();

    Wizard::RenderCommand::SetRenderTarget(1, &pRTV, pDSV);
    Wizard::RenderCommand::ClearRenderTarget(pRTV, clearColor);
    Wizard::RenderCommand::ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0);

    Wizard::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Wizard::Renderer2D::DrawQuad({-0.5, 0}, {1, 1}, { 0, 1, 1 ,1});
    Wizard::Renderer2D::DrawQuad({ 0, 0.5}, {1, 1}, { 1, 1, 0 ,1});
}

void Example2D::OnEvent(Wizard::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Example2D::OnLeave()
{
    
}