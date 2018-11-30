#include <tiny_obj_loader.h>

#include "Models/Model.h"
#include "Handlers/Handlers.h"
#include "Models/model_ubo.h"
#include "Lights/light_model_ubo.h"
#include "Layouts/model_l.h"


Model::Model(std::string path, std::string filename, glm::vec3 pos, glm::vec3 scale) {
	this->filename = filename;
	this->path = path;
	this->pos = pos;
	this->scale = scale;

	modelUBOs = new ModelUBOs;
	modelVBOs = new ModelVBOs;
	shadowVBOs = new ShadowVBOs;
	modelMaterials = new ModelMaterials;
	modelPipeline = new ModelPipeline;
	loadModel();
}


Model::~Model() {
	delete modelMaterials;
	delete modelUBOs;
	delete modelVBOs;
	delete shadowVBOs;
	delete modelPipeline;

	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayout, nullptr);
}


void Model::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (path + filename).c_str(), path.c_str(), true)) {
		throw std::runtime_error(err);
	}


	std::unordered_map<ModelVertex, uint32_t> uniqueVertices = {};

	for (auto material : materials) {
		if (!material.diffuse_texname.empty() && !modelMaterials->hasTexture(path + material.diffuse_texname)) {
			std::cout << "Loading texture " << path + material.diffuse_texname << std::endl;
			modelMaterials->addTexture(path + material.diffuse_texname);
		}
	}

	for (const auto& shape : shapes) {
		uint32_t indexCount = 0;
		for (const auto& index : shape.mesh.indices) {
			ModelVertex vertex = {};
			ShadowVertex shadowVertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			shadowVertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			if (!attrib.texcoords.empty()) {
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertex.texIndex = modelMaterials->indexByFilepath(path + materials[shape.mesh.material_ids[indexCount / 3]].diffuse_texname);

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
				verticesShadow.push_back(shadowVertex);
			}

			indices.push_back(uniqueVertices[vertex]);

			indexCount++;
		}
		indexCount = 0;
	}

	modelVBOs->createBuffers(vertices, indices);
	shadowVBOs->createBuffers(verticesShadow, indices);
}


void Model::createUBOs() {
	modelUBOs->createUniformBuffers(totalSize());
}

void Model::updateUBO(uint32_t imageIndex) {
	modelUBOs->updateUniformBuffer(imageIndex, pos, scale);
}


VkDeviceSize Model::verticesSize() {
	return sizeof(vertices[0]) * vertices.size();
}


VkDeviceSize Model::indicesSize() {
	return sizeof(indices[0]) * indices.size();
}

VkDeviceSize Model::totalSize() {
	return verticesSize() + indicesSize();
}


void Model::createDescriptorSetLayout() {
	layouts::models::PVM_Materials(&descriptorSetLayout, 0, 1, 1, modelMaterials->filepaths.size());
}


void Model::createDescriptorSets() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;

	descriptorSets.resize(presentation->swapchain.images.size());

	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSets[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = modelUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(ModelUBO);


		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(2);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		std::vector<VkDescriptorImageInfo> imageInfos;
		imageInfos.resize(modelMaterials->filepaths.size());
		for (size_t j = 0; j < modelMaterials->filepaths.size(); j++) {
			imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfos[j].imageView = modelMaterials->imageViews[j];
			imageInfos[j].sampler = modelMaterials->samplers[j];
		}
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = imageInfos.size();
		descriptorWrites[1].pImageInfo = imageInfos.data();

		vkUpdateDescriptorSets(devicesHandler->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}


void Model::createDescriptorSetsMatrices() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &modelsHandler->descriptorSetLayoutMatrices;

	descriptorSetsMatrices.resize(presentation->swapchain.images.size());

	for (size_t i = 0; i < presentation->swapchain.images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSetsMatrices[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = modelUBOs->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(ModelUBO);


		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSetsMatrices[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(devicesHandler->device, 1, &descriptorWrite, 0, nullptr);
	}
}


void Model::createPipeline() {
	modelPipeline->create(descriptorSetLayout);
}