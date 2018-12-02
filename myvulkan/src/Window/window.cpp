#include "Handlers/InputHandler.h"
#include "Window/window.h"
#include "Instance/instance.h"


namespace window {

	void createWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		handle = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetCursorPosCallback(handle, mouseCallbackHelper);
		glfwSetKeyCallback(handle, keyCallbackHelper);
		glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glfwSetWindowUserPointer(window, this);
	}

	void createSurface() {
		if (glfwCreateWindowSurface(instance::handle, handle, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void destroy() {
		vkDestroySurfaceKHR(instance::handle, surface, nullptr);
		glfwDestroyWindow(handle);
		glfwTerminate();
	}

}

