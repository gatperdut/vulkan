#include <chrono>
#include <math.h>

#include "Handlers/Handlers.h"
#include "Lights/light_data_ubo.h"
#include "Lights/light_space_ubo.h"


LightsHandler::LightsHandler() {
	lightDataUBOs = new LightDataUBOs;
	lightSpaceUBOs = new LightSpaceUBOs;
	lightPipeline = new LightPipeline;
	shadowPipeline = new ShadowPipeline;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	delete lightDataUBOs;
	delete lightSpaceUBOs;
	delete lightPipeline;
	delete shadowPipeline;
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutData, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutModel, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutSpace, nullptr);
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


VkDescriptorSetLayoutBinding LightsHandler::createDescriptorSetLayoutSpaceBinding(uint32_t binding) {
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = binding;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	return layoutBinding;
}


void LightsHandler::createDescriptorSetLayoutData() {
	VkDescriptorSetLayoutBinding layoutBindingUB = lightDataUBOs->createDescriptorSetLayoutBinding();
	
	VkDescriptorSetLayoutBinding layoutBindingSpace = createDescriptorSetLayoutSpaceBinding(1);

	VkDescriptorSetLayoutBinding layoutBindingShadow = {};
	layoutBindingShadow.binding = 2;
	layoutBindingShadow.descriptorCount = 1;
	layoutBindingShadow.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutBindingShadow.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::vector<VkDescriptorSetLayoutBinding> bindings = { layoutBindingUB, layoutBindingSpace, layoutBindingShadow };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = bindings.size();
	layoutInfo.pBindings = bindings.data();

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


void LightsHandler::createDescriptorSetLayoutSpace() {
	VkDescriptorSetLayoutBinding layoutBinding = createDescriptorSetLayoutSpaceBinding(0);

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, &descriptorSetLayoutSpace) != VK_SUCCESS) {
		throw std::runtime_error("failed to create light model descriptor set layout!");
	}
}


void LightsHandler::createUBOs() {
	lightDataUBOs->createUniformBuffers();
	lightSpaceUBOs->createUniformBuffers();
	for (auto light : lights) {
		light->createUBOs();
	}
}


void LightsHandler::updateUBOs(uint32_t index) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	for (auto light : lights) {
		light->pos.x = sin(glm::radians(time * 50.0f)) * 10.0f;
		light->updateUBOs(index);
	}
	lightDataUBOs->updateDataUBO(index);
	lightSpaceUBOs->updateUniformBuffer(index);
}


void LightsHandler::createDescriptorSetsData() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayoutData;

	descriptorSetsData.resize(presentation->swapchain.images.size());

	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSetsData[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(3);

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = lightDataUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = lights.size() * sizeof(LightDataUBO);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSetsData[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		VkDescriptorBufferInfo bufferInfoSpace = {};
		bufferInfoSpace.buffer = lightSpaceUBOs->buffers[i];
		bufferInfoSpace.offset = 0;
		bufferInfoSpace.range = lights.size() * sizeof(LightSpaceUBO);

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSetsData[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &bufferInfoSpace;

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		imageInfo.imageView = presentation->shadow.imageView;
		imageInfo.sampler = presentation->shadow.sampler;

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSetsData[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(devicesHandler->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}

void LightsHandler::createDescriptorSetsSpace() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayoutSpace;

	descriptorSetsSpace.resize(presentation->swapchain.images.size());

	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSetsSpace[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate space descriptor sets!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = lightSpaceUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = lights.size() * sizeof(LightSpaceUBO);

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSetsSpace[i];
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

void LightsHandler::createPipelines() {
	lightPipeline->create(descriptorSetLayoutModel);
	shadowPipeline->create();
}
