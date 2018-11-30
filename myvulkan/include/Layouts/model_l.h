#pragma once


#include <vulkan/vulkan.h>


namespace layouts {

	namespace models {

		void PVM_Materials(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N, uint32_t b2I, uint32_t b2N);

		void PVM(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N);
	}

}
