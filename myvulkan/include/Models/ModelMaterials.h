#pragma once


#include <vector>
#include <vulkan/vulkan.h>


class ModelMaterials {
private:
	void addImage(std::string);
	void addMipmaps(VkImage, VkFormat, int32_t, int32_t, uint32_t);
	void addImageView();
	void addSampler();

public:
	ModelMaterials();
	~ModelMaterials();

	void addTexture(std::string);
	bool hasTexture(std::string);
	int32_t indexByFilepath(std::string);

	std::vector<std::string> filepaths;
	std::vector<VkImage> images;
	std::vector<uint32_t> mipLevels;
	std::vector<VkDeviceMemory> imageMemories;
	std::vector<VkImageView> imageViews;
	std::vector<VkSampler> samplers;
};

