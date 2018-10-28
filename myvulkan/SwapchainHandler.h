#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class SwapchainHandler {
public:
	SwapchainHandler();
	~SwapchainHandler();

	void createSwapchain();
	void createImageViews();


	VkSwapchainKHR swapchain;
	std::vector<VkImage> images;
	VkFormat format;
	VkExtent2D extent;
	std::vector<VkImageView> imageViews;


private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&);
};

