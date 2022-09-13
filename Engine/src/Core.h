#pragma once

#if defined(WZ_WINDOWS)
	#define PLATFORM_WIN32 1
	#include "DiligentCore/Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
	#include "DiligentCore/Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
	#include "DiligentCore/Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
#elif defined(WZ_APPLE)
	#define PLATFORM_MACOS 1
	#include "DiligentCore/Graphics/GraphicsEngineMetal/interface/EngineFactoryMtl.h"
	#include "DiligentCore/Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"
#endif

#ifdef WZ_APPLE

#endif


#define BIND_FN_EVENT(x) std::bind(&x, this, std::placeholders::_1)