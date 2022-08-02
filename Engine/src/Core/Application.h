#pragma once
#include "wzpch.h"
#include "Log.h"

namespace Wizard {
    class Application 
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();
    
    protected:
        void OnUpdate();


        
    };
}
