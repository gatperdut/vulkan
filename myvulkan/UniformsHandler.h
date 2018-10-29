#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class UniformsHandler {
public:
	UniformsHandler();
	~UniformsHandler();

	void createUniformBuffer();
	void updateUniformBuffer(uint32_t);


	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
};

