#pragma once


#include <vector>
#include <vulkan/vulkan.h>


class CommandBuffersHandler {
private:
	void internalCreateCommandBuffers(std::vector<VkCommandBuffer>*);

public:
	CommandBuffersHandler();
	~CommandBuffersHandler();

	void createCommandBuffers();


	std::vector<VkCommandBuffer> commandBuffers;
};

