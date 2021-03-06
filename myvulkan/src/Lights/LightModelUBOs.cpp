#include <glm/gtc/matrix_transform.hpp>

#include "Handlers/Handlers.h"
#include "Lights/LightModelUBOs.h"
#include "Lights/light_model_ubo.h"
#include "buffers.h"

LightModelUBOs::LightModelUBOs() {

}


LightModelUBOs::~LightModelUBOs() {
	freeResources();
}

void LightModelUBOs::freeResources() {
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void LightModelUBOs::internalCreateUniformBuffers(std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	VkDeviceSize size = lightsHandler->lights.size() * sizeof(LightModelUBO);

	(*buffers).resize(swapchainHandler->images.size());
	(*buffersMemories).resize(swapchainHandler->images.size());
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void LightModelUBOs::createUniformBuffers() {
	if (!buffers.size()) {
		internalCreateUniformBuffers(&buffers, &memories);
	}
	else {
		std::vector<VkBuffer> newBuffers;
		std::vector<VkDeviceMemory> newMemories;

		internalCreateUniformBuffers(&newBuffers, &newMemories);

		freeResources();
		for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
			buffers[i] = newBuffers[i];
			memories[i] = newMemories[i];
		}
	}
}


void LightModelUBOs::updateUniformBuffer(uint32_t currentImage, glm::vec3 pos) {
	void* data;

	LightModelUBO ubo = {};
	
	ubo.model = glm::translate(glm::mat4(1.0), pos);
	ubo.view = cameraHandler->viewMatrix();
	ubo.proj = glm::perspective(glm::radians(45.0f), swapchainHandler->extent.width / (float)swapchainHandler->extent.height, 0.1f, 100.0f);

	ubo.proj[1][1] *= -1;

	vkMapMemory(devicesHandler->device, memories[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);

}
