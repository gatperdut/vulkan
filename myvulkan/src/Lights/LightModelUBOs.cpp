#include <glm/gtc/matrix_transform.hpp>

#include "Handlers/Handlers.h"
#include "Lights/LightModelUBOs.h"
#include "Descriptors/light_d.h"
#include "buffers.h"

LightModelUBOs::LightModelUBOs() {

}


LightModelUBOs::~LightModelUBOs() {
	freeResources();
}

void LightModelUBOs::freeResources() {
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void LightModelUBOs::internalCreateUniformBuffers(std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	VkDeviceSize size = lightsHandler->lights.size() * sizeof(descriptors::lights::PVM);

	(*buffers).resize(presentation->swapchain.images.size());
	(*buffersMemories).resize(presentation->swapchain.images.size());
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
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
		for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
			buffers[i] = newBuffers[i];
			memories[i] = newMemories[i];
		}
	}
}


void LightModelUBOs::updateUniformBuffer(uint32_t currentImage, glm::vec3 pos) {
	void* data;

	descriptors::lights::PVM ubo = {};
	
	ubo.M = glm::translate(glm::mat4(1.0), pos);
	ubo.V = cameraHandler->viewMatrix();
	ubo.P = cameraHandler->projMatrix();

	ubo.P[1][1] *= -1;

	vkMapMemory(devicesHandler->device, memories[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);

}
