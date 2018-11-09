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
	bool hasTexture(std::string);
	int32_t textureIndex(std::string);

	void createDescriptorSetLayout();
	void createDescriptorSets();


	std::string path;
	std::vector<std::string> filenames;
	std::vector<VkImage> images;
	std::vector<VkDeviceMemory> imageMemories;
	std::vector<VkImageView> imageViews;
	std::vector<VkSampler> imageSamplers;
	uint32_t mipLevels;

	VkDescriptorSetLayout descriptorSetLayoutCIS;
	std::vector<VkDescriptorSet> descriptorSets;
};

