#pragma once
#include "wzpch.h"
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
        GLFWwindow* m_Window;
        bool m_WindowShouldClose;

        struct WindowData
        {   
            const char* Title;
            uint32_t Width;
            uint32_t Height;
            std::function<void(Wizard::Event&)> EventCallback;
        };

        WindowData m_Data;
    };
}