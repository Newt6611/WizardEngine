// TODO: remove vulkan in cmake
#pragma once
#include "wzpch.h"

#include "Core/Application.h"

#define PLATFORM_WIN32 1
#include "../thirdparty/DiligentCore/Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"


extern Wizard::Application* CreateApplication();

int main()
{
    Wizard::Application* app = CreateApplication();

    app->Run();

    delete app;
}