#pragma once


#include <vulkan/vulkan.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Vertices/description_v.h"


namespace vertices {

	namespace V_P {

		struct Data {
			glm::vec3 pos;

			bool operator==(const Data& other) const {
				return pos == other.pos;
			}
		};

		extern vertices::Description description;

		void fill();

	}

}

namespace std {
	template<> struct hash<vertices::V_P::Data> {
		size_t operator()(vertices::V_P::Data const& vertex) const {
			return hash<glm::vec3>()(vertex.pos);
		}
	};
}