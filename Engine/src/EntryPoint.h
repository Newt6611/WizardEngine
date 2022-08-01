#pragma once
#include "Wizard/Application.h"
#include <iostream>

extern Wizard::Application* CreateApplication();

int main()
{
    Wizard::Application* app = CreateApplication();
    app->Run();

    delete app;

    std::cout << "done" << std::endl;
}