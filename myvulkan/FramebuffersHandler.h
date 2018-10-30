#pragma once


#include <vulkan/vulkan.h>
#include <vector>


class FramebuffersHandler {
public:
	FramebuffersHandler();
	~FramebuffersHandler();

	void createFramebuffers();


	std::vector<VkFramebuffer> swapChainFramebuffers;
};

