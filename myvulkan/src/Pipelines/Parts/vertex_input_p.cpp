#include <Pipelines/Parts/vertex_input_p.h>


namespace pipelines {

	namespace parts {

		namespace vertex_input {

			VkPipelineVertexInputStateCreateInfo create(uint32_t nBindings, VkVertexInputBindingDescription* bindings, uint32_t nAttributes, VkVertexInputAttributeDescription* attributes) {
				VkPipelineVertexInputStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

				info.vertexBindingDescriptionCount = nBindings;
				info.pVertexBindingDescriptions = bindings;

				info.vertexAttributeDescriptionCount = nAttributes;
				info.pVertexAttributeDescriptions = attributes;

				return info;
			}

		}

	}

}