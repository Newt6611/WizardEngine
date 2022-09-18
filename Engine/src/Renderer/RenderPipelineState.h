#pragma once

#include "Renderer.h"

namespace Wizard {
	class RenderPipelineState
	{
	public:
		RenderPipelineState();
		~RenderPipelineState();
		virtual void Init(const char* state_name);

		inline const IPipelineState* GetPipelineState() { return m_PipelineState; }

	protected:
		RefCntAutoPtr<IPipelineState> m_PipelineState;
		ISwapChain* swapchain;
	};
}