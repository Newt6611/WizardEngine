#include "Application.h"

namespace Wizard {
    Application::Application()
    {
        Log::Init();

        WZ_ENGINE_INFO("Wizard Start");
    }

    Application::~Application()
    {

    } 

    void Application::Run()
    {
        WZ_ENGINE_INFO("run");
    }

    void Application::OnUpdate()
    {

    }    
}
Wizard::Application* CreateApplication();