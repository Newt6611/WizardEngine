#pragma once
#include "Wizard.h"

using namespace Wizard;

class ComponentPanel
{
public:
    ComponentPanel();
    ~ComponentPanel();

    void DrawComponent(Scene& scene, Entity& entity);

private:
    void DrawAddComponent(Scene& scene, Entity& entity);
};