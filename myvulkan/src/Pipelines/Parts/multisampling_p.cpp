#include "Pipelines/Parts/multisampling_p.h"


namespace pipelines {

	namespace parts {

		namespace multisampling {

			VkPipelineMultisampleStateCreateInfo create() {
				VkPipelineMultisampleStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				info.sampleShadingEnable = VK_FALSE;
				info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

				return info;
			}

		}

	}

}