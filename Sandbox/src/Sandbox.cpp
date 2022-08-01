#include "EntryPoint.h"
#include "Wizard/Application.h"

#include <iostream>

class Sandbox : public Wizard::Application
{
public:
    Sandbox() 
    {
        std::cout << "sandbox" << std::endl;
    }

    ~Sandbox() 
    {

    }
};

Wizard::Application* CreateApplication()
{
    return new Sandbox;
}