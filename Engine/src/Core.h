#pragma once

#define PLATFORM_WIN32 1

#define BIND_FN_EVENT(x) std::bind(&x, this, std::placeholders::_1)