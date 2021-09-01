#pragma once
#include "myve_device.hpp"
#include "myve_shader.hpp"
#include "myve_swapchain.hpp"
#include "myve_vbo.hpp"
#include "myve_ubo.hpp"

namespace myve
{
	class Pipeline {
	public:
		Pipeline(Device& device, Swapchain& swapchain, GLFWwindow* window, VBO &vbo, UBO &ubo);
		void drawFrame();
		void cleanUp();
		void recreateSwapChain();
		bool framebufferResized = false;
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		~Pipeline();

	private:

		const int MAX_FRAMES_IN_FLIGHT = 2;

		void createPipeline();
		void createRenderPass();
		void createFramebuffers();
		void createCommandBuffers();
		void createSyncObjects();
		void cleanUpSwapchain();

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

		GLFWwindow* window;
		VBO &vbo;
		UBO& ubo;
		std::unique_ptr<Shader> shader;
		size_t currentFrame = 0;
	};
}