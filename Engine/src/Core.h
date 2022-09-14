#pragma once

#if defined(WZ_WINDOWS)
	#define PLATFORM_WIN32 1
	#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(WZ_APPLE)
	#define PLATFORM_MACOS 1
#endif

#define BIND_FN_EVENT(x) std::bind(&x, this, std::placeholders::_1)