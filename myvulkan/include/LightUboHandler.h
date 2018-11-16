#pragma once


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

class LightUboHandler {
private:
	void internalCreateUniformBuffers(std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	LightUboHandler();
	~LightUboHandler();

	void freeResources();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t);

	VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding();

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

