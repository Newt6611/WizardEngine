#include "EditorLayer.h"
using namespace Wizard;

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_CameraController(1600.f / 900.f)
{
    InitImGui();
}

EditorLayer::~EditorLayer()
{
    m_ApiRenderer.release();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorLayer::OnEnter()
{

}

void EditorLayer::OnUpdate(float ts)
{
    m_CameraController.OnUpdate(ts);

    ITextureView* pRTV = Wizard::RenderTool::GetCurrentBackBufferRTV();
    ITextureView* pDSV = Wizard::RenderTool::GetDepthBufferDSV();

    Wizard::RenderCommand::SetRenderTarget(1, &pRTV, pDSV);
    Wizard::RenderCommand::ClearRenderTarget(pRTV);
    Wizard::RenderCommand::ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0);
    
    Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if ((i + j) % 2 == 0) 
                Renderer2D::DrawQuad({i, j}, {1, 1}, {0, 0, 0, 1});
            else
                Renderer2D::DrawQuad({i, j}, {1, 1}, {1, 1, 1, 1});
        }
    }

    Renderer2D::EndScene();


    NewFrame();
    
    ImGui::Begin("Render State");
    
    ImGui::Text("Draw Calls: %d", RenderTool::GetDrawCall());

    ImGui::End();

    EndFrame();
}

void EditorLayer::OnEvent(Wizard::Event& e)
{
    m_CameraController.OnEvent(e);
}

void EditorLayer::OnLeave()
{
    
}

void EditorLayer::InitImGui()
{
    IRenderDevice* device = RenderTool::GetDevice();
    SwapChainDesc swapChainDesc = RenderTool::GetSwapChain()->GetDesc();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

// TODO setup different platforms
    ImGui_ImplGlfw_InitForOther(RenderTool::GetWindow()->GetWindow(), true);
    //ImGui_ImplDX12_Init(,);
    m_ApiRenderer = std::make_unique<ImGuiDiligentRenderer>(
        device, swapChainDesc.ColorBufferFormat,
        swapChainDesc.DepthBufferFormat,
        ImGuiImplDiligent::DefaultInitialVBSize,
        ImGuiImplDiligent::DefaultInitialIBSize);
}

void EditorLayer::NewFrame()
{
    SwapChainDesc desc = Wizard::RenderTool::GetSwapChain()->GetDesc();
    m_ApiRenderer->NewFrame(desc.Width, desc.Height, desc.PreTransform);
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorLayer::EndFrame()
{
    ImGui::Render();
    m_ApiRenderer->RenderDrawData(RenderTool::GetDeviceContext(), ImGui::GetDrawData());
}

