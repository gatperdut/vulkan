#pragma once


#include <vulkan/vulkan.h>
#include <vector>

#include "capabilities.h"

class CapabilitiesHandler {

public:
	CapabilitiesHandler();
	~CapabilitiesHandler();

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice);
};

