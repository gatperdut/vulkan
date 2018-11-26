#pragma once


#include <set>
#include <array>
#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


struct ModelVertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
	uint32_t texIndex;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(ModelVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(ModelVertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(ModelVertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(ModelVertex, color);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(ModelVertex, texCoord);

		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32_UINT;
		attributeDescriptions[4].offset = offsetof(ModelVertex, texIndex);

		return attributeDescriptions;
	}

	bool operator==(const ModelVertex& other) const {
		return pos == other.pos && normal == other.normal && color == other.color && texCoord == other.texCoord && texIndex == other.texIndex;
	}
};

namespace std {
	template<> struct hash<ModelVertex> {
		size_t operator()(ModelVertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.normal) << 1) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 2) ^ (hash<glm::vec2>()(vertex.texCoord) << 1) ^ (hash<uint32_t>()(vertex.texIndex) << 1);
		}
	};
}
