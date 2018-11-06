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
	std::vector<VkDescriptorSetLayout> descriptorSetlayouts();


	VkDescriptorSetLayout descriptorSetLayoutUB;
	VkDescriptorSetLayout descriptorSetLayoutCIS;
	VkDescriptorPool descriptorPool;
	std::vector<std::vector<VkDescriptorSet>> descriptorSets;
};

