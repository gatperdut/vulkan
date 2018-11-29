#include <vector>

#include "Handlers/Handlers.h"

#include "Layouts/model_l.h"
#include "Bindings/model_b.h"


namespace layouts {

	namespace models {

		void PVM_Mats(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N, uint32_t b2I, uint32_t b2N) {
			VkDescriptorSetLayoutBinding bindingPVM = bindings::models::PVM(b1I, b1N);
			VkDescriptorSetLayoutBinding bindingMats = bindings::models::materials(b2I, b2N);

			std::vector<VkDescriptorSetLayoutBinding> bindings = { bindingPVM, bindingMats };

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = bindings.size();
			layoutInfo.pBindings = bindings.data();

			if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, dsl) != VK_SUCCESS) {
				throw std::runtime_error("failed to create CIS descriptor set layout!");
			}
		}

		void PVM(VkDescriptorSetLayout* dsl, uint32_t b1I, uint32_t b1N) {
			VkDescriptorSetLayoutBinding bindingPVM = bindings::models::PVM(b1I, b1N);

			std::vector<VkDescriptorSetLayoutBinding> bindings = { bindingPVM };

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = bindings.size();
			layoutInfo.pBindings = bindings.data();

			if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, dsl) != VK_SUCCESS) {
				throw std::runtime_error("failed to create CIS descriptor set layout!");
			}
		}

	}

}
