#include "Pipelines/Parts/layout_p.h"


namespace pipelines {

	namespace parts {

		namespace layout {

			VkPipelineLayoutCreateInfo create(uint32_t nDSLs, VkDescriptorSetLayout* DSLs) {
				VkPipelineLayoutCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				info.setLayoutCount = nDSLs;
				info.pSetLayouts = DSLs;
				info.pushConstantRangeCount = 0;
				info.pPushConstantRanges = nullptr;

				return info;
			}

		}

	}

}