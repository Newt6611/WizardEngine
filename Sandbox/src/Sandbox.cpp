#include "Wizard.h"
#include "EntryPoint.h"

class Sandbox : public Wizard::Application
{
public:
    Sandbox() 
    {
        
    }

    ~Sandbox() 
    {

    }
};

Wizard::Application* CreateApplication()
{
    return new Sandbox;
}