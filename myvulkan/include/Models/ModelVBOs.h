#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Models/model_vertex.h"


class ModelVBOs {
private:
	void internalCreateBuffers(VkBuffer*, VkDeviceMemory*, VkBuffer*, VkDeviceMemory*, std::vector<ModelVertex>, std::vector<uint32_t>);

public:
	ModelVBOs();
	~ModelVBOs();

	void freeResources();
	void createBuffers(std::vector<ModelVertex>, std::vector<uint32_t>);


	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory = 0;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory = 0;
};

