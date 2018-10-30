#pragma once


#include <vector>

#include "vertex.h"


class ModelsHandler {
public:
	ModelsHandler();
	~ModelsHandler();

	void loadModel();


	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};
