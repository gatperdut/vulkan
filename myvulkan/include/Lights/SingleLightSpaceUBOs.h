#pragma once



#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

class SingleLightSpaceUBOs {
private:
	void internalCreateUniformBuffers(std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	SingleLightSpaceUBOs();
	~SingleLightSpaceUBOs();

	void freeResources();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t, glm::mat4);

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

