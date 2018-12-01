#include <chrono>
#include <math.h>

#include "Handlers/Handlers.h"
#include "Descriptors/light_d.h"
#include "Bindings/bindings.h"
#include "DSets/light_ds.h"
#include "Layouts/light_l.h"
#include "Timer/timer.h"


LightsHandler::LightsHandler() {
	lightPipeline = new LightPipeline;
	shadowPipeline = new ShadowPipeline;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	uniforms::destroy(Attrs_u);
	uniforms::destroy(PV_u);
	delete lightPipeline;
	delete shadowPipeline;
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutData, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, dsl_Attrs_PVM, nullptr);
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


void LightsHandler::createDSLs() {
	layouts::lights::Attrs_PV_Depth(&descriptorSetLayoutData, 0, 1, 1, 1, 2, lights.size());

	layouts::lights::Attrs_PVM(&dsl_Attrs_PVM, 0, 1, 1, 1);

	layouts::lights::PV(&descriptorSetLayoutSpace, 0, 1);

	layouts::lights::PV(&descriptorSetLayoutSingleSpace, 0, 1);
}

void LightsHandler::createUBOs() {
	uniforms::create(Attrs_u, presentation->swapchain.images.size(), lights.size() * sizeof(descriptors::lights::Attrs), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	uniforms::create(PV_u, presentation->swapchain.images.size(), lights.size() * sizeof(descriptors::lights::PV), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	for (auto light : lights) {
		light->createUBOs();
	}
}


void LightsHandler::updateUBOs(uint32_t index) {
	float time = timer::lapse();

	for (size_t i = 0; i < lights.size(); i++) {
		Light* light = lights[i];
		
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

	update_Attrs_u(index);
	update_PV_u(index);
}

void LightsHandler::update_Attrs_u(uint32_t index) {
	uint32_t nLights = lights.size();

	std::vector<descriptors::lights::Attrs> Attrs;
	Attrs.resize(nLights);

	for (uint32_t i = 0; i < nLights; i++) {
		Attrs[i].pos = glm::vec4(lights[i]->pos, 1.0);
		Attrs[i].color = glm::vec4(lights[i]->color, 1.0);
	}

	uniforms::update(Attrs_u, index, nLights * sizeof(descriptors::lights::Attrs), Attrs.data());
}

void LightsHandler::update_PV_u(uint32_t index) {
	size_t nLights = lightsHandler->lights.size();

	std::vector<descriptors::lights::PV> PVs;
	PVs.resize(nLights);

	for (uint32_t i = 0; i < nLights; i++) {
		PVs[i].PV = lights[i]->projectionView;
	}

	uniforms::update(PV_u, index, nLights * sizeof(descriptors::lights::PV), PVs.data());
}

void LightsHandler::createDS_Attrs_PV_Depth() {
	dsets_Attrs_PV_Depth.resize(presentation->swapchain.images.size());
	dsets::lights::Attrs_PV_Depth(dsets_Attrs_PV_Depth, &descriptorSetLayoutData, Attrs_u, PV_u);
}

void LightsHandler::createDescriptorSetsSingleSpace() {
	for (auto light : lights) {
		light->createDescriptorSetsSpace();
	}
}

void LightsHandler::createDS_multiPV() {
	dsets_multiPV.resize(presentation->swapchain.images.size());
	dsets::lights::multiPV(dsets_multiPV, &descriptorSetLayoutSpace, PV_u);
}


void LightsHandler::createDescriptorSetsModel() {
	for (auto light : lights) {
		light->createDescriptorSetsModel();
	}
}

void LightsHandler::createPipelines() {
	lightPipeline->create(dsl_Attrs_PVM);
	shadowPipeline->create();
}
