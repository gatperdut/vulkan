#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class Presentation {
public:
	Presentation();
	~Presentation();

	void createSwapchain();
	void cleanupSwapChain();
	void createImageViews();
	void createFramebuffersRegular();
	void createFramebuffersShadow();
	void createDepthResources();

	struct {
		VkSwapchainKHR handle;
		std::vector<VkFramebuffer> framebuffers;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
		VkExtent2D extent;
		VkFormat format;
	} swapchain;

	struct {
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	} depth;

	struct {
		std::vector<VkFramebuffer> framebuffers;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
		std::vector<VkDeviceMemory> imageMemories;
		std::vector<VkSampler> samplers;
	} shadow;


	VkFormat findDepthFormat();
};

