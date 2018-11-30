#include "Writes/create_w.h"


namespace writes {

	void buffer(VkWriteDescriptorSet* write, VkDescriptorSet set, uint32_t binding, uint32_t arrayElem, uint32_t count, VkDescriptorBufferInfo* info) {
		write->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write->dstSet = set;
		write->dstBinding = binding;
		write->dstArrayElement = arrayElem;
		write->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		write->descriptorCount = count;
		write->pBufferInfo = info;
	}

	void image(VkWriteDescriptorSet* write, VkDescriptorSet set, uint32_t binding, uint32_t arrayElem, uint32_t count, VkDescriptorImageInfo* info) {
		write->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write->dstSet = set;
		write->dstBinding = binding;
		write->dstArrayElement = arrayElem;
		write->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write->descriptorCount = count;
		write->pImageInfo = info;
	}
}
