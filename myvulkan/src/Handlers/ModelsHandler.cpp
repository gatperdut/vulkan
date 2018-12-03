#include "Handlers/Handlers.h"
#include "Devices/logical.h"
#include "Layouts/model_l.h"



ModelsHandler::ModelsHandler() {

}


ModelsHandler::~ModelsHandler() {
	for (auto model : models) {
		delete model;
	}
	vkDestroyDescriptorSetLayout(devices::logical::dev, dsl_PVM, nullptr);
}


void ModelsHandler::createDSLs() {
	for (auto model : models) {
		model->createDSL_PVM_Materials();
	}

	createDSL_PVM();
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
		pipelines::destroy(model->pipeline);
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

void ModelsHandler::updateUs(uint32_t index) {
	for (auto model : models) {
		model->updateU_PVM(index);
	}
}


void ModelsHandler::createDSL_PVM() {
	layouts::models::PVM(&dsl_PVM, 0, 1);
}