#include <cmath>
#include <algorithm>

#include "Presentation/Presentation.h"
#include "Devices/logical.h"
#include "Devices/physical.h"
#include "Handlers/Handlers.h"
#include "images.h"
#include "image_views.h"
#include "swapchain.h"
#include "memory_types.h"


Presentation::Presentation() {

}


Presentation::~Presentation() {
	cleanupSwapChain();
}


void Presentation::createSwapchain() {
	SwapChainSupportDetails swapChainSupport = capabilitiesHandler->querySwapChainSupport(devices::physical::dev);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	swapchain.extent = chooseSwapExtent(windowHandler->window, swapChainSupport.capabilities);

	swapchain.format = surfaceFormat.format;

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
	createInfo.imageExtent = swapchain.extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = queuesHandler->findQueueFamilies(devices::physical::dev);
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

	if (vkCreateSwapchainKHR(devices::logical::dev, &createInfo, nullptr, &swapchain.handle) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(devices::logical::dev, swapchain.handle, &imageCount, nullptr);
	swapchain.images.resize(imageCount);
	vkGetSwapchainImagesKHR(devices::logical::dev, swapchain.handle, &imageCount, swapchain.images.data());
}

void Presentation::cleanupSwapChain() {
	vkDestroyImageView(devices::logical::dev, depth.imageView, nullptr);
	vkDestroyImage(devices::logical::dev, depth.image, nullptr);
	vkFreeMemory(devices::logical::dev, depth.imageMemory, nullptr);

	for (auto framebuffer : swapchain.framebuffers) {
		vkDestroyFramebuffer(devices::logical::dev, framebuffer, nullptr);
	}
	for (auto imageView : swapchain.imageViews) {
		vkDestroyImageView(devices::logical::dev, imageView, nullptr);
	}
	vkDestroySwapchainKHR(devices::logical::dev, swapchain.handle, nullptr);

	for (size_t i = 0; i < shadow.framebuffers.size(); i++) {
		vkDestroyFramebuffer(devices::logical::dev, shadow.framebuffers[i], nullptr);
		vkDestroyImageView(devices::logical::dev, shadow.imageViews[i], nullptr);
		vkDestroyImage(devices::logical::dev, shadow.images[i], nullptr);
		vkFreeMemory(devices::logical::dev, shadow.imageMemories[i], nullptr);
		vkDestroySampler(devices::logical::dev, shadow.samplers[i], nullptr);
	}

	modelsHandler->destroyPipelines();
}

void Presentation::createImageViews() {
	swapchain.imageViews.resize(swapchain.images.size());

	for (size_t i = 0; i < swapchain.images.size(); i++) {
		swapchain.imageViews[i] = createImageView(swapchain.images[i], swapchain.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}


void Presentation::createFramebuffersRegular() {
	swapchain.framebuffers.resize(swapchain.imageViews.size());

	for (size_t i = 0; i < swapchain.imageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			swapchain.imageViews[i],
			depth.imageView
		};

		VkFramebufferCreateInfo framebufferInfo = {};framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPassHandler->renderPassRegular;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchain.extent.width;
		framebufferInfo.height = swapchain.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(devices::logical::dev, &framebufferInfo, nullptr, &swapchain.framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create regular framebuffer!");
		}
	}
}


void Presentation::createFramebuffersShadow() {
	size_t numLights = lightsHandler->lights.size();

	shadow.images.resize(numLights);
	shadow.imageViews.resize(numLights);
	shadow.imageMemories.resize(numLights);
	shadow.samplers.resize(numLights);
	shadow.framebuffers.resize(numLights);

	for (size_t i = 0; i < numLights; i++) {
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = 1024;
		imageCreateInfo.extent.height = 1024;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.format = VK_FORMAT_D32_SFLOAT;
		imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		if (vkCreateImage(devices::logical::dev, &imageCreateInfo, nullptr, &shadow.images[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shadow image!");
		}

		VkMemoryAllocateInfo memoryAllocInfo = {};
		memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;;
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(devices::logical::dev, shadow.images[i], &memReqs);
		memoryAllocInfo.allocationSize = memReqs.size;
		memoryAllocInfo.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		if (vkAllocateMemory(devices::logical::dev, &memoryAllocInfo, nullptr, &shadow.imageMemories[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate memory for shadow map!");
		}
		if (vkBindImageMemory(devices::logical::dev, shadow.images[i], shadow.imageMemories[i], 0) != VK_SUCCESS) {
			throw std::runtime_error("failed to bind shadow map!");
		}

		VkImageViewCreateInfo depthStencilView = {};
		depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		depthStencilView.format = VK_FORMAT_D32_SFLOAT;
		depthStencilView.subresourceRange = {};
		depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthStencilView.subresourceRange.baseMipLevel = 0;
		depthStencilView.subresourceRange.levelCount = 1;
		depthStencilView.subresourceRange.baseArrayLayer = 0;
		depthStencilView.subresourceRange.layerCount = 1;
		depthStencilView.image = shadow.images[i];
		if (vkCreateImageView(devices::logical::dev, &depthStencilView, nullptr, &shadow.imageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shadow image view!");
		}

		VkSamplerCreateInfo sampler = {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.magFilter = VK_FILTER_NEAREST;
		sampler.minFilter = VK_FILTER_NEAREST;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		sampler.addressModeV = sampler.addressModeU;
		sampler.addressModeW = sampler.addressModeU;
		sampler.mipLodBias = 0.0f;
		sampler.maxAnisotropy = 1.0f;
		sampler.minLod = 0.0f;
		sampler.maxLod = 1.0f;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		if (vkCreateSampler(devices::logical::dev, &sampler, nullptr, &shadow.samplers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shadow sampler!");
		}
		// Create framebuffer
		VkFramebufferCreateInfo shadowFramebufferCreateInfo = {};
		shadowFramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		shadowFramebufferCreateInfo.renderPass = renderPassHandler->renderPassShadow;
		shadowFramebufferCreateInfo.attachmentCount = 1;
		shadowFramebufferCreateInfo.pAttachments = &shadow.imageViews[i];
		shadowFramebufferCreateInfo.width = 1024;
		shadowFramebufferCreateInfo.height = 1024;
		shadowFramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(devices::logical::dev, &shadowFramebufferCreateInfo, nullptr, &shadow.framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shadow framebuffer!");
		}
	}
}

void Presentation::createDepthResources() {
	VkFormat depthFormat = findDepthFormat();

	createImage(swapchain.extent.width, swapchain.extent.height, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depth.image, depth.imageMemory);
	depth.imageView = createImageView(depth.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

	transitionImageLayout(depth.image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}


VkFormat Presentation::findDepthFormat() {
	return findSupportedFormat(
		devices::physical::dev,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}
