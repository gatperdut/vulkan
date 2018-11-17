#include <cstring>

#include "Handlers/Handlers.h"
#include "Models/ModelVBOs.h"
#include "buffers.h"


ModelVBOs::ModelVBOs() {

}


ModelVBOs::~ModelVBOs() {
	freeResources();
}


void ModelVBOs::freeResources() {
	vkDestroyBuffer(devicesHandler->device, vertexBuffer, nullptr);
	vkDestroyBuffer(devicesHandler->device, indexBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, vertexBufferMemory, nullptr);
	vkFreeMemory(devicesHandler->device, indexBufferMemory, nullptr);
}


void ModelVBOs::internalCreateBuffers(VkBuffer *vBuffer, VkDeviceMemory *vBufferMemory, VkBuffer *iBuffer, VkDeviceMemory *iBufferMemory, std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
	VkDeviceSize verticesSize = vertices.size() * sizeof(vertices[0]);
	VkDeviceSize indicesSize = indices.size() * sizeof(indices[0]);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	void* data;
	
	createBuffer(verticesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
	vkMapMemory(devicesHandler->device, stagingBufferMemory, 0, verticesSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)verticesSize);
	vkUnmapMemory(devicesHandler->device, stagingBufferMemory);
	createBuffer(verticesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *vBuffer, *vBufferMemory);
	copyBuffer(stagingBuffer, *vBuffer, verticesSize);

	vkDestroyBuffer(devicesHandler->device, stagingBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, stagingBufferMemory, nullptr);


	createBuffer(indicesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
	vkMapMemory(devicesHandler->device, stagingBufferMemory, 0, indicesSize, 0, &data);
	memcpy(data, indices.data(), (size_t)indicesSize);
	vkUnmapMemory(devicesHandler->device, stagingBufferMemory);
	createBuffer(indicesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *iBuffer, *iBufferMemory);
	copyBuffer(stagingBuffer, *iBuffer, indicesSize);

	vkDestroyBuffer(devicesHandler->device, stagingBuffer, nullptr);
	vkFreeMemory(devicesHandler->device, stagingBufferMemory, nullptr);
}


void ModelVBOs::createBuffers(std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
	if (!vertexBufferMemory) {
		internalCreateBuffers(&vertexBuffer, &vertexBufferMemory, &indexBuffer, &indexBufferMemory, vertices, indices);
	}
	else {
		VkBuffer newVertexBuffer = 0;
		VkDeviceMemory newVertexBufferMemory = 0;
		VkBuffer newIndexBuffer = 0;
		VkDeviceMemory newIndexBufferMemory = 0;

		internalCreateBuffers(&newVertexBuffer, &newVertexBufferMemory, &newIndexBuffer, &newIndexBufferMemory, vertices, indices);

		freeResources();

		vertexBuffer = newVertexBuffer;
		indexBuffer = newIndexBuffer;
		vertexBufferMemory = newVertexBufferMemory;
		indexBufferMemory = newIndexBufferMemory;
	}
}


