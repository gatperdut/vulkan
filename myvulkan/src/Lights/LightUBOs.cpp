#include <glm/gtc/matrix_transform.hpp>
#include <chrono>


#include "Handlers/Handlers.h"
#include "Lights/LightUBOs.h"
#include "Lights/light_ubo.h"
#include "buffers.h"

LightUBOs::LightUBOs() {

}


LightUBOs::~LightUBOs() {
	freeResources();
}

void LightUBOs::freeResources() {
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void LightUBOs::internalCreateUniformBuffers(std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	VkDeviceSize size = lightsHandler->lights.size() * sizeof(LightUbo);

	(*buffers).resize(swapchainHandler->images.size());
	(*buffersMemories).resize(swapchainHandler->images.size());
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void LightUBOs::createUniformBuffers() {

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


void LightUBOs::updateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	void* data;

	std::vector<LightUbo> ubos;
	ubos.resize(lightsHandler->lights.size());
	for (uint32_t i = 0; i < lightsHandler->lights.size(); i++) {
		ubos[i].pos = lightsHandler->lights[i]->pos;
	}
	
	vkMapMemory(devicesHandler->device, memories[currentImage], 0, lightsHandler->lights.size() * sizeof(LightUbo), 0, &data);
	memcpy(data, ubos.data(), lightsHandler->lights.size() * sizeof(LightUbo));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);

}


VkDescriptorSetLayoutBinding LightUBOs::createDescriptorSetLayoutBinding() {
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = 0;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	return layoutBinding;
}