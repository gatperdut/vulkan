#include "Handlers/Handlers.h"
#include "Handlers/ModelsHandler.h"


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
	}
}


void ModelsHandler::createDescriptorSets() {
	for (auto model : models) {
		model->createDescriptorSets();
	}
}


void ModelsHandler::load(std::string path, std::string filename, glm::vec3 pos) {
	Model* model = new Model(path, filename, pos);
	models.push_back(model);
}


void ModelsHandler::createUBOs() {
	for (auto model : models) {
		model->createUBOs();
	}
}

VkDeviceSize ModelsHandler::verticesSize() {
	VkDeviceSize total = 0;
	for (auto model : models) {
		total += model->verticesSize();
	}
	
	return total;
}


VkDeviceSize ModelsHandler::indicesSize() {
	VkDeviceSize total = 0;
	for (auto model : models) {
		total += model->indicesSize();
	}

	return total;
}