#pragma once

#include <functional>

namespace Wizard
{
    enum class EventType {
        None = 0,
        WindowResize, WindowClose, WindowPosition,
        MousePosition, MouseButtonClick, MouseScrolled
    };

    class Event
    {
    public:
        virtual EventType GetEventType() = 0;
        virtual std::string GetDebugName() = 0;

    public:
        bool Handled = false;
    };

    class EventDispatcher 
    {
    public:
        EventDispatcher(Event& e) 
            : m_Event(e)
        { }

        template<typename T>
        bool Dispatch(std::function<bool(T&)> fun) {
            if (m_Event.GetEventType() == T::GetStaticEventType()) {
                m_Event.Handled = fun(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
}
