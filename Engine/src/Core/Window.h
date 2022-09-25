#pragma once
#include "Core.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Events/MouseEvents.h"
#include "Core/Log.h"


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

        inline void* GetNativeWindow() {
            if (!m_NativeWindow) WZ_ENGINE_ERROR("Native Window is nullptr"); 
            return m_NativeWindow;     
        }

        void OnUpdate();

        inline bool WindowShouldClose() {
            return glfwWindowShouldClose(m_Window);
        }

        void SetEventCallback(const std::function<void(Event&)>& e) {
            m_Data.EventCallback = e;
        }
        
        
    private:
        GLFWwindow* m_Window = nullptr;
        void* m_NativeWindow = nullptr;
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
