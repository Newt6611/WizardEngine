#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Events/MouseEvents.h"
#include <GLFW/glfw3.h>

namespace Wizard
{
    class Window
    {
    public:
        static Window* Get() {
            static Window INSTANCE;
            return &INSTANCE;
        }

        Window() {}
        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;

        virtual ~Window() {
            ShutDown();
        }

        void Init(uint32_t width, uint32_t height);
        void ShutDown();

        inline GLFWwindow* GetWindow() {
            return m_Window;
        }

        void OnUpdate();

        inline bool WindowShouldClose() {
            return glfwWindowShouldClose(m_Window);
        }

        void SetEventCallback(const std::function<void(Event&)>& e) {
            m_Data.EventCallback = e;
        }

    private:
        
        
    private:
        GLFWwindow* m_Window = nullptr;
        bool m_WindowShouldClose = false;

        struct WindowData
        {   
            const char* Title = "";
            uint32_t Width = 0;
            uint32_t Height = 0;
            std::function<void(Wizard::Event&)> EventCallback;
        };

        WindowData m_Data;
    };
}
