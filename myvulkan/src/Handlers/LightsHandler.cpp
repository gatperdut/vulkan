#include <chrono>
#include <math.h>

#include "Handlers/Handlers.h"
#include "Lights/light_data_ubo.h"


LightsHandler::LightsHandler() {
	lightDataUBOs = new LightDataUBOs;
	lightPipeline = new LightPipeline;
	shadowPipeline = new ShadowPipeline;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	delete lightDataUBOs;
	delete lightPipeline;
	delete shadowPipeline;
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
	VkDescriptorSetLayoutBinding layoutBindingUB = lightDataUBOs->createDescriptorSetLayoutBinding();
	
	VkDescriptorSetLayoutBinding layoutBindingShadow = {};
	layoutBindingShadow.binding = 1;
	layoutBindingShadow.descriptorCount = 1;
	layoutBindingShadow.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutBindingShadow.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::vector<VkDescriptorSetLayoutBinding> bindings = { layoutBindingUB, layoutBindingShadow };

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
	lightDataUBOs->updateDataUBO(index);
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

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = lightDataUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = lights.size() * sizeof(LightDataUBO);

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(2);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSetsData[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		VkDescriptorImageInfo imageInfo;
		//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		imageInfo.imageView = presentation->shadow.imageView;
		imageInfo.sampler = presentation->shadow.sampler;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSetsData[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(devicesHandler->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
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
