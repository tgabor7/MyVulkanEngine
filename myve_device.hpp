#pragma once 
#include <map>
#include <optional>
#include "myve_window.hpp"

namespace myve
{
	struct QueueFamilyIndices {
		
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};

	class Device {
	public:

		Device(VkInstance instance, VkSurfaceKHR surface);

		VkDevice getDevice() const { return device; }
		VkSurfaceKHR getSurface() const { return surface; }
		VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		VkQueue getPresentQueue() const { return presentQueue; }
		VkQueue getGraphicsQueue() const { return graphicsQueue; }
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkCommandPool getCommandPool() const { return commandPool; }
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void createCommandPool();
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void cleanUp();
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		~Device();


	private:
		void pickPhysicalDevice();
		void createLogicalDevice();

		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		
		

		VkQueue presentQueue;
		VkPhysicalDevice physicalDevice;
		VkInstance instance;
		VkSurfaceKHR surface;
		VkDevice device;
		VkQueue graphicsQueue;
		VkCommandPool commandPool;
	};
}
