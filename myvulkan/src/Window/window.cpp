#include "Handlers/InputHandler.h"
#include "Window/window.h"
#include "Instance/instance.h"


namespace window {

	void createWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetCursorPosCallback(window, mouseCallbackHelper);
		glfwSetKeyCallback(window, keyCallbackHelper);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glfwSetWindowUserPointer(window, this);
	}

	void createSurface() {
		if (glfwCreateWindowSurface(instance::instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void destroy() {
		vkDestroySurfaceKHR(instance::instance, surface, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}

}

