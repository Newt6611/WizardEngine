#pragma once

#include <string>
#include "Events/Event.h"

namespace Wizard {
    class Layer 
    {
    public:
        Layer(const std::string& name = "Defalut Layer") : m_DebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnEnter() {}
        virtual void OnLeave() {}

        virtual void OnUpdate(float ts) {}
        virtual void OnEvent(Event& e) {}

    protected:
        std::string m_DebugName;
    };
}