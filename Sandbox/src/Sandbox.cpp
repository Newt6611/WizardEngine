#include "Wizard.h"
#include "EntryPoint.h"
#include "Example2D.h"

class Sandbox : public Wizard::Application
{
public:
    Sandbox() 
    {
        PushLayer(new Example2D);
    }

    ~Sandbox() 
    {

    }
};

Wizard::Application* CreateApplication()
{
    return new Sandbox;
}