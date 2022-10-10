#include "Wizard.h"
#include "EntryPoint.h"
#include "Example2D.h"
#include "Example3D.h"

class Sandbox : public Wizard::Application
{
public:
    Sandbox() 
    {
        PushLayer(new Example3D);
    }

    ~Sandbox() 
    {

    }
};

Wizard::Application* CreateApplication()
{
    return new Sandbox;
}