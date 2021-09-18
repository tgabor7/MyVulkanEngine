#pragma once
#include "myve_device.hpp"
#include "myve_shader.hpp"
#include "myve_swapchain.hpp"
#include "myve_vbo.hpp"
#include "myve_ubo.hpp"
#include <chrono>
#include "myve_game_object.hpp"
#include "myve_mesh_renderer.hpp"
namespace myve
{
	class Pipeline {
	public:
		Pipeline(Device& device, Swapchain& swapchain, GLFWwindow* window);
		void drawFrame();
		void init();
		void cleanUp();
		void recreateSwapChain();
		bool framebufferResized = false;
		
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		
		~Pipeline();

	private:

		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::chrono::time_point<std::chrono::high_resolution_clock> old_time;
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
		size_t currentFrame = 0;
		std::unique_ptr<MeshRenderer> mesh_renderer;
	};
}
