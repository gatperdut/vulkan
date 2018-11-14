#include <glm/gtc/matrix_transform.hpp>
#include <chrono>


#include "Handlers/Handlers.h"
#include "Addons/UboHandler.h"
#include "uniform_buffer_objects.h"
#include "buffers.h"

UboHandler::UboHandler() {

}


UboHandler::~UboHandler() {
	freeResources();
}

void UboHandler::freeResources() {
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		vkDestroyBuffer(devicesHandler->device, buffers[i], nullptr);
		vkFreeMemory(devicesHandler->device, memories[i], nullptr);
	}
}


void UboHandler::internalCreateUniformBuffer(VkDeviceSize size, std::vector<VkBuffer>* buffers, std::vector<VkDeviceMemory>* buffersMemories) {
	(*buffers).resize(swapchainHandler->images.size());
	(*buffersMemories).resize(swapchainHandler->images.size());
	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, (*buffers)[i], (*buffersMemories)[i]);
	}
}

void UboHandler::createUniformBuffer(VkDeviceSize size) {
	if (!buffers.size()) {
		internalCreateUniformBuffer(size, &buffers, &memories);
	}
	else {
		std::vector<VkBuffer> newBuffers;
		std::vector<VkDeviceMemory> newMemories;

		internalCreateUniformBuffer(size, &newBuffers, &newMemories);

		freeResources();
		for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
			buffers[i] = newBuffers[i];
			memories[i] = newMemories[i];
		}
	}
}


void UboHandler::updateUniformBuffer(uint32_t currentImage, glm::vec3 pos) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	void* data;
	
	UniformBufferObject ubo = {};
	ubo.model = glm::translate(glm::mat4(1.0), pos);
	ubo.model = glm::rotate(ubo.model, 0.03f * time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ubo.view = cameraHandler->viewMatrix();
	ubo.proj = glm::perspective(glm::radians(45.0f), swapchainHandler->extent.width / (float)swapchainHandler->extent.height, 0.1f, 100.0f);

	ubo.proj[1][1] *= -1;

	vkMapMemory(devicesHandler->device, memories[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(devicesHandler->device, memories[currentImage]);

}


VkDescriptorSetLayoutBinding UboHandler::createDescriptorSetLayoutBinding() {
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = 0;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	return layoutBinding;
}