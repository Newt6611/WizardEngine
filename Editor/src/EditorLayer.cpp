#include "EditorLayer.h"
#include "TestController.h"

using namespace Wizard;

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_CameraController(1600.f / 900.f)
{
    InitImGui();

    m_Texture = Wizard::RenderTool::CreateTexture("textures/DGLogo.png", true);
    m_ChernoLogo = Wizard::RenderTool::CreateTexture("textures/ChernoLogo.png", true);

    {
        // e1 = m_Scene.CreateEntity();
        // m_Scene.AddComponent<TagComponent>(e1).Name = "E1";
        // m_Scene.AddComponent<TransformComponent>(e1, glm::vec2{-1.f, 0.f});
        // m_Scene.AddComponent<SpriteRendererComponent>(e1).Color = glm::vec4(0, 1, 1, 1);

        // e2 = m_Scene.CreateEntity();
        // m_Scene.AddComponent<TagComponent>(e2).Name = "E2";
        // m_Scene.AddComponent<SpriteRendererComponent>(e2, m_Texture);
        // auto& t2 = m_Scene.AddComponent<TransformComponent>(e2, glm::vec2(0.f, 5.f));

        // e3 = m_Scene.CreateEntity();
        // m_Scene.AddComponent<TagComponent>(e3).Name = "E3";
        // auto& t3 = m_Scene.AddComponent<TransformComponent>(e3, glm::vec2(0.f, -2.f));
        // t3.Scale = glm::vec3(20, 1, 1);
        // m_Scene.AddComponent<SpriteRendererComponent>(e3, glm::vec4(0.f, 1.f, 0.f, 1.f));
        // auto& r3 = m_Scene.AddComponent<Rigidbody2DComponent>(e3);
        // m_Scene.AddComponent<BoxCollider2DComponent>(e3);

        // SceneSerializer::Serialize("scenes/example.wizard", m_Scene);
    }
    
    e1 = m_Scene.CreateEntity();
    auto& t1 = m_Scene.AddComponent<TransformComponent>(e1);
    t1.Position = glm::vec3(0.f, 0.f, 0.f);
    auto& s1 = m_Scene.AddComponent<SpriteRendererComponent>(e1);
    s1.Color = glm::vec4(1.f, 0.f, 0.f, 1.f);

    m_Scene.AddComponent<ScriptingComponent>(e1, new TestController());
    // SceneSerializer::Deserialize("scenes/example.wizard", m_Scene);
}

EditorLayer::~EditorLayer()
{
    m_GuiRenderer.release();
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
    ITextureView* pMPK = Wizard::RenderTool::GetMousePickingView();

    ITextureView* views[] = { pRTV,  pMPK };
    Wizard::RenderCommand::SetRenderTarget(2, views, pDSV);
    Wizard::RenderCommand::ClearRenderTarget(pMPK);
    Wizard::RenderCommand::ClearRenderTarget(pRTV);
    Wizard::RenderCommand::ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0);
    
    if (Input::IsMouseButtonPressed(Mouse::ButtonLeft)) {
        uint32_t e = RenderTool::GetMousePointingEntity() - 1;
        if (e != -1) {
            m_SelectedEntity = (entt::entity)e;
        }
    }

    {
        static float speed = 15;
        // Update e1 position
        auto& t1 = m_Scene.GetComponent<TransformComponent>(e1);
        if (Input::IsKeyPressed(Key::W)) {
            t1.Position.y += speed * ts;
        } else if (Input::IsKeyPressed(Key::S)) {
            t1.Position.y -= speed * ts;
        }

        if (Input::IsKeyPressed(Key::D)) {
            t1.Position.x += speed * ts;
        } else if (Input::IsKeyPressed(Key::A)) {
            t1.Position.x -= speed * ts;
        }
    }


    Renderer2D::BeginScene(m_CameraController.GetCamera());
    m_Scene.OnUpdate(ts);
    Renderer2D::EndScene();

    NewFrame();
    OnGUI();
    EndFrame();
}

void EditorLayer::OnGUI()
{
    // Render State
    ImGui::Begin("Render State");
    ImGui::Text("Draw Calls: %d", RenderTool::GetDrawCall());
    ImGui::Text("Quads: %d", Renderer2D::GetQuads());
    ImGui::Text("Vertices: %d", Renderer2D::GetVertices());
    ImGui::Text("Indices: %d", Renderer2D::GetIndices());
    ImGui::Text("Pointing Entity: %d", RenderTool::GetMousePointingEntity());
    ImGui::End();

    ImGui::Begin("ToolBar");
    if (ImGui::Button("Simulate Pyhsics")) {
        m_Scene.OnSimulatePhysicsStart();
    }

    if (ImGui::Button("Save File")) {
        SceneSerializer::Serialize("scenes/example.wizard", m_Scene);
    }
    ImGui::End();
    
    if (m_SelectedEntity != entt::null) {
        m_ComponentPanel.DrawComponent(m_Scene, m_SelectedEntity);
    }
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
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOther(RenderTool::GetWindow()->GetWindow(), true);
    m_GuiRenderer = std::make_unique<ImGuiDiligentRenderer>(
        device, swapChainDesc.ColorBufferFormat,
        swapChainDesc.DepthBufferFormat,
        ImGuiImplDiligent::DefaultInitialVBSize,
        ImGuiImplDiligent::DefaultInitialIBSize
    );
}

void EditorLayer::NewFrame()
{
    SwapChainDesc desc = RenderTool::GetSwapChain()->GetDesc();
    m_GuiRenderer->NewFrame(desc.Width, desc.Height, desc.PreTransform);
    //renderer.NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorLayer::EndFrame()
{
    ImGui::Render();
    m_GuiRenderer->RenderDrawData(RenderTool::GetDeviceContext(), ImGui::GetDrawData());
    //renderer.EndFrame();
}