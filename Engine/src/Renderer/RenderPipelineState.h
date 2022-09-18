#pragma once
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsEngine/interface/PipelineState.h"

namespace Wizard {
	using namespace Diligent;

	class RenderPipelineState
	{
	public:
		RenderPipelineState(const std::string& name);
		~RenderPipelineState();

		void Generate();

		inline IPipelineState* GetPipelineState() { return m_PipelineState; }

	public:
		GraphicsPipelineStateCreateInfo createInfo;
		
	protected:
		bool m_Generated = false;
		RefCntAutoPtr<IPipelineState> m_PipelineState;
	};
}