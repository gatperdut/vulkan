#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Models/vertex.h"


class ModelVBOs {
private:
	void internalCreateBuffers(VkBuffer*, VkDeviceMemory*, VkBuffer*, VkDeviceMemory*, std::vector<Vertex>, std::vector<uint32_t>);

public:
	ModelVBOs();
	~ModelVBOs();

	void freeResources();
	void createBuffers(std::vector<Vertex>, std::vector<uint32_t>);


	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory = 0;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory = 0;
};

