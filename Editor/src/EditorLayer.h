#include "Wizard.h"

class EditorLayer : public Wizard::Layer
{
public:
    EditorLayer();

    ~EditorLayer();

    void OnEnter() override;
    void OnUpdate(float ts) override;
    void OnEvent(Wizard::Event& e) override;
    void OnLeave() override;

private:
    void InitImGui();
    void NewFrame();
    void EndFrame();

private:
    std::unique_ptr<ImGuiDiligentRenderer> m_ApiRenderer;

    Wizard::OrthographicCameraController m_CameraController;
    Wizard::Scene m_Scene;
};