#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Handlers/Handlers.h"
#include "Lights/LightSpaceUBOs.h"
#include "Lights/light_space_ubo.h"
#include "buffers.h"

LightSpaceUBOs::LightSpaceUBOs() {

}


LightSpaceUBOs::~LightSpaceUBOs() {
	freeResources();
}

void LightSpaceUBOs::freeResources() {
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void LightSpaceUBOs::internalCreateUniformBuffers(std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	VkDeviceSize size = lightsHandler->lights.size() * sizeof(LightSpaceUBO);

	(*buffers).resize(presentation->swapchain.images.size());
	(*buffersMemories).resize(presentation->swapchain.images.size());
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void LightSpaceUBOs::createUniformBuffers() {
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


void LightSpaceUBOs::updateUniformBuffer(uint32_t currentImage) {
	void* data;
	size_t numLights = lightsHandler->lights.size();

	std::vector<LightSpaceUBO> ubos;
	ubos.resize(numLights);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1.0f);

	for (uint32_t i = 0; i < numLights; i++) {
		glm::mat4 view = glm::lookAt(lightsHandler->lights[i]->pos, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubos[i].projectionView = projection * view * model;
	}

	vkMapMemory(devicesHandler->device, memories[currentImage], 0, numLights * sizeof(LightSpaceUBO), 0, &data);
	memcpy(data, ubos.data(), numLights * sizeof(LightSpaceUBO));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);
}
