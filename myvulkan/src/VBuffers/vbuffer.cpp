#include "Handlers/Handlers.h"
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
		vkMapMemory(devicesHandler->device, stagemem, 0, vSize, 0, &data);
		memcpy(data, vertices, (size_t)vSize);
		vkUnmapMemory(devicesHandler->device, stagemem);
		createBuffer(vSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vbuffer.vbuf, vbuffer.vmem);
		copyBuffer(stagebuf, vbuffer.vbuf, vSize);

		vkDestroyBuffer(devicesHandler->device, stagebuf, nullptr);
		vkFreeMemory(devicesHandler->device, stagemem, nullptr);


		createBuffer(iSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagebuf, stagemem);
		vkMapMemory(devicesHandler->device, stagemem, 0, iSize, 0, &data);
		memcpy(data, indices.data(), (size_t)iSize);
		vkUnmapMemory(devicesHandler->device, stagemem);
		createBuffer(iSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vbuffer.ibuf, vbuffer.imem);
		copyBuffer(stagebuf, vbuffer.ibuf, iSize);

		vkDestroyBuffer(devicesHandler->device, stagebuf, nullptr);
		vkFreeMemory(devicesHandler->device, stagemem, nullptr);
	}

	void destroy(vbuffer& vbuffer) {
		vkDestroyBuffer(devicesHandler->device, vbuffer.vbuf, nullptr);
		vkFreeMemory(devicesHandler->device, vbuffer.vmem, nullptr);

		vkDestroyBuffer(devicesHandler->device, vbuffer.ibuf, nullptr);
		vkFreeMemory(devicesHandler->device, vbuffer.imem, nullptr);
	}

}
