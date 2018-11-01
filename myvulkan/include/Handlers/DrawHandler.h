#pragma once


#include <vulkan/vulkan.h>


class DrawHandler {
public:
	DrawHandler();
	~DrawHandler();

	void drawFrame();


	size_t currentFrame = 0;
};

