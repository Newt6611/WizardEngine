#pragma once

#include "spdlog/spdlog.h"


namespace Wizard {
    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() {
            return s_EngineLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
            return s_ClientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Engine Side
#define WZ_ENGINE_INFO(...)    ::Wizard::Log::GetEngineLogger()->info(__VA_ARGS__)
#define WZ_ENGINE_TRACE(...)   ::Wizard::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define WZ_ENGINE_WARN(...)    ::Wizard::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define WZ_ENGINE_ERROR(...)   ::Wizard::Log::GetEngineLogger()->error(__VA_ARGS__)

// Client Side
#define WZ_INFO(...)           ::Wizard::Log::GetClientLogger()->info(__VA_ARGS__)
#define WZ_TRACE(...)          ::Wizard::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WZ_WARN(...)           ::Wizard::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WZ_ERROR(...)          ::Wizard::Log::GetClientLogger()->error(__VA_ARGS__)