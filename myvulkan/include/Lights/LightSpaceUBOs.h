#pragma once



#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

class LightSpaceUBOs {
private:
	void internalCreateUniformBuffers(std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	LightSpaceUBOs();
	~LightSpaceUBOs();

	void freeResources();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t);

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

