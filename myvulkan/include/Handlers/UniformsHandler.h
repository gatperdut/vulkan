#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class UniformsHandler {


public:
	UniformsHandler();
	~UniformsHandler();

	void computeAlignment();


	VkDeviceSize alignment;
};

