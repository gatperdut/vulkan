#include "Handlers/Handlers.h"


ModelsHandler::ModelsHandler() {

}


ModelsHandler::~ModelsHandler() {
	for (auto model : models) {
		delete model;
	}
}


void ModelsHandler::createDescriptorSetLayouts() {
	for (auto model : models) {
		model->createDescriptorSetLayout();
		model->createDescriptorSetLayoutGeometry();
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
		model->createDescriptorSetsGeometry();
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
		model->updateUBO(index);
	}
}
