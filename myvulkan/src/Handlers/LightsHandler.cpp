#include <chrono>
#include <math.h>

#include "Handlers/Handlers.h"
#include "Lights/light_ubo.h"


LightsHandler::LightsHandler() {
	lightDataUBOs = new LightDataUBOs;
	lightPipeline = new LightPipeline;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	delete lightDataUBOs;
	delete lightPipeline;
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutData, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutModel, nullptr);
}


void LightsHandler::add(glm::vec3 pos, glm::vec3 color) {
	Light* light = new Light(pos, color);
	lights.push_back(light);
}


VkDescriptorSetLayoutBinding LightsHandler::createDescriptorSetLayoutModelBinding() {
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = 0;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	return layoutBinding;
}


void LightsHandler::createDescriptorSetLayoutData() {
	VkDescriptorSetLayoutBinding layoutBinding = lightDataUBOs->createDescriptorSetLayoutBinding();

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, &descriptorSetLayoutData) != VK_SUCCESS) {
		throw std::runtime_error("failed to create light data descriptor set layout!");
	}
}


void LightsHandler::createDescriptorSetLayoutModel() {
	VkDescriptorSetLayoutBinding layoutBinding = createDescriptorSetLayoutModelBinding();

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, &descriptorSetLayoutModel) != VK_SUCCESS) {
		throw std::runtime_error("failed to create light model descriptor set layout!");
	}
}

void LightsHandler::createDataUBOs() {
	lightDataUBOs->createUniformBuffers();
}


void LightsHandler::createModelUBOs() {
	for (auto light : lights) {
		light->createModelUBOs();
	}
}


void LightsHandler::updateUBOs(uint32_t index) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	for (auto light : lights) {
		light->pos.z = 0.03 * light->pos.z + sin(time) * 8 - 4.0f;
		light->updateModelUBO(index);
	}
	lightDataUBOs->updateUniformBuffer(index);
}


void LightsHandler::createDescriptorSetsData() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayoutData;

	descriptorSetsData.resize(swapchainHandler->images.size());

	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSetsData[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = lightDataUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(LightUbo);

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSetsData[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWrite, 0, nullptr);
	}
}


void LightsHandler::createDescriptorSetsModel() {
	for (auto light : lights) {
		light->createDescriptorSetsModel();
	}
}

void LightsHandler::createPipeline() {
	lightPipeline->create(descriptorSetLayoutModel);
}
