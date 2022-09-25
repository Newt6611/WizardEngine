#include "wzpch.h"
#include "Window.h"
#include "Log.h"

// #ifdef WZ_WINDOWS
// #pragma comment(lib, "comctl32.lib")
// #include <CommCtrl.h>
// #endif

namespace Wizard
{
    // static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
    // {
    //     switch(msg)
    //     {
    //     case WM_CREATE:
    //         WZ_ENGINE_INFO("Create");
    //         break;
    //     case WM_DESTROY:
    //         break;
    //     case WM_PAINT:
    //         break;
    //     case WM_COMMAND:
    //         break;
    //     default:
    //         return DefWindowProc(hwnd, msg, wParam, lParam);
    //     }
    //     return 0;
    // }

    void Window::Init(uint32_t width, uint32_t height)  
    {
        m_Data.Width = width;
        m_Data.Height = height;
        m_Data.Title = "Wizard Engine";
        
        if (!glfwInit()) {
            WZ_ENGINE_ERROR("Init GLFW Failed");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, NULL, NULL);
        if (!m_Window) {
            glfwTerminate();
            WZ_ENGINE_ERROR("Create GLFW Window Failed");
            return;
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        
#if defined(WZ_WINDOWS)
        m_NativeWindow = glfwGetWin32Window(m_Window);
#endif
        

        // events
        // window closed
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowClosedEvent e;
            data.EventCallback(e);
        });

        // window resized
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowResizeEvent e(width, height);
            data.EventCallback(e);
        });

        // window position
        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowPositionEvent e(x, y);
            data.EventCallback(e);
        });

        // mouse position
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MousePositionEvent e(x, y);
            data.EventCallback(e);
        });

        // mouse button
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseButtonClick e(button, action);
            data.EventCallback(e);
        });

        // mouse scroll
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent e((float)xoffset, (float)yoffset);
            data.EventCallback(e);
        });
    }

    void Window::OnUpdate()
    {
        //glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::ShutDown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}