#pragma once


#include <vulkan/vulkan.h>


namespace layouts {

	namespace lights {

		void Attrs_PV_Depth(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N, uint32_t b2I, uint32_t b2N, uint32_t b3I, uint32_t b3N);

		void PV(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N);

		void PVM(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N);

	}

}
