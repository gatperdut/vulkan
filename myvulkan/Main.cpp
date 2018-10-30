#include <stdexcept>

#include "Handlers.h"
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
TexturesHandler* texturesHandler;
PipelinesHandler* pipelinesHandler;
ShadersHandler* shadersHandler;
FramebuffersHandler* framebuffersHandler;
BuffersHandler* buffersHandler;
ModelsHandler* modelsHandler;
CommandBuffersHandler* commandBuffersHandler;
SynchrosHandler* synchrosHandler;
DrawHandler* drawHandler;


class MyVulkan {

public:
	void run() {
		windowHandler = new WindowHandler();
		instanceHandler = new InstanceHandler();
		devicesHandler = new DevicesHandler();
		queuesHandler = new QueuesHandler();
		capabilitiesHandler = new CapabilitiesHandler();
		swapchainHandler = new SwapchainHandler();
		renderPassHandler = new RenderPassHandler();
		commandsHandler = new CommandsHandler();
		descriptorsHandler = new DescriptorsHandler();
		uniformsHandler = new UniformsHandler();
		texturesHandler = new TexturesHandler();
		pipelinesHandler = new PipelinesHandler();
		shadersHandler = new ShadersHandler();
		framebuffersHandler = new FramebuffersHandler();
		buffersHandler = new BuffersHandler();
		modelsHandler = new ModelsHandler();
		commandBuffersHandler = new CommandBuffersHandler();
		synchrosHandler = new SynchrosHandler();
		drawHandler = new DrawHandler;

		windowHandler->createWindow();
		instanceHandler->createInstance();
		instanceHandler->setupDebugCallback();
		windowHandler->createSurface();
		devicesHandler->pickPhysicalDevice();
		devicesHandler->createLogicalDevice();
		swapchainHandler->createSwapchain();
		swapchainHandler->createImageViews();
		renderPassHandler->createRenderPass();
		descriptorsHandler->createDescriptorSetLayout();
		pipelinesHandler->createGraphicsPipeline();
		commandsHandler->createCommandPool();
		renderPassHandler->createDepthResources();
		framebuffersHandler->createFramebuffers();
		texturesHandler->createTextureImage();
		texturesHandler->createTextureImageView();
		texturesHandler->createTextureSampler();
		modelsHandler->loadModel();
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
			glfwPollEvents();
			drawHandler->drawFrame();
		}

		vkDeviceWaitIdle(devicesHandler->device);
	}


	void cleanup() {
		delete swapchainHandler;
		delete texturesHandler;
		delete descriptorsHandler;
		delete uniformsHandler;
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