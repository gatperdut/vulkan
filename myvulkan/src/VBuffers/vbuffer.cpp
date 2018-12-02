#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "VBuffers/vbuffer.h"
#include "buffers.h"


namespace vbuffers {

	void create(vbuffer& vbuffer, void* vertices, uint32_t nvertices, size_t vertexsize, std::vector<uint32_t>& indices) {
		VkDeviceSize vSize = nvertices * vertexsize;
		VkDeviceSize iSize = indices.size() * sizeof(indices[0]);

		VkBuffer stagebuf;
		VkDeviceMemory stagemem;

		void* data;

		createBuffer(vSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagebuf, stagemem);
		vkMapMemory(devices::logical::dev, stagemem, 0, vSize, 0, &data);
		memcpy(data, vertices, (size_t)vSize);
		vkUnmapMemory(devices::logical::dev, stagemem);
		createBuffer(vSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vbuffer.vbuf, vbuffer.vmem);
		copyBuffer(stagebuf, vbuffer.vbuf, vSize);

		vkDestroyBuffer(devices::logical::dev, stagebuf, nullptr);
		vkFreeMemory(devices::logical::dev, stagemem, nullptr);


		createBuffer(iSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagebuf, stagemem);
		vkMapMemory(devices::logical::dev, stagemem, 0, iSize, 0, &data);
		memcpy(data, indices.data(), (size_t)iSize);
		vkUnmapMemory(devices::logical::dev, stagemem);
		createBuffer(iSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vbuffer.ibuf, vbuffer.imem);
		copyBuffer(stagebuf, vbuffer.ibuf, iSize);

		vkDestroyBuffer(devices::logical::dev, stagebuf, nullptr);
		vkFreeMemory(devices::logical::dev, stagemem, nullptr);
	}

	void destroy(vbuffer& vbuffer) {
		vkDestroyBuffer(devices::logical::dev, vbuffer.vbuf, nullptr);
		vkFreeMemory(devices::logical::dev, vbuffer.vmem, nullptr);

		vkDestroyBuffer(devices::logical::dev, vbuffer.ibuf, nullptr);
		vkFreeMemory(devices::logical::dev, vbuffer.imem, nullptr);
	}

}
