#pragma once


#include <Bindings/bindings.h>


namespace bindings {

	VkDescriptorSetLayoutBinding create(VkDescriptorType type, VkShaderStageFlags flags, uint32_t binding, uint32_t count) {
		VkDescriptorSetLayoutBinding layoutBinding = {};
		layoutBinding.descriptorType = type;
		layoutBinding.binding = binding;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = flags;

		return layoutBinding;
	}

}

