#include "Wizard.h"

class Example3D : public Wizard::Layer
{
public:
    Example3D();

    ~Example3D();

    void OnEnter() override;
    void OnUpdate(float ts) override;
    void OnEvent(Wizard::Event& e) override;
    void OnLeave() override;

private:
};