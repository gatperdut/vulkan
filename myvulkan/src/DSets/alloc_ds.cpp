#pragma once


#include "Handlers/Handlers.h"
#include "Descriptors/pool.h"
#include "DSets/alloc_ds.h"


namespace dsets {

	void alloc(VkDescriptorSetAllocateInfo* info, VkDescriptorSetLayout* layout) {
		info->sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info->descriptorPool = descriptors::pool::handle;
		info->descriptorSetCount = 1;
		info->pSetLayouts = layout;
	}

}
