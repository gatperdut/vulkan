#include "Bindings/model_b.h"


namespace bindings {

	namespace models {

		VkDescriptorSetLayoutBinding PVM(uint32_t bindingI, uint32_t bindingN) {
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = bindingI;
			binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			binding.descriptorCount = bindingN;
			binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			return binding;
		}

		VkDescriptorSetLayoutBinding materials(uint32_t bindingI, uint32_t bindingN) {
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = bindingI;
			binding.descriptorCount = bindingN;
			binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			return binding;
		}

	}

}
