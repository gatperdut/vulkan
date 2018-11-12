#pragma once


#include <vulkan/vulkan.h>
#include <optional>
#include <vector>


class DevicesHandler {

public:
	DevicesHandler();
	~DevicesHandler();

	void pickPhysicalDevice();
	void createLogicalDevice();
	void computeUboAlignment();

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkPhysicalDeviceProperties properties;

	VkDeviceSize uboAlignment;

private:
	bool checkDeviceExtensionSupport(VkPhysicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice);
};

