#pragma once


#include <vulkan/vulkan.h>


namespace pipelines {

	struct Pipeline {
		VkPipeline handle;
		VkPipelineLayout layout;
	};

	void destroy(Pipeline pipeline);

}