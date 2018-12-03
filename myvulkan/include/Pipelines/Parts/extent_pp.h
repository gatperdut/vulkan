#pragma once


#include <vulkan/vulkan.h>


namespace pipelines {

	namespace parts {

		namespace extent {

			VkExtent2D create(uint32_t width, uint32_t height);

		}

	}

}