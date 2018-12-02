#pragma once


#include <vector>
#include <vulkan/vulkan.h>


namespace instance {

	const std::vector<const char*> extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	extern VkInstance instance;

	void create();

	std::vector<const char*> reqExtensions();

	void destroy();

}
