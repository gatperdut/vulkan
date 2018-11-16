#pragma once


#include <vulkan/vulkan.h>


class PipelineHandler {
public:
	PipelineHandler();
	~PipelineHandler();

	void create(VkDescriptorSetLayout);
	void freeResources();


	VkPipelineLayout layout;
	VkPipeline pipeline;
};

