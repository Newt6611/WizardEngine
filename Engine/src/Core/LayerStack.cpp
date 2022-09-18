#include "wzpch.h"

#include "LayerStack.h"

namespace Wizard {
    LayerStack::LayerStack()
    {

    }

    LayerStack::~LayerStack()
    {
        for (Layer* l : m_Layers) {
            l->OnLeave();
            delete l;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace_back(layer);
    }
}