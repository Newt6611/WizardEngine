#pragma once
#include "wzpch.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

extern Wizard::Application* CreateApplication();

int main()
{
    Wizard::Application* app = CreateApplication();

    app->Run();

    delete app;
}