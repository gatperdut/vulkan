#pragma once


#include <vulkan/vulkan.h>


namespace bindings {

	VkDescriptorSetLayoutBinding create(VkDescriptorType type, VkShaderStageFlags flags, uint32_t binding, uint32_t count);

}

