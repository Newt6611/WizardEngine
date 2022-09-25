#include "wzpch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Wizard
{
    std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_EngineLogger = spdlog::stdout_color_mt("WIZARD");
        s_EngineLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
        s_ClientLogger->set_level(spdlog::level::trace);
    }    
}
