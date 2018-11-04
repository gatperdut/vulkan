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


	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<std::vector<VkDescriptorSet>> descriptorSets;
};

