#pragma once


#include <vector>
#include <vulkan/vulkan.h>


class TexturesHandler {
private:
	void addImage(std::string, VkImage*, VkDeviceMemory*);
	void generateMipmaps(VkImage, VkFormat, int32_t, int32_t, uint32_t);
	VkImageView addImageView(VkImage);
	VkSampler addSampler();

public:
	TexturesHandler(std::string path);
	~TexturesHandler();

	void addTexture(std::string);


	std::string path;
	std::vector<VkImage> images;
	std::vector<VkDeviceMemory> imageMemories;
	std::vector<VkImageView> imageViews;
	std::vector<VkSampler> imageSamplers;
	uint32_t mipLevels;
};

