#pragma once


#include <vulkan/vulkan.h>


class PipelinesHandler {
public:
	PipelinesHandler();
	~PipelinesHandler();

	void createGraphicsPipeline();


	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
};

