#include <vector>

#include "Handlers/Handlers.h"

#include "Layouts/create_l.h"
#include "Layouts/model_l.h"
#include "Bindings/bindings.h"


namespace layouts {

	namespace models {

		void PVM_Materials(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N, uint32_t b2I, uint32_t b2N) {
			VkDescriptorSetLayoutBinding bPVM = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, b1I, b1N);
			VkDescriptorSetLayoutBinding bMaterials = bindings::create(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, b2I, b2N);

			std::vector<VkDescriptorSetLayoutBinding> bindings = { bPVM, bMaterials };

			layouts::create(bindings, dsl);
		}

		void PVM(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N) {
			VkDescriptorSetLayoutBinding bPVM = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, b1I, b1N);

			std::vector<VkDescriptorSetLayoutBinding> bindings = { bPVM };

			layouts::create(bindings, dsl);
		}

	}

}