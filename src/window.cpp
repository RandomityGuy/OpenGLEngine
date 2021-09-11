#include "window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <unordered_map>

std::unordered_map<GLFWwindow*, Window*> windows;

Window::Window()
{
	this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
	}
	windows.insert(std::make_pair(this->window, this));
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	windows.erase(this->window);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Window* wnd = windows.at(window);
	glViewport(0, 0, width, height);
	wnd->width = width;
	wnd->height = height;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* wnd = windows.at(window);
	for (auto& fn : wnd->keyEventHandlers)
	{
		fn(wnd, key, scancode, action, mods);
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* wnd = windows.at(window);
	for (auto& fn : wnd->mouseMoveEventHandlers)
	{
		fn(wnd, xpos, ypos);
	}
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* wnd = windows.at(window);
	for (auto& fn : wnd->mouseButtonEventHandlers)
	{
		fn(wnd, button, action, mods);
	}
}

static void cursor_enter_callback(GLFWwindow* window, int entered)
{
	Window* wnd = windows.at(window);
	for (auto& fn : wnd->mouseEnterEventHandlers)
	{
		fn(wnd, entered);
	}
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* wnd = windows.at(window);
	for (auto& fn : wnd->mouseScrollEventHandlers)
	{
		fn(wnd, yoffset);
	}
}

void Window::activate()
{
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Window::setTitle(std::string title)
{
	this->title = title;
	glfwSetWindowTitle(this->window, title.c_str());
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Window::captureMouse(bool capture)
{
	if (capture)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::setCursorPosition(double x, double y)
{
	glfwSetCursorPos(window, x, y);
}