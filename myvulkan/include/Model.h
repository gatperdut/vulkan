#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Addons/TextureAddon.h"
#include "Addons/UboHandler.h"


class Model {
private:
	void loadModel();
	void loadTexture(std::string);

public:
	Model(std::string, std::string, std::string, glm::vec3);
	~Model();

	void createUBOs();
	void updateUBOs(uint32_t);
	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();
	VkDeviceSize totalSize();
	void createDescriptorSets();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	glm::vec3 pos;

	UboHandler* uboHandler;
	std::string path;
	std::string filename;

	TextureAddon* textureAddon;

	std::vector<VkDescriptorSet> descriptorSets;
};

