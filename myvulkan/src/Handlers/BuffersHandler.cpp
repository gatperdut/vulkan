#include <cstring>

#include "Handlers/Handlers.h"
#include "Handlers/BuffersHandler.h"
#include "buffers.h"


BuffersHandler::BuffersHandler() {

}


BuffersHandler::~BuffersHandler() {
	vkDestroyBuffer(devicesHandler->device, vertexAndIndexBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, vertexAndIndexBufferMemory, nullptr);
}


void BuffersHandler::createVertexAndIndexBuffer() {
	VkDeviceSize verticesSize = modelsHandler->verticesSize();
	VkDeviceSize indicesSize = modelsHandler->indicesSize();

	VkDeviceSize bufferSize = verticesSize + indicesSize;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	VkDeviceSize offset = 0;
	VkDeviceSize modelVerticesSize, modelIndicesSize;
	for (auto model : modelsHandler->models) {
		modelVerticesSize = model->verticesSize();
		modelIndicesSize = model->indicesSize();

		vkMapMemory(devicesHandler->device, stagingBufferMemory, offset, modelVerticesSize, 0, &data);
		memcpy(data, model->vertices.data(), (size_t)modelVerticesSize);
		vkUnmapMemory(devicesHandler->device, stagingBufferMemory);

		vkMapMemory(devicesHandler->device, stagingBufferMemory, offset + modelVerticesSize, modelIndicesSize, 0, &data);
		memcpy(data, model->indices.data(), (size_t)modelIndicesSize);
		vkUnmapMemory(devicesHandler->device, stagingBufferMemory);

		offset += modelVerticesSize + modelIndicesSize;
	}

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexAndIndexBuffer, vertexAndIndexBufferMemory);
	copyBuffer(stagingBuffer, buffersHandler->vertexAndIndexBuffer, bufferSize);

	vkDestroyBuffer(devicesHandler->device, stagingBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, stagingBufferMemory, nullptr);
}


