#include "Wizard.h"
#include "EntryPoint.h"
#include "EditorLayer.h"

class WZEditor : public Wizard::Application
{
public:
    WZEditor() 
    {
        PushLayer(new EditorLayer);
    }

    ~WZEditor() 
    {

    }
};

Wizard::Application* CreateApplication()
{
    return new WZEditor;
}