#include <iostream>
#include <tiny_obj_loader.h>

#include "Models/Model.h"
#include "Handlers/Handlers.h"
#include "Camera/camera.h"
#include "Devices/logical.h"
#include "Descriptors/model_d.h"
#include "Descriptors/light_d.h"
#include "Layouts/model_l.h"
#include "Writes/create_w.h"
#include "Writes/Info/create_wi.h"
#include "DSets/model_ds.h"
#include "Timer/timer.h"


Model::Model(std::string path, std::string filename, glm::vec3 pos, glm::vec3 scale) {
	this->filename = filename;
	this->path = path;
	this->pos = pos;
	this->scale = scale;

	modelMaterials = new ModelMaterials;
	modelPipeline = new ModelPipeline;
	loadModel();
}


Model::~Model() {
	delete modelMaterials;
	uniforms::destroy(u_PVM);
	vbuffers::destroy(vb_P_N_C_TXC_TXI);
	vbuffers::destroy(vb_P);
	delete modelPipeline;

	vkDestroyDescriptorSetLayout(devices::logical::dev, dsl_PVM_Materials, nullptr);
}


void Model::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (path + filename).c_str(), path.c_str(), true)) {
		throw std::runtime_error(err);
	}


	std::unordered_map<vertices::V_P_N_C_TXC_TXI::Data, uint32_t> uniqueVertices = {};

	for (auto material : materials) {
		if (!material.diffuse_texname.empty() && !modelMaterials->hasTexture(path + material.diffuse_texname)) {
			std::cout << "Loading texture " << path + material.diffuse_texname << std::endl;
			modelMaterials->addTexture(path + material.diffuse_texname);
		}
	}

	for (const auto& shape : shapes) {
		uint32_t indexCount = 0;
		for (const auto& index : shape.mesh.indices) {
			vertices::V_P_N_C_TXC_TXI::Data vertex = {};
			vertices::V_P::Data shadowVertex = {};

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

	vbuffers::create(vb_P_N_C_TXC_TXI, vertices.data(), vertices.size(), sizeof(vertices::V_P_N_C_TXC_TXI::Data), indices);
	vbuffers::create(vb_P, verticesShadow.data(), verticesShadow.size(), sizeof(vertices::V_P::Data), indices);
}


void Model::createUs() {
	uniforms::create(u_PVM, presentation->swapchain.images.size(), totalSize(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void Model::updateU_PVM(uint32_t index) {
	float time = timer::lapse();

	descriptors::models::PVM PVM = {};
	PVM.M = glm::translate(glm::mat4(1.0), pos);
	if (pos.x > 0) {
		PVM.M = glm::rotate(PVM.M, 0.25f * time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	PVM.M = glm::scale(PVM.M, scale);
	PVM.V = camera::V();
	PVM.P = camera::P();

	PVM.P[1][1] *= -1;

	uniforms::update(u_PVM, index, sizeof(descriptors::models::PVM), (void*)&PVM);
}


size_t Model::verticesSize() {
	return sizeof(vertices[0]) * vertices.size();
}


size_t Model::indicesSize() {
	return sizeof(indices[0]) * indices.size();
}

size_t Model::totalSize() {
	return verticesSize() + indicesSize();
}


void Model::createDSL_PVM_Materials() {
	layouts::models::PVM_Materials(&dsl_PVM_Materials, 0, 1, 1, modelMaterials->filepaths.size());
}


void Model::createDS_PVM_Materials() {
	ds_PVM_Materials.resize(presentation->swapchain.images.size());
	dsets::models::PVM_Materials(ds_PVM_Materials, &dsl_PVM_Materials, u_PVM, modelMaterials);
}


void Model::createDS_PVM() {
	ds_PVM.resize(presentation->swapchain.images.size());
	dsets::models::PVM(ds_PVM, &modelsHandler->dsl_PVM, u_PVM);
}


void Model::createPipeline() {
	modelPipeline->create(dsl_PVM_Materials);
}