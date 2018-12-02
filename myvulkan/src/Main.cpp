#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>

#include "Main.h"
#include "Handlers/Handlers.h"
#include "Window/window.h"
#include "Instance/instance.h"
#include "Instance/debug.h"
#include "Devices/physical.h"
#include "Devices/logical.h"
#include "image_views.h"
#include "images.h"
#include "memory_types.h"
#include "buffers.h"

GLFWwindow* window::window;
VkSurfaceKHR window::surface;

VkInstance instance::instance;
VkDebugUtilsMessengerEXT instance::debug::callback;

VkPhysicalDevice devices::physical::dev;
VkDevice devices::logical::dev;


QueuesHandler* queuesHandler;
CapabilitiesHandler* capabilitiesHandler;
Presentation* presentation;
RenderPassHandler* renderPassHandler;
CommandsHandler* commandsHandler;
DescriptorsHandler* descriptorsHandler;
ShadersHandler* shadersHandler;
ModelsHandler* modelsHandler;
LightsHandler* lightsHandler;
CommandBuffersHandler* commandBuffersHandler;
SynchrosHandler* synchrosHandler;
DrawHandler* drawHandler;
InputHandler* inputHandler;
CameraHandler* cameraHandler;


class MyVulkan {

public:
	void run() {
		queuesHandler = new QueuesHandler;
		capabilitiesHandler = new CapabilitiesHandler;
		presentation = new Presentation;
		renderPassHandler = new RenderPassHandler;
		commandsHandler = new CommandsHandler;
		descriptorsHandler = new DescriptorsHandler;
		shadersHandler = new ShadersHandler;
		modelsHandler = new ModelsHandler;
		lightsHandler = new LightsHandler;
		commandBuffersHandler = new CommandBuffersHandler;
		synchrosHandler = new SynchrosHandler;
		drawHandler = new DrawHandler;
		inputHandler = new InputHandler;
		cameraHandler = new CameraHandler;

		window::createWindow();
		instance::create();
		instance::debug::setupDebugCallback();
		window::createSurface();
		devices::physical::pick();
		devices::logical::create();
		presentation->createSwapchain();
		presentation->createImageViews();
		renderPassHandler->createRenderPassRegular();
		renderPassHandler->createRenderPassShadow();
		commandsHandler->createCommandPool();
		presentation->createDepthResources();
		presentation->createFramebuffersRegular();
		lightsHandler->add(glm::vec3(13.0f, 3.0f, 10.0f), glm::vec3(0.6f, 0.6f, 0.6f));
		lightsHandler->add(glm::vec3(13.0f, 3.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		lightsHandler->add(glm::vec3(13.0f, 3.0f, 5.0f), glm::vec3(0.3f, 0.0f, 0.0f));
		presentation->createFramebuffersShadow();
		//lightsHandler->add(glm::vec3(0.0f, 4.0f, -5.0f), glm::vec3(0.5f, 0.0f, 0.0f));
		lightsHandler->createDSLs();
		lightsHandler->createUs();
		//modelsHandler->load("models/wonderwoman/", "wonderwoman.obj", glm::vec3(-1.0f, 1.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelsHandler->load("models/cube/", "untitled.obj", glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.9f, 80.0f, 80.0f));
		//modelsHandler->load("models/Aragorn/", "Aragorn.obj", glm::vec3(1.0f, 2.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelsHandler->load("models/cube/", "untitled.obj", glm::vec3(2.5f, 4.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelsHandler->load("models/cube/", "untitled.obj", glm::vec3(2.5f, 0.0f, 6.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelsHandler->load("models/cube/", "untitled.obj", glm::vec3(2.5f, -4.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//modelsHandler->load("models/torus/", "torus.obj", glm::vec3(12.5f, -4.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//modelsHandler->load("models/cube/", "untitled.obj", glm::vec3(1.5f, 6.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelsHandler->createDSLs();
		modelsHandler->createUs();
		descriptorsHandler->createDescriptorPool();
		lightsHandler->createDSs();
		modelsHandler->createDSs();
		lightsHandler->createPipelines();
		modelsHandler->createPipelines();
		commandBuffersHandler->createCommandBuffersShadow();
		commandBuffersHandler->createCommandBuffersRegular();
		synchrosHandler->createSyncObjects();

		mainLoop();
		cleanup();
	}

private:
	void mainLoop() {
		while (!glfwWindowShouldClose(window::window)) {
			inputHandler->processMovement();
			glfwPollEvents();
			drawHandler->drawFrame();
		}

		vkDeviceWaitIdle(devices::logical::dev);
	}


	void cleanup() {
		delete lightsHandler;
		delete modelsHandler;
		delete presentation;
		delete descriptorsHandler;
		delete synchrosHandler;
		delete commandsHandler;
		delete shadersHandler;
		delete renderPassHandler;
		delete capabilitiesHandler;
		delete queuesHandler;
		devices::logical::destroy();
		window::destroy();
		instance::destroy();
		delete drawHandler;
		delete inputHandler;
		delete cameraHandler;
	}
};


int main() {
	MyVulkan app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}