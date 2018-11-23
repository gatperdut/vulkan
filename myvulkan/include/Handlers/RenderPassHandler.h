#pragma once


#include <vulkan/vulkan.h>


class RenderPassHandler {
public:
	RenderPassHandler();
	~RenderPassHandler();

	void createRenderPassRegular();
	void createRenderPassShadow();

	VkRenderPass renderPassRegular;
	VkRenderPass renderPassShadow;
};

