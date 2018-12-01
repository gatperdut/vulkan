#pragma once


#include <set>
#include <array>
#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


namespace vertices {

	struct V_P {
		glm::vec3 pos;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(V_P);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(V_P, pos);

			return attributeDescriptions;
		}

		bool operator==(const V_P& other) const {
			return pos == other.pos;
		}
	};

}

namespace std {
	template<> struct hash<vertices::V_P> {
		size_t operator()(vertices::V_P const& vertex) const {
			return hash<glm::vec3>()(vertex.pos);
		}
	};
}