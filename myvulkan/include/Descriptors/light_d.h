#pragma once


#include <glm/glm.hpp>


namespace descriptors {

	namespace lights {

		struct Attrs {
			glm::vec4 pos;
			glm::vec4 color;
		};

		struct PVM {
			glm::mat4 P;
			glm::mat4 V;
			glm::mat4 M;
		};

		struct PV {
			glm::mat4 PV;
		};

	}

}
