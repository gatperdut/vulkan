#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Lights/light_vertex.h"


class LightVBOs {
private:
	void internalCreateBuffers(VkBuffer*, VkDeviceMemory*, VkBuffer*, VkDeviceMemory*, std::vector<LightVertex>, std::vector<uint32_t>);

public:
	LightVBOs();
	~LightVBOs();

	void freeResources();
	void createBuffers(std::vector<LightVertex>, std::vector<uint32_t>);


	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory = 0;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory = 0;
};

