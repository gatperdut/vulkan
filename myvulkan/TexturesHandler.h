#pragma once


const std::string TEXTURE_PATH = "textures/chalet.jpg";


#include <vulkan/vulkan.h>


class TexturesHandler {
public:
	TexturesHandler();
	~TexturesHandler();

	void createTextureImage();
	void generateMipmaps(VkImage, VkFormat, int32_t, int32_t, uint32_t);
	void createTextureImageView();
	void createTextureSampler();


	VkImage textureImage;
	uint32_t mipLevels;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
};

