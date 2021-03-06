#include <stdexcept>
#include <glm/glm.hpp>

#include "Main.h"
#include "Handlers/Handlers.h"
#include "image_views.h"
#include "images.h"
#include "memory_types.h"
#include "buffers.h"
#include "Models/model_vertex.h"

WindowHandler* windowHandler;
InstanceHandler* instanceHandler;
DevicesHandler* devicesHandler;
QueuesHandler* queuesHandler;
CapabilitiesHandler* capabilitiesHandler;
SwapchainHandler* swapchainHandler;
RenderPassHandler* renderPassHandler;
CommandsHandler* commandsHandler;
DescriptorsHandler* descriptorsHandler;
ShadersHandler* shadersHandler;
FramebuffersHandler* framebuffersHandler;
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
		windowHandler = new WindowHandler;
		instanceHandler = new InstanceHandler;
		devicesHandler = new DevicesHandler;
		queuesHandler = new QueuesHandler;
		capabilitiesHandler = new CapabilitiesHandler;
		swapchainHandler = new SwapchainHandler;
		renderPassHandler = new RenderPassHandler;
		commandsHandler = new CommandsHandler;
		descriptorsHandler = new DescriptorsHandler;
		shadersHandler = new ShadersHandler;
		framebuffersHandler = new FramebuffersHandler;
		modelsHandler = new ModelsHandler;
		lightsHandler = new LightsHandler;
		commandBuffersHandler = new CommandBuffersHandler;
		synchrosHandler = new SynchrosHandler;
		drawHandler = new DrawHandler;
		inputHandler = new InputHandler;
		cameraHandler = new CameraHandler;

		windowHandler->createWindow();
		instanceHandler->createInstance();
		instanceHandler->setupDebugCallback();
		windowHandler->createSurface();
		devicesHandler->pickPhysicalDevice();
		devicesHandler->createLogicalDevice();
		devicesHandler->computeUboAlignment();
		swapchainHandler->createSwapchain();
		swapchainHandler->createImageViews();
		renderPassHandler->createRenderPass();
		commandsHandler->createCommandPool();
		renderPassHandler->createDepthResources();
		framebuffersHandler->createFramebuffers();
		lightsHandler->add(glm::vec3(2.0f, 3.0f, -12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightsHandler->add(glm::vec3(0.0f, 4.0f, -5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightsHandler->createDescriptorSetLayoutData();
		lightsHandler->createDescriptorSetLayoutModel();
		lightsHandler->createDataUBOs();
		lightsHandler->createModelUBOs();
		modelsHandler->load("models/wonderwoman/", "wonderwoman.obj", glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelsHandler->load("models/table/", "table.obj", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.9f, 0.9f, 0.9f));
		modelsHandler->load("models/Aragorn/", "Aragorn.obj", glm::vec3(1.0f, 2.0f, 0.0f), glm::vec3(0.012f, 0.012f, 0.012f));
		modelsHandler->createDescriptorSetLayouts();
		modelsHandler->createUBOs();
		descriptorsHandler->createDescriptorPool();
		lightsHandler->createDescriptorSetsData();
		lightsHandler->createDescriptorSetsModel();
		modelsHandler->createDescriptorSets();
		lightsHandler->createPipeline();
		modelsHandler->createPipelines();
		commandBuffersHandler->createCommandBuffers();
		synchrosHandler->createSyncObjects();

		mainLoop();
		cleanup();
	}

private:
	void mainLoop() {
		while (!glfwWindowShouldClose(windowHandler->window)) {
			inputHandler->processMovement();
			glfwPollEvents();
			drawHandler->drawFrame();
		}

		vkDeviceWaitIdle(devicesHandler->device);
	}


	void cleanup() {
		delete lightsHandler;
		delete modelsHandler;
		delete swapchainHandler;
		delete descriptorsHandler;
		delete synchrosHandler;
		delete commandsHandler;
		delete shadersHandler;
		delete renderPassHandler;
		delete framebuffersHandler;
		delete capabilitiesHandler;
		delete queuesHandler;
		delete devicesHandler;
		delete windowHandler;
		delete instanceHandler;
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