#include "Application.h"
#include <iostream>

namespace Wizard {
    Application::Application()
    {

    }

    Application::~Application()
    {

    } 

    void Application::Run()
    {
        std::cout << "run" << std::endl;
    }

    Application* CreateApplication();
}