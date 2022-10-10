#pragma once
#include "Wizard.h"
#include "Panels/ComponentPanel.h"
#include "ImGuiD3D12.h"

class EditorLayer : public Wizard::Layer
{
public:
    EditorLayer();

    ~EditorLayer();

    void OnEnter() override;
    void OnUpdate(float ts) override;
    void OnEvent(Wizard::Event& e) override;
    void OnLeave() override;

    void OnGUI();

private:
    void InitImGui();
    void NewFrame();
    void EndFrame();
private:
    ComponentPanel m_ComponentPanel;
    Wizard::OrthographicCameraController m_CameraController;
    Wizard::Scene m_Scene;
    Wizard::Entity m_SelectedEntity = entt::null;


    std::unique_ptr<ImGuiDiligentRenderer> m_GuiRenderer;
    //ImGuiD3D12 renderer;

    Wizard::Entity e1;
    Wizard::Entity e2;
    Wizard::Entity e3;
    std::shared_ptr<Wizard::Texture2D> m_Texture;
    std::shared_ptr<Wizard::Texture2D> m_ChernoLogo;
};