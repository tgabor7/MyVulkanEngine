#include "myve_sprite_renderer.hpp"

namespace myve
{
	SpriteRenderer::SpriteRenderer(const std::string shaderPath, Device& device, Swapchain& swapchain, const VkRenderPass& renderPass) : SimpleRenderer(shaderPath, device, swapchain, renderPass)
	{

	}
	void SpriteRenderer::draw(const VkCommandBuffer& commandBuffer, uint32_t currentImage)
	{
		shader->bind(commandBuffer);

		ubo->bind(commandBuffer, pipelineLayout, currentImage);

		/*for (Sprite& m : sprites)
		{
			PushConstantData d;

			shader->pushConstants(commandBuffer, sizeof(PushConstantData), d);

			m.bind(commandBuffer);
			m.draw(commandBuffer);
		}*/
	}
	void SpriteRenderer::update(const VkCommandBuffer& commandBuffer, uint32_t currentImage)
	{

	}
}