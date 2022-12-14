#include "Wizard.h"

class Example2D : public Wizard::Layer
{
public:
    Example2D();

    ~Example2D();

    void OnEnter() override;
    void OnUpdate(float ts) override;
    void OnEvent(Wizard::Event& e) override;
    void OnLeave() override;

private:
    Wizard::OrthographicCameraController m_CameraController;
    Wizard::Scene m_Scene;
    
    Wizard::Entity e1;
    Wizard::Entity e2;
    Wizard::Entity e3;
    std::shared_ptr<Wizard::Texture2D> m_Texture;
    std::shared_ptr<Wizard::Texture2D> m_ChernoLogo;
};