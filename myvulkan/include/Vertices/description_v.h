#pragma once


#include <vector>
#include <vulkan/vulkan.h>


namespace vertices {

	struct Description {
		VkVertexInputBindingDescription binding;
		std::vector< VkVertexInputAttributeDescription> attributes;
	};

}
