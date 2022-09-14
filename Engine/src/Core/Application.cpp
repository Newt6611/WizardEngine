#include "Application.h"

namespace Wizard {
    Application::Application()
    {
        Log::Init();
        WZ_ENGINE_INFO("Wizard Start");

        m_Window = Window::Get();
        m_Window->Init(800, 600);
        m_Window->SetEventCallback(BIND_FN_EVENT(Application::OnEvent));

        m_Renderer = std::make_unique<Renderer>();
        m_Renderer->Init(RendererAPI::D3D12, m_Window);
        m_Renderer->InitTest();
        
    }

    Application::~Application()
    {
        
    } 

    void Application::Run()
    {
        WZ_ENGINE_INFO("run");
        while (!m_Window->WindowShouldClose()) {
            OnUpdate();
        }
        
    }

    void Application::OnUpdate()
    {
        m_Window->OnUpdate();
        m_Renderer->Render();
        m_Renderer->Present();
    }

    void Application::OnEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowClosedEvent>(BIND_FN_EVENT(Application::OnWindowClosed));
    }

    bool Application::OnWindowClosed(WindowClosedEvent& e)
    {
        WZ_ENGINE_INFO("WindowClosed");
        return true;
    }
}

Wizard::Application* CreateApplication();