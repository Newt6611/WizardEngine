#pragma once

namespace Wizard {
    class ScriptingEntity
    {
    public:
        ScriptingEntity() = default;
        virtual ~ScriptingEntity() {}

        virtual void OnCreate() {}
        virtual void OnUpdate(float ts) {}
        virtual void OnShutdown() {}
    };
}