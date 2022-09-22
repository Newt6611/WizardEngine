#include "Example2D.h"

Example2D::Example2D()
    : Layer("Example2D"), m_CameraController(1600.f / 900.f)
{
    m_Texture = Wizard::RenderTool::CreateTexture("textures/DGLogo.png", true);
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
    //const float clearColor[] = { 0.f, 0.f, 0.f, 1.0f };
    
    ITextureView* pRTV = Wizard::RenderTool::GetCurrentBackBufferRTV();
    ITextureView* pDSV = Wizard::RenderTool::GetDepthBufferDSV();

    Wizard::RenderCommand::SetRenderTarget(1, &pRTV, pDSV);
    Wizard::RenderCommand::ClearRenderTarget(pRTV, clearColor);
    Wizard::RenderCommand::ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0);

    Wizard::Renderer2D::BeginScene(m_CameraController.GetCamera());
    
    static float r = 0;
    r += ts * 100;
    if (r >= 360) {
        r = 0;
    }
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            if ((i + j) % 2 == 0) {
                Wizard::Renderer2D::DrawQuad({i, j}, {1, 1}, {0, 0, 0, 1});
            } else {
                Wizard::Renderer2D::DrawQuad({i, j}, {1, 1}, {1, 1, 1, 1});
            }
        }
    }
    
    Wizard::Renderer2D::EndScene();
}

void Example2D::OnEvent(Wizard::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Example2D::OnLeave()
{
    
}