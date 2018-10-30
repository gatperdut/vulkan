#include <cmath>
#include <algorithm>

#include "SwapchainHandler.h"
#include "Handlers.h"
#include "image_views.h"


SwapchainHandler::SwapchainHandler() {

}


SwapchainHandler::~SwapchainHandler() {
	cleanupSwapChain();
}


void SwapchainHandler::createSwapchain() {
	SwapChainSupportDetails swapChainSupport = capabilitiesHandler->querySwapChainSupport(devicesHandler->physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	extent = chooseSwapExtent(swapChainSupport.capabilities);

	format = surfaceFormat.format;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = windowHandler->surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = queuesHandler->findQueueFamilies(devicesHandler->physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(devicesHandler->device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(devicesHandler->device, swapchain, &imageCount, nullptr);
	images.resize(imageCount);
	vkGetSwapchainImagesKHR(devicesHandler->device, swapchain, &imageCount, images.data());
}

void SwapchainHandler::recreateSwapChain() {
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(windowHandler->window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(devicesHandler->device);

	cleanupSwapChain();

	createSwapchain();
	createImageViews();
	renderPassHandler->createRenderPass();
	pipelinesHandler->createGraphicsPipeline();
	renderPassHandler->createDepthResources();
	framebuffersHandler->createFramebuffers();
	commandBuffersHandler->createCommandBuffers();
}

void SwapchainHandler::cleanupSwapChain() {
	vkDestroyImageView(devicesHandler->device, renderPassHandler->depthImageView, nullptr);
	vkDestroyImage(devicesHandler->device, renderPassHandler->depthImage, nullptr);
	vkFreeMemory(devicesHandler->device, renderPassHandler->depthImageMemory, nullptr);

	for (auto framebuffer : framebuffersHandler->swapChainFramebuffers) {
		vkDestroyFramebuffer(devicesHandler->device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(devicesHandler->device, commandsHandler->commandPool, static_cast<uint32_t>(commandBuffersHandler->commandBuffers.size()), commandBuffersHandler->commandBuffers.data());

	vkDestroyPipeline(devicesHandler->device, pipelinesHandler->graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(devicesHandler->device, pipelinesHandler->pipelineLayout, nullptr);
	vkDestroyRenderPass(devicesHandler->device, renderPassHandler->renderPass, nullptr);

	for (auto imageView : swapchainHandler->imageViews) {
		vkDestroyImageView(devicesHandler->device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(devicesHandler->device, swapchainHandler->swapchain, nullptr);
}

void SwapchainHandler::createImageViews() {
	imageViews.resize(images.size());

	for (size_t i = 0; i < images.size(); i++) {
		imageViews[i] = createImageView(images[i], format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

VkSurfaceFormatKHR SwapchainHandler::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapchainHandler::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D SwapchainHandler::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}

	int width, height;
	glfwGetFramebufferSize(windowHandler->window, &width, &height);

	VkExtent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	};

	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

	return actualExtent;
}