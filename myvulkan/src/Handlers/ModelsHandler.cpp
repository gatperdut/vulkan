#include "Handlers/ModelsHandler.h"


ModelsHandler::ModelsHandler() {

}


ModelsHandler::~ModelsHandler() {
	for (auto model : models) {
		delete model;
	}
}


void ModelsHandler::load(std::string modelPath, std::string texturePath, glm::vec3 pos) {
	Model* model = new Model(modelPath, texturePath, pos);
	models.push_back(model);
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