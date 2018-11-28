#include <glm/gtc/matrix_transform.hpp>

#include "Handlers/Handlers.h"
#include "Lights/SingleLightSpaceUBOs.h"
#include "Lights/light_space_ubo.h"
#include "buffers.h"

SingleLightSpaceUBOs::SingleLightSpaceUBOs() {

}


SingleLightSpaceUBOs::~SingleLightSpaceUBOs() {
	freeResources();
}

void SingleLightSpaceUBOs::freeResources() {
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void SingleLightSpaceUBOs::internalCreateUniformBuffers(std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	VkDeviceSize size = sizeof(LightSpaceUBO);

	(*buffers).resize(presentation->swapchain.images.size());
	(*buffersMemories).resize(presentation->swapchain.images.size());
	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void SingleLightSpaceUBOs::createUniformBuffers() {
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


void SingleLightSpaceUBOs::updateUniformBuffer(uint32_t currentImage, glm::mat4 projectionView) {
	void* data;

	std::vector<LightSpaceUBO> ubo;
	ubo.resize(1);

	ubo[0].projectionView = projectionView;

	vkMapMemory(devicesHandler->device, memories[currentImage], 0, sizeof(LightSpaceUBO), 0, &data);
	memcpy(data, ubo.data(), sizeof(LightSpaceUBO));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);
}
