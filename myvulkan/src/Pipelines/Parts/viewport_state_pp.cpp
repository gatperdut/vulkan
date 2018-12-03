#include "Pipelines/Parts/viewport_state_pp.h"


namespace pipelines {

	namespace parts {

		namespace viewport_state {

			VkPipelineViewportStateCreateInfo create(VkViewport* viewport, VkRect2D* scissor) {
				VkPipelineViewportStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				info.viewportCount = 1;
				info.pViewports = viewport;
				info.scissorCount = 1;
				info.pScissors = scissor;

				return info;
			}

		}

	}

}
