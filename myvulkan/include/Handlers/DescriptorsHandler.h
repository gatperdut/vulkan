#pragma once


#include <vector>

#include <vulkan/vulkan.h>


class DescriptorsHandler {
public:
	DescriptorsHandler();
	~DescriptorsHandler();

	void createDescriptorPool();
	void createDescriptorSetLayout();
	void createDescriptorSets();


	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
};

