#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class UniformsHandler {
private:
	void internalCreateUniformBuffer(std::vector<VkBuffer>*, std::vector<VkDeviceMemory>*);

public:
	UniformsHandler();
	~UniformsHandler();

	void computeAlignment();
	void freeResources();
	void createUniformBuffer();
	void updateUniformBuffer(uint32_t);


	VkDeviceSize alignment;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
};

