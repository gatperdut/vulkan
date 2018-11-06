#include <stdexcept>
#include <glm/glm.hpp>

#include "Handlers/Handlers.h"
#include "image_views.h"
#include "images.h"
#include "memory_types.h"
#include "buffers.h"
#include "vertex.h"

WindowHandler* windowHandler;
InstanceHandler* instanceHandler;
DevicesHandler* devicesHandler;
QueuesHandler* queuesHandler;
CapabilitiesHandler* capabilitiesHandler;
SwapchainHandler* swapchainHandler;
RenderPassHandler* renderPassHandler;
CommandsHandler* commandsHandler;
DescriptorsHandler* descriptorsHandler;
UniformsHandler* uniformsHandler;
PipelinesHandler* pipelinesHandler;
ShadersHandler* shadersHandler;
FramebuffersHandler* framebuffersHandler;
BuffersHandler* buffersHandler;
ModelsHandler* modelsHandler;
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
		uniformsHandler = new UniformsHandler;
		pipelinesHandler = new PipelinesHandler;
		shadersHandler = new ShadersHandler;
		framebuffersHandler = new FramebuffersHandler;
		buffersHandler = new BuffersHandler;
		modelsHandler = new ModelsHandler;
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
		uniformsHandler->computeAlignment();
		swapchainHandler->createSwapchain();
		swapchainHandler->createImageViews();
		renderPassHandler->createRenderPass();
		descriptorsHandler->createDescriptorSetLayout();
		pipelinesHandler->createGraphicsPipeline();
		commandsHandler->createCommandPool();
		renderPassHandler->createDepthResources();
		framebuffersHandler->createFramebuffers();
		modelsHandler->load("models/pharaoh.obj", "models/", "textures/pharaoh.jpeg", glm::vec3(-4.0f, -20.0f, -20.0f));
		//modelsHandler->load("models/angel.obj", "textures/angel.jpeg", glm::vec3(4.0f, 0.0f, 0.0f));
		//modelsHandler->load("models/house.obj", "textures/house.png", glm::vec3(-4.0f, 0.0f, 0.0f));
		buffersHandler->createVertexAndIndexBuffer();
		uniformsHandler->createUniformBuffer();
		descriptorsHandler->createDescriptorPool();
		descriptorsHandler->createDescriptorSets();
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
		delete uniformsHandler;
		delete swapchainHandler;
		delete descriptorsHandler;
		delete buffersHandler;
		delete synchrosHandler;
		delete commandsHandler;
		delete pipelinesHandler;
		delete shadersHandler;
		delete renderPassHandler;
		delete framebuffersHandler;
		delete modelsHandler;
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