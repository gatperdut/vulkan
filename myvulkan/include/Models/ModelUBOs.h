#pragma once


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>


class ModelUBOs {
private:
	void internalCreateUniformBuffers(VkDeviceSize, std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	ModelUBOs();
	~ModelUBOs();

	void freeResources();
	void createUniformBuffers(VkDeviceSize);
	void updateUniformBuffer(uint32_t, glm::vec3, glm::vec3);

	VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding();

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

