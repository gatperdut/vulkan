#pragma once


#include <vulkan/vulkan.h>


class ShadowPipeline {
public:
	ShadowPipeline();
	~ShadowPipeline();

	void create();
	void freeResources();


	VkPipelineLayout layout;
	VkPipeline pipeline;
};

