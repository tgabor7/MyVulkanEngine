#pragma once
#include "myve_device.hpp"
#include "myve_swapchain.hpp"
#include <glm/mat4x4.hpp>
#include "myve_texture.hpp"
#include "myve_camera.hpp"

namespace myve
{
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class UBO {
	public:
		UBO(Device &device, Swapchain& swapchain, Texture &texture);
		void cleanUp();
		void createUniformBuffers();
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();
		void bind(VkCommandBuffer commandBuffer,VkPipelineLayout pipelineLayout, uint32_t currentImage);
		void update(uint32_t currentImage, VkExtent2D swapChainExtent);

		VkDescriptorSetLayout &getSetLayout() { return descriptorSetLayout; }
		Texture& texture;

		~UBO();
	private:
		Camera c{};

		Device& device;
		Swapchain& swapchain;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
	};
}