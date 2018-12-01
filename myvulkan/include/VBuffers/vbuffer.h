#pragma once


#include <vector>
#include <vulkan/vulkan.h>


namespace vbuffers {

	struct vbuffer {
		VkBuffer vbuf;
		VkDeviceMemory vmem;
		VkBuffer ibuf;
		VkDeviceMemory imem;
	};

	void create(vbuffer& vbuffer, void* vertices, uint32_t nvertices, size_t vertexsize, std::vector<uint32_t>& indices);

	void destroy(vbuffer& vbuffer);
}
