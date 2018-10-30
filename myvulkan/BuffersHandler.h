#pragma once


#include <vulkan/vulkan.h>


class BuffersHandler {
public:
	BuffersHandler();
	~BuffersHandler();

	void createVertexAndIndexBuffer();


	VkBuffer vertexAndIndexBuffer;
	VkDeviceMemory vertexAndIndexBufferMemory;
};

