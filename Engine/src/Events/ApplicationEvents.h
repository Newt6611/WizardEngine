#pragma once
#include "Event.h"

namespace Wizard
{
    class WindowClosedEvent : public Event
    {
    public:
        WindowClosedEvent() { }

        ~WindowClosedEvent() { }

        static EventType GetStaticEventType() {
            return EventType::WindowClose;
        }

        EventType GetEventType() override {
            return EventType::WindowClose;
        }

        std::string GetDebugName() override {
            return "WindowClosed";
        }
    };

    class WindowResizeEvent : public Event 
    {
    public:
        WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) { }
        ~WindowResizeEvent() { }

        inline int GetWidth() {
            return m_Width;
        }

        inline int GetHeight() {
            return m_Height;
        }

        static EventType GetStaticEventType() {
            return EventType::WindowResize;
        }

        EventType GetEventType() override {
            return EventType::WindowResize;
        }

        std::string GetDebugName() override {
            std::string s = "WindowResize: " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
            return s;
        }
    private:
        int m_Width;
        int m_Height;
    };

    class WindowPositionEvent : public Event 
    {
    public:
        WindowPositionEvent(int x, int y) : m_X(x), m_Y(y) { }
        ~WindowPositionEvent() { }

        inline int GetX() {
            return m_X;
        }

        inline int GetY() {
            return m_Y;
        }

        static EventType GetStaticEventType() {
            return EventType::WindowPosition;
        }

        EventType GetEventType() override {
            return EventType::WindowPosition;
        }

        std::string GetDebugName() override {
            std::string s = "WindowPosition: " + std::to_string(m_X) + ", " + std::to_string(m_Y);
            return s;
        }
    private:
        int m_X;
        int m_Y;
    };
}



