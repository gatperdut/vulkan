#pragma once

#include <vulkan/vulkan.h>
#include <optional>


struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};


class QueuesHandler {
public:
	QueuesHandler();
	~QueuesHandler();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);


	VkQueue graphicsQueue;
	VkQueue presentQueue;
};

