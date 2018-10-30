#pragma once


#include <vector>
#include <vulkan/vulkan.h>


class SynchrosHandler {
public:
	SynchrosHandler();
	~SynchrosHandler();

	void createSyncObjects();


	static const int MAX_FRAMES_IN_FLIGHT = 2;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
};

