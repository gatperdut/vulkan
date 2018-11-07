#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "Addons/TexturesHandler.h"
#include "vertex.h"


class Model {
private:
	void load();
	void addTexture(std::string);

public:
	Model(std::string, std::string, glm::vec3);
	~Model();

	VkDeviceSize verticesSize();
	VkDeviceSize indicesSize();


	glm::vec3 pos;
	std::string filename;
	std::string path;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	TexturesHandler* texturesHandler;
};

