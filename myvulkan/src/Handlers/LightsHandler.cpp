#include "Handlers/Handlers.h"
#include "light_ubo.h"


LightsHandler::LightsHandler() {
	lightUboHandler = new LightUboHandler;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	delete lightUboHandler;
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayout, nullptr);
}


void LightsHandler::add(glm::vec3 pos) {
	Light* light = new Light(pos);
	lights.push_back(light);
}

void LightsHandler::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding layoutBinding = lightUboHandler->createDescriptorSetLayoutBinding();

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create CIS descriptor set layout!");
	}
}

void LightsHandler::createUBOs() {
	lightUboHandler->createUniformBuffers();
}

void LightsHandler::updateUBO(uint32_t index) {
	lightUboHandler->updateUniformBuffer(index);
}

void LightsHandler::createDescriptorSets() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;

	descriptorSets.resize(swapchainHandler->images.size());

	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSets[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = lightUboHandler->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(LightUbo);

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWrite, 0, nullptr);
	}
}
