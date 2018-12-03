#pragma once


#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "Vertices/description_v.h"


namespace vertices {

	namespace V_P_N_C_TXC_TXI {

		struct Data {
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec3 color;
			glm::vec2 texCoord;
			uint32_t texIndex;

			bool operator==(const Data& other) const {
				return pos == other.pos && normal == other.normal && color == other.color && texCoord == other.texCoord && texIndex == other.texIndex;
			}
		};

		extern vertices::Description description;

		void fill();

	}

}

namespace std {
	template<> struct hash<vertices::V_P_N_C_TXC_TXI::Data> {
		size_t operator()(vertices::V_P_N_C_TXC_TXI::Data const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.normal) << 1) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 2) ^ (hash<glm::vec2>()(vertex.texCoord) << 1) ^ (hash<uint32_t>()(vertex.texIndex) << 1);
		}
	};
}
