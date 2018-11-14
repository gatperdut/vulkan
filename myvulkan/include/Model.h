#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Addons/TextureAddon.h"
#include "Addons/UboHandler.h"
#include "Addons/BufferHandler.h"


class Model {
private:
	void loadModel();

public:
	Model(std::string, std::string, glm::vec3);
	~Model();

	void createUBOs();
	void updateUBOs(uint32_t);
	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();
	VkDeviceSize totalSize();
	void createDescriptorSetLayout();
	void createDescriptorSets();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	glm::vec3 pos;

	std::string path;
	std::string filename;

	UboHandler* uboHandler;
	TextureAddon* textureAddon;
	BufferHandler* bufferHandler;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
};

