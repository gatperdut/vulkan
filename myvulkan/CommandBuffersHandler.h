#pragma once


#include <vector>
#include <vulkan/vulkan.h>


class CommandBuffersHandler {
public:
	CommandBuffersHandler();
	~CommandBuffersHandler();

	void createCommandBuffers();


	std::vector<VkCommandBuffer> commandBuffers;
};

