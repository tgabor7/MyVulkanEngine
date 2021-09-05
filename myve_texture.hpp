#pragma once
#include "myve_device.hpp"
#define STB_IMAGE_IMPLEMENTATION


namespace myve
{
	class Texture
	{
	public:
		Texture(Device &device);
		void cleanUp();
		VkImageView getImageView() const { return textureImageView; }
		VkSampler getSampler() const { return textureSampler; }

		void createTextureImage();
		void createTextureImageView();
		void createTextureSampler();

		~Texture();
	private:
		
		VkImageView createImageView(VkImage image, VkFormat format);
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		Device& device;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		VkImageView textureImageView;
		VkSampler textureSampler;
	};
}