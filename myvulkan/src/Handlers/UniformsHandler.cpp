#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>


#include "Handlers/Handlers.h"
#include "Handlers/UniformsHandler.h"
#include "uniform_buffer_objects.h"
#include "buffers.h"


UniformsHandler::UniformsHandler() {

}


UniformsHandler::~UniformsHandler() {
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, uniformsHandler->uniformBuffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, uniformsHandler->uniformBuffersMemory[i], nullptr);
	}
}


void UniformsHandler::createUniformBuffer() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapchainHandler->images.size());
	uniformBuffersMemory.resize(swapchainHandler->images.size());

	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}


void UniformsHandler::updateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), 0.03f * time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//ubo.model = glm::rotate(glm::mat4(1.0f), 0 * time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = cameraHandler->viewMatrix();
	ubo.proj = glm::perspective(glm::radians(45.0f), swapchainHandler->extent.width / (float)swapchainHandler->extent.height, 0.1f, 30.0f);

	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(devicesHandler->device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(devicesHandler->device, uniformBuffersMemory[currentImage]);
}
