#include "myve_pipeline.hpp"

//std

#include <stdexcept>

namespace myve
{
	Pipeline::Pipeline(Device& device, Swapchain& swapchain, GLFWwindow* window, VBO& vbo, UBO& ubo) : device{ device }, swapchain{ swapchain }, window{ window }, vbo{ vbo }, ubo{ubo}
	{
		createRenderPass();
		ubo.createDescriptorSetLayout();
		createPipeline();
		createFramebuffers();
		
		device.createCommandPool();
		
		vbo.createVertexBuffer();
		vbo.createIndexBuffer();

		ubo.createUniformBuffers();
		ubo.createDescriptorPool();
		ubo.createDescriptorSets();

		createCommandBuffers();
		createSyncObjects();
		glfwSetWindowUserPointer(window, this);

	}
	void Pipeline::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<Pipeline*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
	void Pipeline::cleanUpSwapchain()
	{
		for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
			vkDestroyFramebuffer(device.getDevice(), swapChainFramebuffers[i], nullptr);
		}

		vkFreeCommandBuffers(device.getDevice(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		vkDestroyPipeline(device.getDevice(), graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device.getDevice(), pipelineLayout, nullptr);
		vkDestroyRenderPass(device.getDevice(), renderPass, nullptr);

		for (size_t i = 0; i < swapchain.getSwapChainImageCount(); i++) {
			vkDestroyImageView(device.getDevice(), swapchain.getSwapChainImageViews()[i], nullptr);
		}

		vkDestroySwapchainKHR(device.getDevice(), swapchain.getSwapchain(), nullptr);
	}
	void Pipeline::recreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.getDevice());

		cleanUpSwapchain();

		swapchain.createSwapchain();
		swapchain.createImageViews();
		createRenderPass();
		createPipeline();
		createFramebuffers();
		ubo.createUniformBuffers();
		ubo.createDescriptorPool();
		ubo.createDescriptorSets();

		createCommandBuffers();

		imagesInFlight.resize(swapchain.getSwapChainImageCount(), VK_NULL_HANDLE);

	}
	void Pipeline::createSyncObjects()
	{
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(swapchain.getSwapChainImageCount(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device.getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create synchronization objects for a frame!");
			}
		}
	}
	Pipeline::~Pipeline()
	{

	}
	void Pipeline::drawFrame()
	{
		vkWaitForFences(device.getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(device.getDevice(), swapchain.getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(device.getDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		ubo.update(imageIndex, swapchain.getExtent());

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device.getDevice(), 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapchain.getSwapchain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}
	void Pipeline::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapchain.getImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(device.getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void Pipeline::createFramebuffers()
	{
		swapChainFramebuffers.resize(swapchain.getSwapChainImageCount());

		for (size_t i = 0; i < swapchain.getSwapChainImageCount(); i++) {
			VkImageView attachments[] = {
				swapchain.getSwapChainImageViews()[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapchain.getExtent().width;
			framebufferInfo.height = swapchain.getExtent().height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}
	void Pipeline::createCommandBuffers()
	{
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = device.getCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapchain.getExtent();

			VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			vbo.bind(commandBuffers[i]);

			ubo.bind(commandBuffers[i], pipelineLayout, i);

			//vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			vkCmdDrawIndexed(commandBuffers[i], vbo.getIndexCount(), 1, 0, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}
	void Pipeline::createPipeline()
	{

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1; // Optional
		pipelineLayoutInfo.pSetLayouts = &ubo.getSetLayout(); // Optional

		if (vkCreatePipelineLayout(device.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		shader = std::make_unique<Shader>(device, swapchain, "shaders/simple_shader", pipelineLayout, &renderPass, &graphicsPipeline, &ubo);

	}
	void Pipeline::cleanUp()
	{
		vkDestroySemaphore(device.getDevice(), renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device.getDevice(), imageAvailableSemaphore, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device.getDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(device.getDevice(), imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device.getDevice(), inFlightFences[i], nullptr);
		}


		vkDestroyCommandPool(device.getDevice(), commandPool, nullptr);
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(device.getDevice(), framebuffer, nullptr);
		}

		vkDestroyPipeline(device.getDevice(), graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device.getDevice(), pipelineLayout, nullptr);
		vkDestroyRenderPass(device.getDevice(), renderPass, nullptr);
	}
}