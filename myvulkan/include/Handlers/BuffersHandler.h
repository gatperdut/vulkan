#pragma once


#include <vulkan/vulkan.h>


class BuffersHandler {
private:
	void internalCreateVertexAndIndexBuffer(VkBuffer*, VkDeviceMemory*);

public:
	BuffersHandler();
	~BuffersHandler();

	void freeResources();
	void createVertexAndIndexBuffer();


	VkBuffer vertexAndIndexBuffer;
	VkDeviceMemory vertexAndIndexBufferMemory = 0;
};

