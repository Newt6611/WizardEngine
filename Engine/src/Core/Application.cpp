#include "wzpch.h"
#include "Application.h"

namespace Wizard {
    Application::Application()
    {
        Log::Init();
        WZ_ENGINE_INFO("Wizard Start");

        m_Window = Window::Get();
        m_Window->Init(800, 600);
        m_Window->SetEventCallback(BIND_FN_EVENT(Application::OnEvent));

        m_Renderer = Renderer::Get();

#ifdef WZ_WINDOWS
        m_Renderer->Init(RendererAPI::D3D12, m_Window);
#endif
#ifdef WZ_APPLE
        m_Renderer->Init(RendererAPI::Metal, m_Window);
#endif
#ifdef WZ_LINUX
        m_Renderer->Init(RendererAPI::Vulkan, m_Window);
#endif

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
        m_Renderer->Update();
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