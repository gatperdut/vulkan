#pragma once


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

class LightModelUBOs {
private:
	void internalCreateUniformBuffers(std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	LightModelUBOs();
	~LightModelUBOs();

	void freeResources();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t, glm::vec3);

	VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding();

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

