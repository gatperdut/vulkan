#pragma once


#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace layout {

			VkPipelineLayoutCreateInfo create(uint32_t nDSLs, VkDescriptorSetLayout* DSLs);

		}

	}

}