#include "wzpch.h"
#include "Application.h"

namespace Wizard {
    Application::Application()
    {
        Log::Init();
        WZ_ENGINE_INFO("Wizard Start");

        m_Window = Window::Get();
        m_Window->Init(1280, 720);
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
    }

    Application::~Application()
    {
        m_Renderer->Shutdown();
    } 

    void Application::Run()
    {
        WZ_ENGINE_INFO("run");
        while (!m_Window->WindowShouldClose()) {
            float time = glfwGetTime();
            float ts = time - m_LastTimeFrame;
            m_LastTimeFrame = time;
            
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(ts);
            }
            
            OnUpdate(ts);
        }
    }

    void Application::OnUpdate(float ts)
    {
        m_Window->OnUpdate();
        m_Renderer->Present();
    }

    void Application::OnEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowClosedEvent>(BIND_FN_EVENT(Application::OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_FN_EVENT(Application::OnWindowResize));

        for (std::vector<Layer*>::reverse_iterator it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            (*it)->OnEvent(e);
            if (e.Handled) 
                break;
        }
    }

    bool Application::OnWindowClosed(WindowClosedEvent& e)
    {
        WZ_ENGINE_INFO("WindowClosed");
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        m_Renderer->OnResize(e.GetWidth(), e.GetHeight());
        return true;
    }

    void Application::PushLayer(Layer* layer) 
    {
        m_LayerStack.PushLayer(layer);
        layer->OnEnter();
    }
}

Wizard::Application* CreateApplication();