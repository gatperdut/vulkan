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

		static VkVertexInputBindingDescription bindings() {
			VkVertexInputBindingDescription binding = {};
			binding.binding = 0;
			binding.stride = sizeof(V_P);
			binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return binding;
		}

		static std::array<VkVertexInputAttributeDescription, 1> attributes() {
			std::array<VkVertexInputAttributeDescription, 1> attributes = {};

			attributes[0].binding = 0;
			attributes[0].location = 0;
			attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributes[0].offset = offsetof(V_P, pos);

			return attributes;
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