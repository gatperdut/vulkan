#include "Handlers/Handlers.h"


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
		model->updateUBO(index);
	}
}


VkDescriptorSetLayoutBinding ModelsHandler::createDescriptorSetLayoutBinding() {
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = 0;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	return layoutBinding;
}


void ModelsHandler::createDescriptorSetLayoutMatrices() {
	VkDescriptorSetLayoutBinding layoutBindingUB = createDescriptorSetLayoutBinding();

	std::vector<VkDescriptorSetLayoutBinding> bindings = { layoutBindingUB };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = bindings.size();
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(devicesHandler->device, &layoutInfo, nullptr, &descriptorSetLayoutMatrices) != VK_SUCCESS) {
		throw std::runtime_error("failed to create CIS descriptor set layout!");
	}
}