#pragma once


#include <glm/glm.hpp>


namespace descriptors {

	namespace models {
		
		struct PVM {
			glm::mat4 P;
			glm::mat4 V;
			glm::mat4 M;
		};

	}

}
