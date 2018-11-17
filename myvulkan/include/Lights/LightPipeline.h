#pragma once


#include <vulkan/vulkan.h>


class LightPipeline {
public:
	LightPipeline();
	~LightPipeline();

	void create(VkDescriptorSetLayout);
	void freeResources();


	VkPipelineLayout layout;
	VkPipeline pipeline;
};

