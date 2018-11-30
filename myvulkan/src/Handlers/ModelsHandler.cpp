#include "Handlers/Handlers.h"
#include "Layouts/model_l.h"



ModelsHandler::ModelsHandler() {

}


ModelsHandler::~ModelsHandler() {
	for (auto model : models) {
		delete model;
	}
	vkDestroyDescriptorSetLayout(devicesHandler->device, descriptorSetLayoutMatrices, nullptr);
}


void ModelsHandler::createDescriptorSetLayouts() {
	for (auto model : models) {
		model->createDescriptorSetLayout();
	}
}


std::vector<VkDescriptorSetLayout> ModelsHandler::getDescriptorSetLayouts() {
	std::vector<VkDescriptorSetLayout> result;
	for (auto model : models) {
		result.push_back(model->descriptorSetLayout);
	}
	return result;
}


void ModelsHandler::createDescriptorSets() {
	for (auto model : models) {
		model->createDescriptorSets();
		model->createDescriptorSetsMatrices();
	}
}

void ModelsHandler::createPipelines() {
	for (auto model : models) {
		model->createPipeline();
	}
}


void ModelsHandler::destroyPipelines() {
	for (auto model : models) {
		model->modelPipeline->freeResources();
	}
}


void ModelsHandler::load(std::string path, std::string filename, glm::vec3 pos, glm::vec3 scale) {
	Model* model = new Model(path, filename, pos, scale);
	models.push_back(model);
}


void ModelsHandler::createUBOs() {
	for (auto model : models) {
		model->createUBOs();
	}
}

void ModelsHandler::updateUBOs(uint32_t index) {
	for (auto model : models) {
		model->update_PVM_u(index);
	}
}


void ModelsHandler::createDSL_PVM() {
	layouts::models::PVM(&descriptorSetLayoutMatrices, 0, 1);
}