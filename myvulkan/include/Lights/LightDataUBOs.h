#pragma once



#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

class LightDataUBOs {
private:
	void internalCreateUniformBuffers(std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	LightDataUBOs();
	~LightDataUBOs();

	void freeResources();
	void createUniformBuffers();
	void updateDataUBO(uint32_t);

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
};

