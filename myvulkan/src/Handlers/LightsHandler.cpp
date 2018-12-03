#include <chrono>
#include <math.h>
#include <iostream>

#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "Pipelines/Types/light_pt.h"
#include "Descriptors/light_d.h"
#include "Bindings/bindings.h"
#include "DSets/light_ds.h"
#include "Layouts/light_l.h"
#include "Timer/timer.h"


LightsHandler::LightsHandler() {
	shadowPipeline = new ShadowPipeline;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	uniforms::destroy(u_Attrs);
	uniforms::destroy(u_PV);
	pipelines::destroy(pipeline);
	delete shadowPipeline;
	vkDestroyDescriptorSetLayout(devices::logical::dev, dsl_Attrs, nullptr);
	vkDestroyDescriptorSetLayout(devices::logical::dev, dsl_Attrs_PVM, nullptr);
	vkDestroyDescriptorSetLayout(devices::logical::dev, dsl_PVs, nullptr);
	vkDestroyDescriptorSetLayout(devices::logical::dev, dsl_PV, nullptr);
}


void LightsHandler::add(glm::vec3 pos, glm::vec3 color) {
	if (lights.size() == 3) {
		std::cout << "Limit of 3 lights reached." << std::endl;
	}
	Light* light = new Light(pos, color);
	lights.push_back(light);
}


void LightsHandler::createDSLs() {
	layouts::lights::Attrs_PV_Depth(&dsl_Attrs, 0, 1, 1, 1, 2, lights.size());

	layouts::lights::Attrs_PVM(&dsl_Attrs_PVM, 0, 1, 1, 1);

	layouts::lights::PV(&dsl_PVs, 0, 1);

	layouts::lights::PV(&dsl_PV, 0, 1);
}

void LightsHandler::createUs() {
	uniforms::create(u_Attrs, presentation->swapchain.images.size(), lights.size() * sizeof(descriptors::lights::Attrs), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	uniforms::create(u_PV, presentation->swapchain.images.size(), lights.size() * sizeof(descriptors::lights::PV), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	for (auto light : lights) {
		light->createUs();
	}
}


void LightsHandler::updateUs(uint32_t index) {
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

		light->updateUs(index);
	}

	updateU_Attrs(index);
	updateU_PV(index);
}

void LightsHandler::updateU_Attrs(uint32_t index) {
	uint32_t nLights = lights.size();

	std::vector<descriptors::lights::Attrs> Attrs;
	Attrs.resize(nLights);

	for (uint32_t i = 0; i < nLights; i++) {
		Attrs[i].pos = glm::vec4(lights[i]->pos, 1.0);
		Attrs[i].color = glm::vec4(lights[i]->color, 1.0);
	}

	uniforms::update(u_Attrs, index, nLights * sizeof(descriptors::lights::Attrs), Attrs.data());
}

void LightsHandler::updateU_PV(uint32_t index) {
	size_t nLights = lightsHandler->lights.size();

	std::vector<descriptors::lights::PV> PVs;
	PVs.resize(nLights);

	for (uint32_t i = 0; i < nLights; i++) {
		PVs[i].PV = lights[i]->projectionView;
	}

	uniforms::update(u_PV, index, nLights * sizeof(descriptors::lights::PV), PVs.data());
}

void LightsHandler::createPipelines() {
	pipelines::types::light::create(pipeline, dsl_Attrs_PVM);
	shadowPipeline->create();
}

void LightsHandler::createDSs() {
	createDS_Attrs_PV_Depth();
	createDS_PVs();
	createDS_PV();
	createDS_PVM();
}

void LightsHandler::createDS_Attrs_PV_Depth() {
	ds_Attrs_PV_Depth.resize(presentation->swapchain.images.size());
	dsets::lights::Attrs_PV_Depth(ds_Attrs_PV_Depth, &dsl_Attrs, u_Attrs, u_PV);
}

void LightsHandler::createDS_PV() {
	for (auto light : lights) {
		light->createDS_PV();
	}
}

void LightsHandler::createDS_PVs() {
	ds_PVs.resize(presentation->swapchain.images.size());
	dsets::lights::multiPV(ds_PVs, &dsl_PVs, u_PV);
}


void LightsHandler::createDS_PVM() {
	for (auto light : lights) {
		light->createDS_Attrs_PVM();
	}
}
