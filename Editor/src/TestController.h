#include "Wizard.h"
using namespace Wizard;

class TestController : public ScriptingEntity
    {
    public:
        TestController()
        {

        }

        virtual ~TestController()
        {
            WZ_TRACE("bye");
        }

        virtual void OnCreate() override
        {
            WZ_TRACE("On Create");
        }

        virtual void OnUpdate(float ts) override
        {
            WZ_TRACE("On Update");
        }

        virtual void OnShutdown() override
        {

        }
    };