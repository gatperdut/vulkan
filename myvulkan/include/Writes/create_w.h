#pragma once


#include <vulkan/vulkan.h>


namespace writes {

	void buffer(VkWriteDescriptorSet* write, VkDescriptorSet set, uint32_t binding, uint32_t arrayElem, uint32_t count, VkDescriptorBufferInfo* info);

	void image(VkWriteDescriptorSet* write, VkDescriptorSet set, uint32_t binding, uint32_t arrayElem, uint32_t count, VkDescriptorImageInfo* info);

}
