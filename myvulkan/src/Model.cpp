#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Model.h"


Model::Model(std::string path, std::string filename, glm::vec3 pos) {
	this->path = path;
	this->filename = filename;
	this->pos = pos;

	texturesHandler = new TexturesHandler(path);

	load();

	texturesHandler->createDescriptorSetLayout();
}


Model::~Model() {
	delete texturesHandler;
}


void Model::load() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials,  &err, (path + filename).c_str(), path.c_str(), true)) {
		throw std::runtime_error(err);
	}
	std::cout << err << std::endl;

	for (tinyobj::material_t material : materials) {
		if (!texturesHandler->hasTexture(material.ambient_texname)) {
			std::cout << "Texture used: " << material.ambient_texname << std::endl;
			texturesHandler->addTexture(material.ambient_texname);
		};
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		uint32_t indexCount = 0;
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.materialIndex = texturesHandler->textureIndex(materials[shape.mesh.material_ids[indexCount / 3]].ambient_texname);

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

			indexCount++;
		}
	}
}


void Model::addTexture(std::string filename) {
	texturesHandler->addTexture(filename);
}


VkDeviceSize Model::verticesSize() {
	return sizeof(vertices[0]) * vertices.size();
}


VkDeviceSize Model::indicesSize() {
	return sizeof(indices[0]) * indices.size();
}