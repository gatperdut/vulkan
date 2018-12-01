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


void ModelsHandler::createDSLs() {
	for (auto model : models) {
		model->createDSL_PVM_Materials();
	}

	createDSL_PVM();
}


std::vector<VkDescriptorSetLayout> ModelsHandler::getDescriptorSetLayouts() {
	std::vector<VkDescriptorSetLayout> result;
	for (auto model : models) {
		result.push_back(model->dsl_PVM_Materials);
	}
	return result;
}


void ModelsHandler::createDSs() {
	for (auto model : models) {
		model->createDS_PVM_Materials();
		model->createDS_PVM();
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


void ModelsHandler::createUs() {
	for (auto model : models) {
		model->createUs();
	}
}

void ModelsHandler::updateUBOs(uint32_t index) {
	for (auto model : models) {
		model->updateU_PVM(index);
	}
}


void ModelsHandler::createDSL_PVM() {
	layouts::models::PVM(&descriptorSetLayoutMatrices, 0, 1);
}