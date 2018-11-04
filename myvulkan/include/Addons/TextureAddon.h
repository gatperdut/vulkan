#pragma once


#include <vulkan/vulkan.h>


class TextureAddon {
public:
	TextureAddon(std::string path);
	~TextureAddon();

	void createTextureImage();
	void generateMipmaps(VkImage, VkFormat, int32_t, int32_t, uint32_t);
	void createTextureImageView();
	void createTextureSampler();


	std::string path;
	VkImage textureImage;
	uint32_t mipLevels;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
};

