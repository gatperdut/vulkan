#include "Timer/timer.h"


#include <chrono>


namespace timer {

	float lapse() {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();

		return std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	}

}