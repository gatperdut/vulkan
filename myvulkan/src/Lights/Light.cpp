#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Handlers/Handlers.h"
#include "Descriptors/light_d.h"
#include "Camera/camera.h"
#include "DSets/light_ds.h"
#include "Lights/Light.h"
#include "Vertices/P_v.h"

Light::Light(glm::vec3 pos, glm::vec3 color) {
	this->pos = pos;
	this->color = color;

	loadModel();
}


Light::~Light() {
	uniforms::destroy(u_Attrs);
	uniforms::destroy(u_PVM);
	uniforms::destroy(u_PV);
	vbuffers::destroy(vb_P);
}

void Light::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "models/light/light.obj", "models/light/", true)) {
		throw std::runtime_error(err);
	}

	std::unordered_map<vertices::V_P::Data, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		uint32_t indexCount = 0;
		for (const auto& index : shape.mesh.indices) {
			vertices::V_P::Data vertex = {};

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

	vbuffers::create(vb_P, vertices.data(), vertices.size(), sizeof(vertices::V_P::Data), indices);
}


void Light::createUs() {
	uniforms::create(u_Attrs, presentation->swapchain.images.size(), sizeof(descriptors::lights::Attrs), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	uniforms::create(u_PVM, presentation->swapchain.images.size(), sizeof(descriptors::lights::PVM), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	uniforms::create(u_PV, presentation->swapchain.images.size(), sizeof(descriptors::lights::PV), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void Light::updateUs(uint32_t index) {
	updateU_Attrs(index);
	updateU_PVM(index);
	updateU_PV(index);
}


void Light::updateU_Attrs(uint32_t index) {
	descriptors::lights::Attrs Attrs;

	Attrs.pos = glm::vec4(pos, 1.0);
	Attrs.color = glm::vec4(color, 1.0);

	uniforms::update(u_Attrs, index, sizeof(descriptors::lights::Attrs), &Attrs);
}

void Light::updateU_PVM(uint32_t index) {
	descriptors::lights::PVM PVM = {};

	PVM.P = camera::P();
	PVM.V = camera::V();
	PVM.M = glm::translate(glm::mat4(1.0), pos);

	PVM.P[1][1] *= -1;

	uniforms::update(u_PVM, index, sizeof(descriptors::lights::PVM), &PVM);
}

void Light::updateU_PV(uint32_t index) {
	descriptors::lights::PV PV;

	PV.PV = projectionView;

	uniforms::update(u_PV, index, sizeof(descriptors::lights::PV), &PV);
}

void Light::createDS_Attrs_PVM() {
	ds_Attrs_PVM.resize(presentation->swapchain.images.size());
	dsets::lights::Attrs_PVM(ds_Attrs_PVM, &lightsHandler->dsl_Attrs_PVM, u_Attrs, u_PVM);
}

void Light::createDS_PV() {
	ds_PV.resize(presentation->swapchain.images.size());
	dsets::lights::singlePV(ds_PV, &lightsHandler->dsl_PV, u_PV);
}


void Light::updateProjectionView() {
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 40.0f);
	//glm::mat4 projection = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 40.0f);

	glm::vec3 eye = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::mat4 view = glm::lookAt(pos, eye, glm::vec3(0.0f, 1.0f, 0.0f));

	projectionView = projection * view;
}
