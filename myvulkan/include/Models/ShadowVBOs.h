#pragma once


#include <vector>
#include <vulkan/vulkan.h>

#include "Shadows/shadow_vertex.h"


class ShadowVBOs {
private:
	void internalCreateBuffers(VkBuffer*, VkDeviceMemory*, VkBuffer*, VkDeviceMemory*, std::vector<ShadowVertex>, std::vector<uint32_t>);

public:
	ShadowVBOs();
	~ShadowVBOs();

	void freeResources();
	void createBuffers(std::vector<ShadowVertex>, std::vector<uint32_t>);


	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory = 0;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory = 0;
};

