#pragma once


#include <vulkan/vulkan.h>


class CommandsHandler {
public:
	CommandsHandler();
	~CommandsHandler();

	void createCommandPool();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer );


	VkCommandPool commandPool;
};

