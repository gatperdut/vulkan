#pragma once


#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace color_blend {

			VkPipelineColorBlendStateCreateInfo create(uint32_t nAttachments, VkPipelineColorBlendAttachmentState* attachments);

		}

	}

}