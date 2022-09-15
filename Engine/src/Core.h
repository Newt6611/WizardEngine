#pragma once

#define BIND_FN_EVENT(x) std::bind(&x, this, std::placeholders::_1)