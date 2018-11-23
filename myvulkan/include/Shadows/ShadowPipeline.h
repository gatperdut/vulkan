#pragma once


#include <vulkan/vulkan.h>


class ShadowPipeline {
private:
	VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding();

public:
	ShadowPipeline();
	~ShadowPipeline();

	void create();
	void freeResources();
	void createDescriptorSetLayout();


	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout layout;
	VkPipeline pipeline;
};

