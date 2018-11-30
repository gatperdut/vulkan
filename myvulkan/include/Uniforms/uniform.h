#pragma once


#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>


namespace uniforms {

	struct uniform {
		size_t count;
		std::vector<VkBuffer> buffers;
		std::vector<VkDeviceMemory> memories;
	};

	void create(uniform& uniform, size_t count, size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memprops);

	void destroy(uniform& uniform);

	void update(uniform& uniform, uint32_t index, size_t size, void* data);
}
