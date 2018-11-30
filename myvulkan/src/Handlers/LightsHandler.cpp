#include <chrono>
#include <math.h>

#include "Handlers/Handlers.h"
#include "Bindings/bindings.h"
#include "Layouts/light_l.h"
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
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutSingleSpace, nullptr);
}


void LightsHandler::add(glm::vec3 pos, glm::vec3 color) {
	if (lights.size() == 3) {
		std::cout << "Limit of 3 lights reached." << std::endl;
	}
	Light* light = new Light(pos, color);
	lights.push_back(light);
}




void LightsHandler::createDescriptorSetLayoutData() {
	layouts::lights::Properties_PV_Depth(&descriptorSetLayoutData, 0, 1, 1, 1, 2, lights.size());
}


void LightsHandler::createDescriptorSetLayoutModel() {
	layouts::lights::Model(&descriptorSetLayoutModel, 0, 1);
}


void LightsHandler::createDescriptorSetLayoutSpace() {
	layouts::lights::PV(&descriptorSetLayoutSpace, 0, 1);
}


void LightsHandler::createDescriptorSetLayoutSingleSpace() {
	layouts::lights::PV(&descriptorSetLayoutSingleSpace, 0, 1);
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

	for (size_t i = 0; i < lights.size(); i++) {
		Light* light = lights[i];
		float factor = 1.0f;
		if (i == 0) {
			light->pos.y = 2.0f + cos(glm::radians(time * 30.0f)) * 5.0f;
		}
		if (i == 1) {
			light->pos.y = 2.0f - cos(glm::radians(time * 30.0f)) * 5.0f;
		}
		if (i == 2) {
			light->pos.z = 5.0f + cos(glm::radians(time * 60.0f)) * 4.5f;
		}
		
		light->updateProjectionView();

		light->updateUBOs(index);
	}
	for (auto light : lights) {
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

		std::vector<VkDescriptorImageInfo> imageInfos = {};
		imageInfos.resize(lights.size());
		for (size_t j = 0; j < lights.size(); j++) {
			imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			imageInfos[j].imageView = presentation->shadow.imageViews[j];
			imageInfos[j].sampler = presentation->shadow.samplers[j];
		}

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSetsData[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[2].descriptorCount = imageInfos.size();
		descriptorWrites[2].pImageInfo = imageInfos.data();

		vkUpdateDescriptorSets(devicesHandler->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}

void LightsHandler::createDescriptorSetsSingleSpace() {
	for (auto light : lights) {
		light->createDescriptorSetsSpace();
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
