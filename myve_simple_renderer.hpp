#pragma once
#include "myve_shader.hpp"
#include "myve_game_object.hpp"

namespace myve
{
	class SimpleRenderer 
	{
	public:
		SimpleRenderer(const std::string shaderPath, Device& device, Swapchain& swapchain, const VkRenderPass& renderPass);
		virtual void draw(const VkCommandBuffer& commandBuffer, uint32_t currentImage) = 0;
		virtual void initialize() = 0;

		void cleanUp();

		~SimpleRenderer();
	protected:


		std::unique_ptr<Shader> shader;
		std::shared_ptr<UBO> ubo;
		std::shared_ptr<Texture> texture;
		std::vector<GameObject> gameObjects;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo;
		VkPipelineLayout pipelineLayout;

		Device& device;
		Swapchain& swapchain;
	};
}