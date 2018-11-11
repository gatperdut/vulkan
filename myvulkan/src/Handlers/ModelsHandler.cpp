#include "Handlers/ModelsHandler.h"


ModelsHandler::ModelsHandler() {

}


ModelsHandler::~ModelsHandler() {
	for (auto model : models) {
		delete model;
	}
}


void ModelsHandler::load(std::string path, std::string filename, std::string textureFilename, glm::vec3 pos) {
	Model* model = new Model(path, filename, textureFilename, pos);
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