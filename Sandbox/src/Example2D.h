#include "Wizard.h"

class Example2D : public Wizard::Layer
{
public:
    Example2D();

    ~Example2D();

    void OnEnter() override;
    void OnUpdate(float ts) override;
    void OnEvent(Wizard::Event& e);
    void OnLeave() override;

private:
    Wizard::OrthographicCameraController m_CameraController;
    std::shared_ptr<Wizard::Texture> m_Texture;
};