#pragma once
#include "Event.h"


namespace Wizard 
{
    class MousePositionEvent : public Event
    {
    public:
        MousePositionEvent(double x, double y) : m_X(x), m_Y(y) { }
        ~MousePositionEvent() { }

        inline int GetX() {
            return (int)m_X;
        }

        inline int GetY() {
            return (int)m_Y;
        }

        static EventType GetStaticEventType() {
            return EventType::MousePosition;
        }

        EventType GetEventType() override {
            return EventType::MousePosition;
        }

        std::string GetDebugName() override {
            std::string s = "MousePosition: " + std::to_string(m_X) + ", " + std::to_string(m_Y);
            return s;
        }

    private:
        double m_X, m_Y;
    };

    class MouseButtonClick : public Event
    {
    public:
        MouseButtonClick(int button, int action) : m_Button(button), m_Action(action) { }
        ~MouseButtonClick() { }

        static EventType GetStaticEventType() {
            return EventType::MouseButtonClick;
        }

        EventType GetEventType() override {
            return EventType::MouseButtonClick;
        }

        std::string GetDebugName() override {
            std::string s = "";
            switch (m_Button)
            {
            case 0:
                s += "Mouse Button Left ";
                break;
            case 1:
                s += "Mouse Button Right ";
                break;
            case 2:
                s += "Mouse Button Middle ";
                break;
            }

            switch (m_Action)
            {
            case 0:
                s += "Release";
                break;
            case 1:
                s += "Press";
                break;
            case 2:
                s += "Repeat";
                break;
            }
            return s;
        }

    private:
        int m_Button;
        int m_Action;
    };
}