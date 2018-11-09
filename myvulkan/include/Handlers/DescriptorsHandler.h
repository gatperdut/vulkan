#pragma once


#include <vector>

#include <vulkan/vulkan.h>


class DescriptorsHandler {
public:
	DescriptorsHandler();
	~DescriptorsHandler();

	void createDescriptorPool();
	void resetDescriptorPool();
	void createDescriptorSetLayout();
	void createDescriptorSets();

	VkDescriptorSetLayout descriptorSetLayoutUB;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
};

