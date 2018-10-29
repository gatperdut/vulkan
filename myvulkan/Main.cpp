#include "Handlers.h"
#include "image_views.h"
#include "images.h"
#include "memory_types.h"
#include "buffers.h"
#include "vertex.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
const std::string MODEL_PATH = "models/chalet.obj";


#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <set>

#include <array>

#include <cmath>


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>

const int MAX_FRAMES_IN_FLIGHT = 2;

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



std::vector<Vertex> vertices;

std::vector<uint32_t> indices;

class HelloTriangleApplication {

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
		createFramebuffers();
		texturesHandler->createTextureImage();
		texturesHandler->createTextureImageView();
		texturesHandler->createTextureSampler();
		loadModel();
		createVertexAndIndexBuffer();
		uniformsHandler->createUniformBuffer();
		descriptorsHandler->createDescriptorPool();
		descriptorsHandler->createDescriptorSets();
		createCommandBuffers();
		createSyncObjects();

		mainLoop();
		cleanup();
	}

private:

	std::vector<VkFramebuffer> swapChainFramebuffers;
	
	VkBuffer vertexAndIndexBuffer;
	VkDeviceMemory vertexAndIndexBufferMemory;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;

	void mainLoop() {
		while (!glfwWindowShouldClose(windowHandler->window)) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(devicesHandler->device);
	}

	void cleanupSwapChain() {
		vkDestroyImageView(devicesHandler->device, renderPassHandler->depthImageView, nullptr);
		vkDestroyImage(devicesHandler->device, renderPassHandler->depthImage, nullptr);
		vkFreeMemory(devicesHandler->device, renderPassHandler->depthImageMemory, nullptr);

		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(devicesHandler->device, framebuffer, nullptr);
		}

		vkFreeCommandBuffers(devicesHandler->device, commandsHandler->commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		vkDestroyPipeline(devicesHandler->device, pipelinesHandler->graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(devicesHandler->device, pipelinesHandler->pipelineLayout, nullptr);
		vkDestroyRenderPass(devicesHandler->device, renderPassHandler->renderPass, nullptr);

		for (auto imageView : swapchainHandler->imageViews) {
			vkDestroyImageView(devicesHandler->device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(devicesHandler->device, swapchainHandler->swapchain, nullptr);
	}

	void cleanup() {
		cleanupSwapChain();

		vkDestroySampler(devicesHandler->device, texturesHandler->textureSampler, nullptr);
		vkDestroyImageView(devicesHandler->device, texturesHandler->textureImageView, nullptr);
		vkDestroyImage(devicesHandler->device, texturesHandler->textureImage, nullptr);
		vkFreeMemory(devicesHandler->device, texturesHandler->textureImageMemory, nullptr);

		vkDestroyDescriptorPool(devicesHandler->device, descriptorsHandler->descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorsHandler->descriptorSetLayout, nullptr);

		for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
			vkDestroyBuffer(devicesHandler->device, uniformsHandler->uniformBuffers[i], nullptr);
			vkFreeMemory(devicesHandler->device, uniformsHandler->uniformBuffersMemory[i], nullptr);
		}

		vkDestroyBuffer(devicesHandler->device, vertexAndIndexBuffer, nullptr);
		vkFreeMemory(devicesHandler->device, vertexAndIndexBufferMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(devicesHandler->device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(devicesHandler->device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(devicesHandler->device, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(devicesHandler->device, commandsHandler->commandPool, nullptr);

		delete capabilitiesHandler;
		delete queuesHandler;
		delete devicesHandler;
		//vkDestroyDevice(device, nullptr);
		delete windowHandler;
		//vkDestroySurfaceKHR(instanceHandler->instance, windowHandler->surface, nullptr);
		//glfwDestroyWindow(window);
		//glfwTerminate();
		delete instanceHandler;
		//if (enableValidationLayers) {
		//	DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
		//}
		//vkDestroyInstance(instance, nullptr);
	}


	void recreateSwapChain() {
		int width = 0, height = 0;
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(windowHandler->window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(devicesHandler->device);

		cleanupSwapChain();

		swapchainHandler->createSwapchain();
		swapchainHandler->createImageViews();
		renderPassHandler->createRenderPass();
		pipelinesHandler->createGraphicsPipeline();
		renderPassHandler->createDepthResources();
		createFramebuffers();
		createCommandBuffers();
	}


	void createFramebuffers() {
		swapChainFramebuffers.resize(swapchainHandler->imageViews.size());

		for (size_t i = 0; i < swapchainHandler->imageViews.size(); i++) {
			std::array<VkImageView, 2> attachments = {
				swapchainHandler->imageViews[i],
				renderPassHandler->depthImageView
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPassHandler->renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapchainHandler->extent.width;
			framebufferInfo.height = swapchainHandler->extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(devicesHandler->device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void loadModel() {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str())) {
			throw std::runtime_error(err);
		}

		std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex = {};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void createVertexAndIndexBuffer() {
		VkDeviceSize verticesSize = sizeof(vertices[0]) * vertices.size();
		VkDeviceSize indicesSize = sizeof(indices[0]) * indices.size();

		VkDeviceSize bufferSize = verticesSize + indicesSize;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(devicesHandler->device, stagingBufferMemory, 0, verticesSize, 0, &data);
			memcpy(data, vertices.data(), (size_t) verticesSize);
		vkUnmapMemory(devicesHandler->device, stagingBufferMemory);

		vkMapMemory(devicesHandler->device, stagingBufferMemory, verticesSize, indicesSize, 0, &data);
			memcpy(data, indices.data(), (size_t) indicesSize);
		vkUnmapMemory(devicesHandler->device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexAndIndexBuffer, vertexAndIndexBufferMemory);
		copyBuffer(stagingBuffer, vertexAndIndexBuffer, bufferSize);

		vkDestroyBuffer(devicesHandler->device, stagingBuffer, nullptr);
		vkFreeMemory(devicesHandler->device, stagingBufferMemory, nullptr);
	}

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = commandsHandler->beginSingleTimeCommands();
			VkBufferCopy copyRegion = {};
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		commandsHandler->endSingleTimeCommands(commandBuffer);
	}

	void createCommandBuffers() {
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandsHandler->commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(devicesHandler->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffers!");
			}

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPassHandler->renderPass;
				renderPassInfo.framebuffer = swapChainFramebuffers[i];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = swapchainHandler->extent;

				std::array<VkClearValue, 2> clearValues = {};
				clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
				clearValues[1].depthStencil = { 1.0f, 0 };
				renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();

				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
					vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinesHandler->graphicsPipeline);

					VkBuffer vertexBuffers[] = { vertexAndIndexBuffer };
					VkDeviceSize verticesOffsets[] = { 0 };
					vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, verticesOffsets);

					VkDeviceSize indicesOffset = sizeof(vertices[0]) * vertices.size();
					vkCmdBindIndexBuffer(commandBuffers[i], vertexAndIndexBuffer, indicesOffset, VK_INDEX_TYPE_UINT32);

					vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinesHandler->pipelineLayout, 0, 1, &descriptorsHandler->descriptorSets[i], 0, nullptr);

					//vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
					vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
				vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}

	void drawFrame() {
		vkWaitForFences(devicesHandler->device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(devicesHandler->device, swapchainHandler->swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
		
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		uniformsHandler->updateUniformBuffer(imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(devicesHandler->device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(queuesHandler->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { swapchainHandler->swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;
	
		result = vkQueuePresentKHR(queuesHandler->presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowHandler->framebufferResized) {
			windowHandler->framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void createSyncObjects() {
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo  semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(devicesHandler->device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(devicesHandler->device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(devicesHandler->device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create synchronization objects for a frame!");
			}
		}
	}

	





};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}