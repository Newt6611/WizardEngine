#pragma once

#if defined(WZ_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32 1
#elif defined(WZ_APPLE)
    #define GLFW_EXPOSE_NATIVE_NSGL 1
    #ifdef PLATFORM_WIN32
        #undef PLATFORM_WIN32
    #endif
#elif defined(WZ_LINUX)
    #define GLFW_EXPOSE_NATIVE_X11 1
#endif


#define BIND_FN_EVENT(x) std::bind(&x, this, std::placeholders::_1)