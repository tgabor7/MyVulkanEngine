#pragma once
#include "myve_device.hpp"
#include "myve_shader.hpp"
#include "myve_swapchain.hpp"
#include "myve_vbo.hpp"
#include "myve_ubo.hpp"
#include <chrono>
#include "myve_game_object.hpp"

namespace myve
{
	class Pipeline {
	public:
		Pipeline(Device& device, Swapchain& swapchain, GLFWwindow* window);
		void drawFrame();
		void init();
		void addGameObject(GameObject& gameObject);
		void cleanUp();
		void recreateSwapChain();
		bool framebufferResized = false;
		
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		
		std::vector<GameObject> gameObjects;

		~Pipeline();

	private:

		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::chrono::steady_clock::time_point old_time = std::chrono::high_resolution_clock::now();
		uint32_t fps = 0;

		void createPipeline();
		void createRenderPass();
		void createFramebuffers();
		void createCommandBuffers();
		void createSyncObjects();
		void cleanUpSwapchain();
		void createDepthResources();
		void createColorResources();
		VkFormat findDepthFormat();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		Device& device;
		Swapchain& swapchain;
		VkCommandPool commandPool;

		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
		VkImageView colorImageView;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;

		GLFWwindow* window;
		std::unique_ptr<Shader> shader;
		size_t currentFrame = 0;
	};
}