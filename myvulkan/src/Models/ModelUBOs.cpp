#include <glm/gtc/matrix_transform.hpp>
#include <chrono>


#include "Handlers/Handlers.h"
#include "Models/ModelUBOs.h"
#include "Descriptors/model_d.h"
#include "buffers.h"

ModelUBOs::ModelUBOs() {

}


ModelUBOs::~ModelUBOs() {
	freeResources();
}

void ModelUBOs::freeResources() {
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void ModelUBOs::internalCreateUniformBuffers(VkDeviceSize size, std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	(*buffers).resize(presentation->swapchain.images.size());
	(*buffersMemories).resize(presentation->swapchain.images.size());
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void ModelUBOs::createUniformBuffers(VkDeviceSize size) {
	if (!buffers.size()) {
		internalCreateUniformBuffers(size, &buffers, &memories);
	}
	else {
		std::vector<VkBuffer> newBuffers;
		std::vector<VkDeviceMemory> newMemories;

		internalCreateUniformBuffers(size, &newBuffers, &newMemories);

		freeResources();
		for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
			buffers[i] = newBuffers[i];
			memories[i] = newMemories[i];
		}
	}
}


void ModelUBOs::updateUniformBuffer(uint32_t currentImage, glm::vec3 pos, glm::vec3 scale) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	void* data;
	
	descriptors::models::PVM ubo = {};
	//ubo.model = pos.x == 0.0f ? glm::translate(glm::mat4(1.0), pos) : glm::translate(glm::mat4(1.0), glm::vec3(pos.x, sin(time) * 8.0f + 4.0f, pos.z));
	ubo.M = glm::translate(glm::mat4(1.0), pos);
	if (pos.x > 0) {
		ubo.M = glm::rotate(ubo.M, 0.25f * time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	ubo.M = glm::scale(ubo.M, scale);
	ubo.V = cameraHandler->viewMatrix();
	ubo.P = cameraHandler->projMatrix();

	ubo.P[1][1] *= -1;

	vkMapMemory(devicesHandler->device, memories[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);

}
