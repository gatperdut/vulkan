#pragma once


const int WIDTH = 800;
const int HEIGHT = 600;


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class WindowHandler {
public:
	WindowHandler();
	~WindowHandler();

	void createWindow();
	void createSurface();


	GLFWwindow* window;
	VkSurfaceKHR surface;
	bool framebufferResized = false;

private:
	static void framebufferResizeCallback(GLFWwindow*, int, int);
};
