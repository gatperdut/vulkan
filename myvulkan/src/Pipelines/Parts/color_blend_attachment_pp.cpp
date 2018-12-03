#include "Pipelines/Parts/color_blend_attachment_pp.h"


namespace pipelines {

	namespace parts {

		namespace color_blend_attachment {

			VkPipelineColorBlendAttachmentState create() {
				VkPipelineColorBlendAttachmentState attachment = {};
				attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
				attachment.blendEnable = VK_FALSE;

				return attachment;
			}

		}

	}

}