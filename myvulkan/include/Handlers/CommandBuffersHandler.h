#pragma once


#include <vector>
#include <vulkan/vulkan.h>


class CommandBuffersHandler {
public:
	CommandBuffersHandler();
	~CommandBuffersHandler();

	void createCommandBuffersRegular();
	void createCommandBuffersShadow();


	std::vector<VkCommandBuffer> commandBuffersRegular;
	std::vector<VkCommandBuffer> commandBuffersShadow;
};

