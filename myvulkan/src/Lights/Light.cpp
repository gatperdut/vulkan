#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Handlers/Handlers.h"
#include "Lights/Light.h"
#include "Lights/light_model_ubo.h"
#include "Lights/light_space_ubo.h"


Light::Light(glm::vec3 pos, glm::vec3 color) {
	this->pos = pos;
	this->color = color;

	lightModelUBOs = new LightModelUBOs;
	singleLightSpaceUBOs = new SingleLightSpaceUBOs;
	lightVBOs = new LightVBOs;
	loadModel();
}


Light::~Light() {
	delete lightModelUBOs;
	delete singleLightSpaceUBOs;
	delete lightVBOs;
}

void Light::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "models/light/light.obj", "models/light/", true)) {
		throw std::runtime_error(err);
	}

	std::unordered_map<LightVertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		uint32_t indexCount = 0;
		for (const auto& index : shape.mesh.indices) {
			LightVertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);

			indexCount++;
		}
		indexCount = 0;
	}

	lightVBOs->createBuffers(vertices, indices);
}


void Light::createUBOs() {
	lightModelUBOs->createUniformBuffers();
	singleLightSpaceUBOs->createUniformBuffers();
}

void Light::updateUBOs(uint32_t index) {
	singleLightSpaceUBOs->updateUniformBuffer(index, projectionView);
	lightModelUBOs->updateUniformBuffer(index, pos);
}


void Light::createDescriptorSetsModel() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &lightsHandler->descriptorSetLayoutModel;

	descriptorSetsModel.resize(presentation->swapchain.images.size());

	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSetsModel[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = lightModelUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(LightModelUBO);


		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSetsModel[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWrite, 0, nullptr);
	}
}

void Light::createDescriptorSetsSpace() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &lightsHandler->descriptorSetLayoutSingleSpace;

	descriptorSetsSpace.resize(presentation->swapchain.images.size());

	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSetsSpace[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate Space descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = singleLightSpaceUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(LightSpaceUBO);


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


void Light::updateProjectionView() {
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 40.0f);
	//glm::mat4 projection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 40.0f);

	glm::vec3 eye = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::mat4 view = glm::lookAt(pos, eye, glm::vec3(0.0f, 1.0f, 0.0f));

	projectionView = projection * view;
}
