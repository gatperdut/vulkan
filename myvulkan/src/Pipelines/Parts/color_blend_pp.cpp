#include "Pipelines/Parts/color_blend_pp.h"


namespace pipelines {

	namespace parts {

		namespace color_blend {

			VkPipelineColorBlendStateCreateInfo create(uint32_t nAttachments, VkPipelineColorBlendAttachmentState* attachments) {
				VkPipelineColorBlendStateCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				info.logicOpEnable = VK_FALSE;
				info.attachmentCount = nAttachments;
				info.pAttachments = attachments;

				return info;
			}

		}

	}

}