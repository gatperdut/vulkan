#pragma once


#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace rasterizer {

			VkPipelineRasterizationStateCreateInfo create(VkCullModeFlags cullMode, VkBool32 depthBiasEnable);

		}

	}

}