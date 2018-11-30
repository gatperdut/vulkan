#pragma once


#include <vulkan/vulkan.h>


class LightPipeline {
public:
	LightPipeline();
	~LightPipeline();

	void create(VkDescriptorSetLayout dsl_Attrs_PVM);
	void freeResources();


	VkPipelineLayout layout;
	VkPipeline pipeline;
};

