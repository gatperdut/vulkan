#include "Handlers/Handlers.h"
#include "Instance/instance.h"


WindowHandler::WindowHandler() {

}


WindowHandler::~WindowHandler() {
	vkDestroySurfaceKHR(instance::instance, surface, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}


void WindowHandler::createWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetCursorPosCallback(window, mouseCallbackHelper);
	glfwSetKeyCallback(window, keyCallbackHelper);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}


void WindowHandler::createSurface() {
	if (glfwCreateWindowSurface(instance::instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}


void WindowHandler::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto myself = reinterpret_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	myself->framebufferResized = true;
}
