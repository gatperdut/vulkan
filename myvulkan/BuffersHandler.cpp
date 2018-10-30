#include <cstring>

#include "Handlers.h"
#include "BuffersHandler.h"
#include "buffers.h"


BuffersHandler::BuffersHandler() {

}


BuffersHandler::~BuffersHandler() {
	vkDestroyBuffer(devicesHandler->device, vertexAndIndexBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, vertexAndIndexBufferMemory, nullptr);
}


void BuffersHandler::createVertexAndIndexBuffer() {
	VkDeviceSize verticesSize = sizeof(modelsHandler->vertices[0]) * modelsHandler->vertices.size();
	VkDeviceSize indicesSize = sizeof(modelsHandler->indices[0]) * modelsHandler->indices.size();

	VkDeviceSize bufferSize = verticesSize + indicesSize;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(devicesHandler->device, stagingBufferMemory, 0, verticesSize, 0, &data);
	memcpy(data, modelsHandler->vertices.data(), (size_t)verticesSize);
	vkUnmapMemory(devicesHandler->device, stagingBufferMemory);

	vkMapMemory(devicesHandler->device, stagingBufferMemory, verticesSize, indicesSize, 0, &data);
	memcpy(data, modelsHandler->indices.data(), (size_t)indicesSize);
	vkUnmapMemory(devicesHandler->device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexAndIndexBuffer, vertexAndIndexBufferMemory);
	copyBuffer(stagingBuffer, buffersHandler->vertexAndIndexBuffer, bufferSize);

	vkDestroyBuffer(devicesHandler->device, stagingBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, stagingBufferMemory, nullptr);
}


