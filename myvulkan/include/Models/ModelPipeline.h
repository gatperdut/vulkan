#pragma once


#include <vulkan/vulkan.h>


class ModelPipeline {
public:
	ModelPipeline();
	~ModelPipeline();

	void create(VkDescriptorSetLayout);
	void freeResources();


	VkPipelineLayout layout;
	VkPipeline pipeline;
};

