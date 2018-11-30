#pragma once


#include <vector>
#include <vulkan/vulkan.h>


namespace layouts {

	void create(std::vector<VkDescriptorSetLayoutBinding> bindings, VkDescriptorSetLayout* dsl);

}
