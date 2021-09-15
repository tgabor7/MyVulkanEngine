#include "myve_simple_renderer.hpp"
#include <stdexcept>

namespace myve
{
	SimpleRenderer::SimpleRenderer(const std::string shaderPath, Device& device, Swapchain& swapchain, const VkRenderPass& renderPass) : device{ device }, swapchain{swapchain}
	{
		texture = std::make_shared<Texture>(device, "textures/bunny-atlas.jpg");

		texture->createTextureImage();
		texture->createTextureImageView();
		texture->createTextureSampler();

		ubo = std::make_shared<UBO>(device, swapchain, *texture);

		ubo->createDescriptorSetLayout();

		ubo->createUniformBuffers();
		ubo->createDescriptorPool();
		ubo->createDescriptorSets();

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &ubo->getSetLayout();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(device.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		shader = std::make_unique<Shader>(device, swapchain, shaderPath, pipelineLayout, renderPass);

		//initialize();
	}
	SimpleRenderer::~SimpleRenderer()
	{
	}
}