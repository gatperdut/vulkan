#pragma once


#include <array>
#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace vertex_input {

			VkPipelineVertexInputStateCreateInfo create(uint32_t nBindings, VkVertexInputBindingDescription* bindings, uint32_t nAttributes, VkVertexInputAttributeDescription* attributes);

		}

	}

}