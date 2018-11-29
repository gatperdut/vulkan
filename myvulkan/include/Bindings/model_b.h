#pragma once


#include <vulkan/vulkan.h>


namespace bindings {

	namespace models {

		VkDescriptorSetLayoutBinding PVM(uint32_t bindingI, uint32_t bindingN);

		VkDescriptorSetLayoutBinding materials(uint32_t bindingI, uint32_t bindingN);

	}

}

