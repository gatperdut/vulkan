#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Handlers/Handlers.h"
#include "Descriptors/light_d.h"
#include "DSets/light_ds.h"
#include "Lights/Light.h"


Light::Light(glm::vec3 pos, glm::vec3 color) {
	this->pos = pos;
	this->color = color;

	lightModelUBOs = new LightModelUBOs;
	singleLightSpaceUBOs = new SingleLightSpaceUBOs;
	lightVBOs = new LightVBOs;
	loadModel();
}


Light::~Light() {
	uniforms::destroy(Attrs_u);
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
	uniforms::create(Attrs_u, presentation->swapchain.images.size(), sizeof(descriptors::lights::Attrs), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	lightModelUBOs->createUniformBuffers();
	singleLightSpaceUBOs->createUniformBuffers();
}

void Light::updateUBOs(uint32_t index) {
	update_Attrs_u(index);
	lightModelUBOs->updateUniformBuffer(index, pos);
	singleLightSpaceUBOs->updateUniformBuffer(index, projectionView);
}


void Light::update_Attrs_u(uint32_t index) {
	descriptors::lights::Attrs Attrs;

	Attrs.pos = glm::vec4(pos, 1.0);
	Attrs.color = glm::vec4(color, 1.0);

	uniforms::update(Attrs_u, index, sizeof(descriptors::lights::Attrs), &Attrs);
}


void Light::createDescriptorSetsModel() {
	dsets_Attrs_PVM.resize(presentation->swapchain.images.size());
	dsets::lights::Attrs_PVM(dsets_Attrs_PVM, &lightsHandler->dsl_Attrs_PVM, Attrs_u, lightModelUBOs);
}

void Light::createDescriptorSetsSpace() {
	dsets_singlePV.resize(presentation->swapchain.images.size());
	dsets::lights::singlePV(dsets_singlePV, &lightsHandler->descriptorSetLayoutSingleSpace, singleLightSpaceUBOs);
}


void Light::updateProjectionView() {
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 40.0f);
	//glm::mat4 projection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 40.0f);

	glm::vec3 eye = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::mat4 view = glm::lookAt(pos, eye, glm::vec3(0.0f, 1.0f, 0.0f));

	projectionView = projection * view;
}
