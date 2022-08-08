#pragma once
#include "wzpch.h"
#include "Log.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Core.h"
#include "Window.h"
#include "Renderer/Renderer.h"

namespace Wizard {
    class Application 
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();

        void Print(int);

    protected:
        void OnUpdate();
        void OnEvent(Event& e);


        bool OnWindowClosed(WindowClosedEvent& e);

    private:
        Window* m_Window;
        std::unique_ptr<Renderer> m_Renderer;
    };
}
