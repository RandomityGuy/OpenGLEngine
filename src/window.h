#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

class Window
{

	GLFWwindow* window;

public:
	Window();
	~Window();

	int width = 800;
	int height = 600;
	std::string title = std::string("OpenGL");

	std::vector<std::function<void(Window*, int, int, int, int)>> keyEventHandlers;
	std::vector<std::function<void(Window*, double, double)>> mouseMoveEventHandlers;
	std::vector<std::function<void(Window*, int, int, int)>> mouseButtonEventHandlers;
	std::vector<std::function<void(Window*, int)>> mouseEnterEventHandlers;
	std::vector<std::function<void(Window*, int)>> mouseScrollEventHandlers;

	void activate();
	void setTitle(std::string title);

	bool shouldClose();
	void swapBuffers();
	virtual void processInput();
	void captureMouse(bool capture);
	void setCursorPosition(double x, double y);
};