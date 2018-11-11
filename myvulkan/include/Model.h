#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Addons/TextureAddon.h"
#include "vertex.h"


class Model {
private:
	void loadModel();
	void loadTexture(std::string);

public:
	Model(std::string, std::string, std::string, glm::vec3);
	~Model();

	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();


	glm::vec3 pos;
	std::string path;
	std::string filename;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	TextureAddon* textureAddon;
};

