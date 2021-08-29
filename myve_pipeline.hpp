#pragma once
#include "myve_device.hpp"
#include "myve_shader.hpp"
#include "myve_swapchain.hpp"

namespace myve
{
	class Pipeline {
	public:
		Pipeline(Device& device, Swapchain& swapchain);
		void drawFrame();
		void cleanUp();
		~Pipeline();

	private:

		const int MAX_FRAMES_IN_FLIGHT = 2;

		void createPipeline();
		void createRenderPass();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSyncObjects();

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

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<Shader> shader;
		size_t currentFrame = 0;
	};
}