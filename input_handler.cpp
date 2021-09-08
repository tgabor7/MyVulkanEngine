#include "input_handler.hpp"
#include <cassert>

GLFWwindow* InputHandler::window = nullptr;
Mouse InputHandler::oldMouse = {};
bool* InputHandler::keys = new bool[GLFW_KEY_LAST];
bool* InputHandler::mouse_keys = new bool[GLFW_KEY_LAST];

bool InputHandler::isKeyPressed(int key)
{
	assert(window && "Window was nullptr");

	return (glfwGetKey(window, key) == GLFW_PRESS) && !keys[key];
}
bool InputHandler::isKeyDown(int key)
{
	assert(window && "Window was nullptr");

	return (glfwGetKey(window, key) == GLFW_PRESS);
}
bool InputHandler::isMouseKeyPressed(int key)
{
	assert(window && "Window was nullptr");

	return (glfwGetMouseButton(window, key) == GLFW_PRESS) && !mouse_keys[key];
}
bool InputHandler::isMouseKeyDown(int key)
{
	assert(window && "Window was nullptr");

	return (glfwGetMouseButton(window, key) == GLFW_PRESS);
}
void InputHandler::update()
{
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		keys[i] = isKeyDown(i);
	}
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		mouse_keys[i] = isMouseKeyDown(i);
	}
}
Mouse InputHandler::getMouseD()
{
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);

	Mouse currentMouse = { x - oldMouse.x, y - oldMouse.y };

	oldMouse = { x, y };

	return currentMouse;
}
void InputHandler::init(GLFWwindow *_window)
{
	window = _window;
}

InputHandler::InputHandler()
{

}