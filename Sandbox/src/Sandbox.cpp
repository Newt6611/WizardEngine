#include "Wizard.h"
#include "EntryPoint.h"

class Sandbox : public Wizard::Application
{
public:
    Sandbox() 
    {
        WZ_INFO("init sandbox");
    }

    ~Sandbox() 
    {

    }
};

Wizard::Application* CreateApplication()
{
    return new Sandbox;
}