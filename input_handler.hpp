#pragma once
#include "myve_window.hpp"


struct Mouse {
	double x;
	double y;

};
class InputHandler
{
public:
	InputHandler();
	static void init(GLFWwindow* window);
	
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);

	static bool isMouseKeyDown(int key);
	static bool isMouseKeyPressed(int key);

	static Mouse getMouseD();
	static GLFWwindow* window;
	static void update();

private:
	static Mouse oldMouse;
	static bool* keys;
	static bool *mouse_keys;
};