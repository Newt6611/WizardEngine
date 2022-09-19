#pragma once

#include "Log.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "LayerStack.h"

namespace Wizard {
    class Application 
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();

    protected:
        void OnUpdate(float delta);
        void OnEvent(Event& e);

        bool OnWindowClosed(WindowClosedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        void PushLayer(Layer* layer);

    private:
        Window* m_Window;
        Renderer* m_Renderer;
        LayerStack m_LayerStack;

        float m_LastTimeFrame;
    };
}
