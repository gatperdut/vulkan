#include "Handlers/Handlers.h"
#include "Uniforms/uniform.h"
#include "buffers.h"


namespace uniforms {

	void create(uniform& uniform, size_t count, size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memprops) {
		uniform.count = count;
		uniform.buffers.resize(count);
		uniform.memories.resize(count);
		for (size_t i = 0; i < count; i++) {
			createBuffer(size, usage, memprops, uniform.buffers[i], uniform.memories[i]);
		}
	}

	void destroy(uniform& uniform) {
		for (size_t i = 0; i < uniform.count; i++) {
			vkDestroyBuffer(devicesHandler->device, uniform.buffers[i], nullptr);
			vkFreeMemory(devicesHandler->device, uniform.memories[i], nullptr);
		}
	}

	void update(uniform& uniform, uint32_t index, size_t size, void* data) {
		void* mapped;

		vkMapMemory(devicesHandler->device, uniform.memories[index], 0, size, 0, &mapped);
		memcpy(mapped, data, size);
		vkUnmapMemory(devicesHandler->device, uniform.memories[index]);
	}

}
