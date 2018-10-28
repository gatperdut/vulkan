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


	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

private:
	bool checkDeviceExtensionSupport(VkPhysicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice);
};

