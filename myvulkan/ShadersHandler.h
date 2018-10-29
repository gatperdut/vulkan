#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class ShadersHandler {
public:
	ShadersHandler();
	~ShadersHandler();

	VkShaderModule createShaderModule(const std::vector<char>&);
};

