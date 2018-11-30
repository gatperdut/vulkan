#include <chrono>
#include <math.h>

#include "Handlers/Handlers.h"
#include "Bindings/bindings.h"
#include "DSets/light_ds.h"
#include "Layouts/light_l.h"


LightsHandler::LightsHandler() {
	lightDataUBOs = new LightDataUBOs;
	lightSpaceUBOs = new LightSpaceUBOs;
	lightPipeline = new LightPipeline;
	shadowPipeline = new ShadowPipeline;
}


LightsHandler::~LightsHandler() {
	for (auto light : lights) {
		delete light;
	}

	delete lightDataUBOs;
	delete lightSpaceUBOs;
	delete lightPipeline;
	delete shadowPipeline;
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutData, nullptr);
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutModel, nullptr);
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




void LightsHandler::createDescriptorSetLayoutData() {
	layouts::lights::Properties_PV_Depth(&descriptorSetLayoutData, 0, 1, 1, 1, 2, lights.size());
}


void LightsHandler::createDescriptorSetLayoutModel() {
	layouts::lights::PVM(&descriptorSetLayoutModel, 0, 1);
}


void LightsHandler::createDescriptorSetLayoutSpace() {
	layouts::lights::PV(&descriptorSetLayoutSpace, 0, 1);
}


void LightsHandler::createDescriptorSetLayoutSingleSpace() {
	layouts::lights::PV(&descriptorSetLayoutSingleSpace, 0, 1);
}


void LightsHandler::createUBOs() {
	lightDataUBOs->createUniformBuffers();
	lightSpaceUBOs->createUniformBuffers();
	for (auto light : lights) {
		light->createUBOs();
	}
}


void LightsHandler::updateUBOs(uint32_t index) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	for (size_t i = 0; i < lights.size(); i++) {
		Light* light = lights[i];
		float factor = 1.0f;
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
	for (auto light : lights) {
	}
	lightDataUBOs->updateDataUBO(index);
	lightSpaceUBOs->updateUniformBuffer(index);
}


void LightsHandler::createDescriptorSetsData() {
	dsets_Properties_PV_Depth.resize(presentation->swapchain.images.size());
	dsets::lights::Properties_PV_Depth(dsets_Properties_PV_Depth, &descriptorSetLayoutData, lightDataUBOs, lightSpaceUBOs);
}

void LightsHandler::createDescriptorSetsSingleSpace() {
	for (auto light : lights) {
		light->createDescriptorSetsSpace();
	}
}

void LightsHandler::createDescriptorSetsSpace() {
	dsets_multiPV.resize(presentation->swapchain.images.size());
	dsets::lights::multiPV(dsets_multiPV, &lightsHandler->descriptorSetLayoutSpace, lightSpaceUBOs);
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
