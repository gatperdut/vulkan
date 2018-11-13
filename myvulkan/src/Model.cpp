#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Handlers/Handlers.h"
#include "Model.h"
#include "uniform_buffer_objects.h"


Model::Model(std::string path, std::string filename, std::string textureFilename, glm::vec3 pos) {
	this->filename = filename;
	this->path = path;
	this->pos = pos;

	uboHandler = new UboHandler();
	bufferHandler = new BufferHandler();
	loadTexture(path + textureFilename);
	loadModel();
}


Model::~Model() {
	delete textureAddon;
	delete uboHandler;
	delete bufferHandler;
}


void Model::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials,  &err, (path + filename).c_str(), path.c_str(), true)) {
		throw std::runtime_error(err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	bufferHandler->createBuffers(vertices, indices);
}


void Model::createUBOs() {
	uboHandler->createUniformBuffer(totalSize());
}

void Model::updateUBOs(uint32_t imageIndex) {
	uboHandler->updateUniformBuffer(imageIndex, pos);
}


void Model::loadTexture(std::string path) {
	textureAddon = new TextureAddon(path);

	textureAddon->createTextureImage();
	textureAddon->createTextureImageView();
	textureAddon->createTextureSampler();
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


void Model::createDescriptorSets() {
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorsHandler->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &modelsHandler->descriptorSetLayout;

	descriptorSets.resize(swapchainHandler->images.size());

	for (size_t i = 0; i < swapchainHandler->images.size(); i++) {
		if (vkAllocateDescriptorSets(devicesHandler->device, &allocInfo, &descriptorSets[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate UB descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uboHandler->buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWriteUB = {};
		descriptorWriteUB.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteUB.dstSet = descriptorSets[i];
		descriptorWriteUB.dstBinding = 0;
		descriptorWriteUB.dstArrayElement = 0;
		descriptorWriteUB.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorWriteUB.descriptorCount = 1;
		descriptorWriteUB.pBufferInfo = &bufferInfo;

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureAddon->textureImageView;
		imageInfo.sampler = textureAddon->textureSampler;

		VkWriteDescriptorSet descriptorWriteCIS = {};
		descriptorWriteCIS.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteCIS.dstSet = descriptorSets[i];
		descriptorWriteCIS.dstBinding = 1;
		descriptorWriteCIS.dstArrayElement = 0;
		descriptorWriteCIS.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWriteCIS.descriptorCount = 1;
		descriptorWriteCIS.pImageInfo = &imageInfo;

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = { descriptorWriteUB, descriptorWriteCIS };

		vkUpdateDescriptorSets(devicesHandler->device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
	}
}