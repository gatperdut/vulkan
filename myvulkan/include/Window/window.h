#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace window {

	const int WIDTH = 1400;
	const int HEIGHT = 1100;

	void createWindow();

	void createSurface();

	void destroy();

	extern GLFWwindow* handle;
	
	extern VkSurfaceKHR surface;

}