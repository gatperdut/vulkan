#pragma once


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

#include "Model.h"
#include "vertex.h"


class UboHandler {
private:
	void internalCreateUniformBuffer(VkDeviceSize, std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	UboHandler();
	~UboHandler();

	void freeResources();
	void createUniformBuffer(VkDeviceSize);
	void updateUniformBuffer(uint32_t, glm::vec3);

	VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding();

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

