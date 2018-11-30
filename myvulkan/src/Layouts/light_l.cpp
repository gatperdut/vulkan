#include "Handlers/Handlers.h"

#include "Layouts/create_l.h"
#include "Layouts/light_l.h"
#include "Bindings/bindings.h"


namespace layouts {

	namespace lights {

		void Attrs_PV_Depth(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N, uint32_t b2I, uint32_t b2N, uint32_t b3I, uint32_t b3N) {
			VkDescriptorSetLayoutBinding bAttrs = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_FRAGMENT_BIT, b1I, b1N);

			VkDescriptorSetLayoutBinding bPV = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, b2I, b2N);

			VkDescriptorSetLayoutBinding bDepth = bindings::create(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, b3I, b3N);

			std::vector<VkDescriptorSetLayoutBinding> bindings = { bAttrs, bPV, bDepth };

			layouts::create(bindings, dsl);
		}

		void Attrs_PVM(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N, uint32_t b2I, uint32_t b2N) {
			VkDescriptorSetLayoutBinding bAttrs = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_FRAGMENT_BIT, b1I, b1N);

			VkDescriptorSetLayoutBinding bPVM = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, b2I, b2N);
			
			std::vector<VkDescriptorSetLayoutBinding> bindings = { bAttrs, bPVM };

			layouts::create(bindings, dsl);
		}

		void PV(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N) {
			VkDescriptorSetLayoutBinding bPV = bindings::create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, b1I, b1N);

			std::vector<VkDescriptorSetLayoutBinding> bindings = { bPV };

			layouts::create(bindings, dsl);
		}

	}

}
