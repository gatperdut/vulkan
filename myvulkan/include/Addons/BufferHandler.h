#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "vertex.h"


class BufferHandler {
private:
	void internalCreateBuffers(VkBuffer*, VkDeviceMemory*, VkBuffer*, VkDeviceMemory*, std::vector<Vertex>, std::vector<uint32_t>);

public:
	BufferHandler();
	~BufferHandler();

	void freeResources();
	void createBuffers(std::vector<Vertex>, std::vector<uint32_t>);


	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory = 0;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory = 0;
};

