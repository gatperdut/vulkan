#include <glm/gtc/matrix_transform.hpp>

#include "Handlers/Handlers.h"
#include "Lights/LightDataUBOs.h"
#include "Lights/light_data_ubo.h"
#include "buffers.h"

LightDataUBOs::LightDataUBOs() {

}


LightDataUBOs::~LightDataUBOs() {
	freeResources();
}

void LightDataUBOs::freeResources() {
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void LightDataUBOs::internalCreateUniformBuffers(std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	VkDeviceSize size = lightsHandler->lights.size() * sizeof(LightDataUBO) * 2;

	(*buffers).resize(presentation->swapchain.images.size());
	(*buffersMemories).resize(presentation->swapchain.images.size());
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void LightDataUBOs::createUniformBuffers() {
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


void LightDataUBOs::updateDataUBO(uint32_t currentImage) {
	void* data;

	std::vector<LightDataUBO> ubos;
	ubos.resize(lightsHandler->lights.size());
	for (uint32_t i = 0; i < lightsHandler->lights.size(); i++) {
		ubos[i].pos = glm::vec4(lightsHandler->lights[i]->pos, 1.0);
		ubos[i].color = glm::vec4(lightsHandler->lights[i]->color, 1.0);
	}
	
	vkMapMemory(devicesHandler->device, memories[currentImage], 0, lightsHandler->lights.size() * sizeof(LightDataUBO), 0, &data);
	memcpy(data, ubos.data(), lightsHandler->lights.size() * sizeof(LightDataUBO));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);

}
