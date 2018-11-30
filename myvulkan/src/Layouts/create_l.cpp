#include "Handlers/Handlers.h"

#include "Layouts/create_l.h"


namespace layouts {

	void create(std::vector<VkDescriptorSetLayoutBinding> bindings, VkDescriptorSetLayout* dsl) {
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = bindings.size();
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, dsl) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

}
