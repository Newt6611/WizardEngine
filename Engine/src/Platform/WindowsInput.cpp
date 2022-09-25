#ifdef WZ_WINDOWS
#include "wzpch.h"
#include "Core/Window.h"
#include "Core/Input.h"

namespace Wizard {

    bool Input::IsKeyPressed(const KeyCode key)
	{
		GLFWwindow* window = (GLFWwindow*)Window::Get()->GetWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		GLFWwindow* window = (GLFWwindow*)Window::Get()->GetWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = (GLFWwindow*)Window::Get()->GetWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}
#endif