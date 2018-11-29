#pragma once


#include <vulkan/vulkan.h>


namespace bindings {

	namespace lights {

		VkDescriptorSetLayoutBinding properties(uint32_t I, uint32_t N);

		VkDescriptorSetLayoutBinding PV(uint32_t I, uint32_t N);

	}

}

