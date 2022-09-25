#include "Example2D.h"

Example2D::Example2D()
    : Layer("Example2D"), m_CameraController(1600.f / 900.f)
{
    m_Texture = Wizard::RenderTool::CreateTexture("textures/DGLogo.png", true);
    m_ChernoLogo = Wizard::RenderTool::CreateTexture("textures/ChernoLogo.png", true);

    e1 = m_Scene.CreateEntity();
    m_Scene.AddComponent<Wizard::TransformComponent>(e1, glm::vec2(-1.f, 0.f));
    m_Scene.AddComponent<Wizard::SpriteRendererComponent>(e1, m_Texture);

    e2 = m_Scene.CreateEntity();
    m_Scene.AddComponent<Wizard::TransformComponent>(e2, glm::vec2(0.f, 0.f));
    m_Scene.AddComponent<Wizard::SpriteRendererComponent>(e2, m_ChernoLogo);

    e3 = m_Scene.CreateEntity();
    m_Scene.AddComponent<Wizard::TransformComponent>(e3, glm::vec2(1.f, 0.f));
    m_Scene.AddComponent<Wizard::SpriteRendererComponent>(e3, glm::vec4(0.f, 1.f, 0.f, 1.f));
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

    ITextureView* pRTV = Wizard::RenderTool::GetCurrentBackBufferRTV();
    ITextureView* pDSV = Wizard::RenderTool::GetDepthBufferDSV();

    Wizard::RenderCommand::SetRenderTarget(1, &pRTV, pDSV);
    Wizard::RenderCommand::ClearRenderTarget(pRTV);
    Wizard::RenderCommand::ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0);

    Wizard::Renderer2D::BeginScene(m_CameraController.GetCamera());
    
    m_Scene.OnUpdate(ts);
    
    Wizard::Renderer2D::EndScene();

    WZ_TRACE(Wizard::RenderTool::GetDrawCall());
}

void Example2D::OnEvent(Wizard::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Example2D::OnLeave()
{
    
}