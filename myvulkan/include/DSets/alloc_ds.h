#pragma once


#include <vulkan/vulkan.h>


namespace dsets {

	void alloc(VkDescriptorSetAllocateInfo* info, VkDescriptorSetLayout* layout);

}
