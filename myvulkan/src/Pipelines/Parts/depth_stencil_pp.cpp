#include "Pipelines/Parts/depth_stencil_pp.h"


namespace pipelines {

	namespace parts {

		namespace depth_stencil {

			VkPipelineDepthStencilStateCreateInfo create() {
				VkPipelineDepthStencilStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				info.depthTestEnable = VK_TRUE;
				info.depthWriteEnable = VK_TRUE;
				info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
				info.stencilTestEnable = VK_FALSE;

				return info;
			}
		}

	}

}