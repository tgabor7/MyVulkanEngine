#pragma once
#include "myve_device.hpp"

namespace myve
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Swapchain {

	public:
		Swapchain(Device& device, Window &window);
		~Swapchain();

		void cleanUp();
		
		VkExtent2D getExtent() const { return swapChainExtent; }
		VkFormat getImageFormat() const { return swapChainImageFormat; }
		std::vector<VkImageView> getSwapChainImageViews() const { return swapChainImageViews; }
		size_t getSwapChainImageCount() const { return swapChainImageViews.size(); }
		VkSwapchainKHR getSwapchain() const { return swapChain; }

		void createSwapchain();
		void createImageViews();


		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	private:
		
		
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);


		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;

		Device& device;
		GLFWwindow* window;

	};
}