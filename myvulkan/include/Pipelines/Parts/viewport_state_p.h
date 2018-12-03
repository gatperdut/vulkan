#pragma once


#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace viewport_state {

			VkPipelineViewportStateCreateInfo create(VkViewport* viewport, VkRect2D* scissor);

		}

	}

}
