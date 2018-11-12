#pragma once


#include <vector>

#include <vulkan/vulkan.h>


class DescriptorsHandler {
public:
	DescriptorsHandler();
	~DescriptorsHandler();

	void createDescriptorPool();
	void resetDescriptorPool();

	VkDescriptorPool descriptorPool;
	std::vector<std::vector<VkDescriptorSet>> descriptorSets;
};

