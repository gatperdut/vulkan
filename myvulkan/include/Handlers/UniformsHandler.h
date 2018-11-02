#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class UniformsHandler {
public:
	UniformsHandler();
	~UniformsHandler();

	void computeAlignment();
	void createUniformBuffer();
	void updateUniformBuffer(uint32_t);


	size_t alignment;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
};

