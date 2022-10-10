#pragma once

#include "Core.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Events/MouseEvents.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include "Scene/ScriptingEntity.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RenderTool.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/OrthographicCameraController.h"
#include "Renderer/Buffer.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderPipelineState.h"

using namespace Diligent;

// Real Imgui
#include "glm/gtc/type_ptr.hpp"
#include "DiligentTools/ThirdParty/imgui/imgui.h"
#include "DiligentTools/ThirdParty/imgui/backends/imgui_impl_glfw.h"
#include "DiligentTools/ThirdParty/imgui/backends/imgui_impl_dx12.h"

#include "DiligentTools/Imgui/interface/ImGuiDiligentRenderer.hpp"
#include "DiligentTools/Imgui/interface/ImGuiImplDiligent.hpp"

