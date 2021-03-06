#pragma once
#include "myve_device.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <string>

namespace myve
{
	class Texture
	{
	public:
		Texture(Device &device, const std::string &path);
		void cleanUp();
		VkImageView getImageView() const { return textureImageView; }
		VkSampler getSampler() const { return textureSampler; }

		void createTextureImage();
		void createTextureImageView();
		void createTextureSampler();
		static void createImage(Device &device, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t mipLevels);
		static VkImageView createImageView(Device &device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

		~Texture();
	private:
		void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);


		std::string path;

		Device& device;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		
		static uint32_t mipLevels;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		VkImageView textureImageView;
		VkSampler textureSampler;
	};
}