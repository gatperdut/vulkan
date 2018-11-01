#pragma once


#include <vulkan/vulkan.h>


class RenderPassHandler {
private:
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>&, VkImageTiling, VkFormatFeatureFlags);

public:
	RenderPassHandler();
	~RenderPassHandler();

	void createRenderPass();
	void createDepthResources();

	VkRenderPass renderPass;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
};

